#include "NvFlowPCH.h"

#include "GameWorks/RendererHooksNvFlow.h"
#include "GameWorks/GridAccessHooksNvFlow.h"

IMPLEMENT_MODULE( FNvFlowModule, NvFlow );
DEFINE_LOG_CATEGORY(LogNvFlow);

// NvFlow begin
#if WITH_NVFLOW
void NvFlowUpdateScene(FRHICommandListImmediate& RHICmdList, TArray<FPrimitiveSceneInfo*>& Primitives);
bool NvFlowDoRenderPrimitive(FRHICommandList& RHICmdList, const FViewInfo& View, FPrimitiveSceneInfo* PrimitiveSceneInfo);
void NvFlowDoRenderFinish(FRHICommandListImmediate& RHICmdList, const FViewInfo& View);
uint32 NvFlowQueryGridExportParams(FRHICommandListImmediate& RHICmdList, const FBox& Bounds, uint32 MaxCount, GridExportParamsNvFlow* ResultParamsList);
#endif
// NvFlow end

struct RendererHooksNvFlowImpl : public RendererHooksNvFlow
{
	virtual void NvFlowUpdateScene(FRHICommandListImmediate& RHICmdList, TArray<FPrimitiveSceneInfo*>& Primitives)
	{
		::NvFlowUpdateScene(RHICmdList, Primitives);
	}

	virtual bool NvFlowDoRenderPrimitive(FRHICommandList& RHICmdList, const FViewInfo& View, FPrimitiveSceneInfo* PrimitiveSceneInfo)
	{
		return ::NvFlowDoRenderPrimitive(RHICmdList, View, PrimitiveSceneInfo);
	}

	virtual void NvFlowDoRenderFinish(FRHICommandListImmediate& RHICmdList, const FViewInfo& View)
	{
		::NvFlowDoRenderFinish(RHICmdList, View);
	}
};
RendererHooksNvFlowImpl GRendererHooksNvFlowImpl;

struct GridAccessHooksNvFlowImpl : public GridAccessHooksNvFlow
{
	virtual uint32 NvFlowQueryGridExportParams(FRHICommandListImmediate& RHICmdList, const FBox& Bounds, uint32 MaxCount, GridExportParamsNvFlow* ResultParamsList)
	{
		return ::NvFlowQueryGridExportParams(RHICmdList, Bounds, MaxCount, ResultParamsList);
	}
};
GridAccessHooksNvFlowImpl GGridAccessHooksNvFlowImpl;

struct FNvFlowCommands
{
	FAutoConsoleCommand ConsoleCommandFlowVis;
	FAutoConsoleCommand ConsoleCommandFlowVisMode;

	void CommandFlowVis(const TArray<FString>& Args)
	{
		UFlowGridAsset::sGlobalDebugDraw = !UFlowGridAsset::sGlobalDebugDraw;
	}

	void CommandFlowVisMode(const TArray<FString>& Args)
	{
		uint32 FlowVisMode = FCString::Atoi(*Args[0]);
		UFlowGridAsset::sGlobalDebugDraw = true;
		UFlowGridAsset::sGlobalRenderingMode = FMath::Clamp<uint32>(FlowVisMode, 0, 5);
	}

	FNvFlowCommands() :
		ConsoleCommandFlowVis(
			TEXT("flowvis"),
			*NSLOCTEXT("Flow", "CommandText_FlowVis", "Enable/Disable Flow debug visualization").ToString(),
			FConsoleCommandWithArgsDelegate::CreateRaw(this, &FNvFlowCommands::CommandFlowVis)
			),
		ConsoleCommandFlowVisMode(
			TEXT("flowvismode"),
			*NSLOCTEXT("Flow", "CommandText_FlowVisMode", "Set Flow debug visualization mode").ToString(),
			FConsoleCommandWithArgsDelegate::CreateRaw(this, &FNvFlowCommands::CommandFlowVisMode)
			)
	{
	}
};

void FNvFlowModule::StartupModule()
{
	Commands = MakeUnique<FNvFlowCommands>();

	FlowModule = nullptr;

	FString LibPath;
	FString LibName;

#if PLATFORM_WINDOWS
	#if PLATFORM_64BITS
	LibPath = FPaths::EngineDir() / TEXT("Plugins/GameWorks/NvFlow/Libraries/win64/");
	LibName = TEXT("NvFlowD3D11Release_win64.dll");
	#else
	LibPath = FPaths::EngineDir() / TEXT("Plugins/GameWorks/NvFlow/Libraries/win32/");
	LibName = TEXT("NvFlowD3D11Release_win32.dll");
	#endif
#endif

	FPlatformProcess::PushDllDirectory(*LibPath);
	FlowModule = FPlatformProcess::GetDllHandle(*(LibPath + LibName));
	FPlatformProcess::PopDllDirectory(*LibPath);

	GRendererNvFlowHooks = &GRendererHooksNvFlowImpl;
	GGridAccessNvFlowHooks = &GGridAccessHooksNvFlowImpl;
}

void FNvFlowModule::ShutdownModule()
{
	if (FlowModule)
	{
		FPlatformProcess::FreeDllHandle(FlowModule);
		FlowModule = nullptr;
	}
}