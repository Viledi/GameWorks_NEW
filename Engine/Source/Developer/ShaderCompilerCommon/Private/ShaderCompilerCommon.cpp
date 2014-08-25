// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	ShaderCompilerCommon.cpp: Common functionality for platform Shader Compilers.
=============================================================================*/

#include "ShaderCompilerCommon.h"
#include "ModuleManager.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, ShaderCompilerCommon);

int16 GetNumUniformBuffersUsed(const FShaderResourceTable& InSRT)
{
	auto CountLambda = [&](const TArray<uint32>& In)
					{
						int16 LastIndex = -1;
						for (int32 i = 0; i < In.Num(); ++i)
						{
							auto BufferIndex = FRHIResourceTableEntry::GetUniformBufferIndex(In[i]);
							if (BufferIndex != FRHIResourceTableEntry::GetEndOfStreamToken())
							{
								LastIndex = FMath::Max(LastIndex, (int16)BufferIndex);
							}
						}

						return LastIndex + 1;
					};
	int16 Num = CountLambda(InSRT.SamplerMap);
	Num = FMath::Max(Num, (int16)CountLambda(InSRT.ShaderResourceViewMap));
	Num = FMath::Max(Num, (int16)CountLambda(InSRT.TextureMap));
	Num = FMath::Max(Num, (int16)CountLambda(InSRT.UnorderedAccessViewMap));
	return Num;
}


void BuildResourceTableTokenStream(const TArray<uint32>& InResourceMap, int32 MaxBoundResourceTable, TArray<uint32>& OutTokenStream)
{
	// First we sort the resource map.
	TArray<uint32> SortedResourceMap = InResourceMap;
	SortedResourceMap.Sort();

	// The token stream begins with a table that contains offsets per bound uniform buffer.
	// This offset provides the start of the token stream.
	OutTokenStream.AddZeroed(MaxBoundResourceTable+1);
	auto LastBufferIndex = FRHIResourceTableEntry::GetEndOfStreamToken();
	for (int32 i = 0; i < SortedResourceMap.Num(); ++i)
	{
		auto BufferIndex = FRHIResourceTableEntry::GetUniformBufferIndex(SortedResourceMap[i]);
		if (BufferIndex != LastBufferIndex)
		{
			// Store the offset for resources from this buffer.
			OutTokenStream[BufferIndex] = OutTokenStream.Num();
			LastBufferIndex = BufferIndex;
		}
		OutTokenStream.Add(SortedResourceMap[i]);
	}

	// Add a token to mark the end of the stream. Not needed if there are no bound resources.
	if (OutTokenStream.Num())
	{
		OutTokenStream.Add(FRHIResourceTableEntry::GetEndOfStreamToken());
	}
}

void BuildResourceTableMapping(
	const TMap<FString,FResourceTableEntry>& ResourceTableMap,
	const TMap<FString,uint32>& ResourceTableLayoutHashes,
	TBitArray<>& UsedUniformBufferSlots,
	FShaderParameterMap& ParameterMap,
	FShaderResourceTable& OutSRT)
{
	check(OutSRT.ResourceTableBits == 0);
	check(OutSRT.ResourceTableLayoutHashes.Num() == 0);

	// Build resource table mapping
	int32 MaxBoundResourceTable = -1;
	TArray<uint32> ResourceTableSRVs;
	TArray<uint32> ResourceTableSamplerStates;
	TArray<uint32> ResourceTableUAVs;

	for( auto MapIt = ResourceTableMap.CreateConstIterator(); MapIt; ++MapIt )
	{
		const FString& Name	= MapIt->Key;
		const FResourceTableEntry& Entry = MapIt->Value;

		uint16 BufferIndex, BaseIndex, Size;
		if (ParameterMap.FindParameterAllocation( *Name, BufferIndex, BaseIndex, Size ) )
		{
			ParameterMap.RemoveParameterAllocation(*Name);

			uint16 UniformBufferIndex = INDEX_NONE, UBBaseIndex, UBSize;
			if (ParameterMap.FindParameterAllocation(*Entry.UniformBufferName, UniformBufferIndex, UBBaseIndex, UBSize) == false)
			{
				UniformBufferIndex = UsedUniformBufferSlots.FindAndSetFirstZeroBit();
				ParameterMap.AddParameterAllocation(*Entry.UniformBufferName,UniformBufferIndex,0,0);
			}

			OutSRT.ResourceTableBits |= (1 << UniformBufferIndex);
			MaxBoundResourceTable = FMath::Max<int32>(MaxBoundResourceTable, (int32)UniformBufferIndex);

			while (OutSRT.ResourceTableLayoutHashes.Num() <= MaxBoundResourceTable)
			{
				OutSRT.ResourceTableLayoutHashes.Add(0);
			}
			OutSRT.ResourceTableLayoutHashes[UniformBufferIndex] = ResourceTableLayoutHashes.FindChecked(Entry.UniformBufferName);

			auto ResourceMap = FRHIResourceTableEntry::Create(UniformBufferIndex, Entry.ResourceIndex, BaseIndex);
			switch( Entry.Type )
			{
			case UBMT_TEXTURE:
				OutSRT.TextureMap.Add(ResourceMap);
				break;
			case UBMT_SAMPLER:
				OutSRT.SamplerMap.Add(ResourceMap);
				break;
			case UBMT_SRV:
				OutSRT.ShaderResourceViewMap.Add(ResourceMap);
				break;
			case UBMT_UAV:
				OutSRT.UnorderedAccessViewMap.Add(ResourceMap);
				break;
			default:
				check(0);
			}
		}
	}

	OutSRT.MaxBoundResourceTable = MaxBoundResourceTable;
}

bool RemoveUniformBuffersFromSource(FString& SourceCode)
{
	static const FString StaticStructToken(TEXT("static const struct"));
	int32 StaticStructTokenPos = SourceCode.Find(StaticStructToken, ESearchCase::CaseSensitive, ESearchDir::FromStart);
	while (StaticStructTokenPos != INDEX_NONE)
	{
		static const FString CloseBraceSpaceToken(TEXT("} "));
		int32 CloseBraceSpaceTokenPos = SourceCode.Find(CloseBraceSpaceToken, ESearchCase::CaseSensitive, ESearchDir::FromStart, StaticStructTokenPos + StaticStructToken.Len());
		if (CloseBraceSpaceTokenPos == INDEX_NONE)
		{
			check(0);	//@todo-rco: ERROR
			return false;
		}

		int32 NamePos = CloseBraceSpaceTokenPos + CloseBraceSpaceToken.Len();
		static const FString SpaceEqualsToken(TEXT(" ="));
		int32 SpaceEqualsTokenPos = SourceCode.Find(SpaceEqualsToken, ESearchCase::CaseSensitive, ESearchDir::FromStart, NamePos);
		if (SpaceEqualsTokenPos == INDEX_NONE)
		{
			check(0);	//@todo-rco: ERROR
			return false;
		}

		FString UniformBufferName = SourceCode.Mid(NamePos, SpaceEqualsTokenPos - NamePos);
		check(UniformBufferName.Len() > 0);

		static const FString CloseBraceSemicolorToken(TEXT("};"));
		int32 CloseBraceSemicolonTokenPos = SourceCode.Find(CloseBraceSemicolorToken, ESearchCase::CaseSensitive, ESearchDir::FromStart, SpaceEqualsTokenPos + SpaceEqualsToken.Len());
		if (CloseBraceSemicolonTokenPos == INDEX_NONE)
		{
			check(0);	//@todo-rco: ERROR
			return false;
		}

		// Comment out this UB
		auto& SourceCharArray = SourceCode.GetCharArray();
		SourceCharArray[StaticStructTokenPos] = TCHAR('/');
		SourceCharArray[StaticStructTokenPos + 1] = TCHAR('*');
		SourceCharArray[CloseBraceSemicolonTokenPos] = TCHAR('*');
		SourceCharArray[CloseBraceSemicolonTokenPos + 1] = TCHAR('/');

		// Find & Replace this UB
		FString UBSource = UniformBufferName + FString(TEXT("."));
		FString UBDest = UniformBufferName + FString(TEXT("_"));
		SourceCode.ReplaceInline(*UBSource, *UBDest, ESearchCase::CaseSensitive);


		// Find next UB
		StaticStructTokenPos = SourceCode.Find(StaticStructToken, ESearchCase::CaseSensitive, ESearchDir::FromStart, CloseBraceSemicolonTokenPos + 2);
	}

	return true;
}

FString CreateCrossCompilerBatchFileContents(const FString& ShaderFile, const FString& OutputFile, const FString& FrequencySwitch, const FString& EntryPoint, const FString& VersionSwitch, const FString& ExtraArguments)
{
	FString BatchFile = TEXT("@echo off");
	BatchFile += TEXT("\nif defined ue.hlslcc GOTO DONE\nset ue.hlslcc=");
	BatchFile += FPaths::RootDir() / TEXT("Engine\\Source\\ThirdParty\\hlslcc\\hlslcc\\bin\\Win64\\VS2013\\hlslcc_64.exe");
	BatchFile += TEXT("\n\n:DONE\n%ue.hlslcc% ");
	BatchFile += FString::Printf(TEXT("\"%s\" -o=\"%s\" %s -entry=%s %s %s"), *ShaderFile, *OutputFile, *FrequencySwitch, *EntryPoint, *VersionSwitch, *ExtraArguments);
	BatchFile += TEXT("\npause\n");
	return BatchFile;
}
