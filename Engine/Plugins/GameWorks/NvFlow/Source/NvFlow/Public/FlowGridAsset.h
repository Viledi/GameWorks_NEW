
#pragma once

// NvFlow begin

#include "GridInteractionNvFlow.h"
#include "FlowGridAsset.generated.h"

UENUM()
enum EFlowGridDimension
{
	EFGD_256 = 8,
	EFGD_512 = 9,
	EFGD_1024 = 10,
};

UCLASS(BlueprintType, hidecategories=object, MinimalAPI)
class UFlowGridAsset : public UObject
{
	GENERATED_UCLASS_BODY()

	/** Grid cell size: defines resolution of simulation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		GridCellSize;

	/** Grid dimension: dimension * cellSize defines size of simulation domain*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	TEnumAsByte<EFlowGridDimension> VirtualGridDimension;

	/** Allows increase of maximum number of cells*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		MemoryLimitScale;

	/** Simulation update rate in updates per second*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		SimulationRate;

	/** If true, multiAdapter is used if supported*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	uint32		bMultiAdapterEnabled : 1;

	/** If true, grid affects GPU particles*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	uint32		bEnableParticlesInteraction : 1;

	/** Enum indicating what interaction channel this object has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	TEnumAsByte<enum EInteractionChannelNvFlow> InteractionChannel;

	/** Custom Channels for Responses */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	struct FInteractionResponseContainerNvFlow ResponseToInteractionChannels;

	/** If true, higher res density and volume rendering are disabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	uint32		bEnableParticleMode : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		ParticleToGridAccelTimeConstant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		ParticleToGridDecelTimeConstant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		ParticleToGridThresholdMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		GridToParticleAccelTimeConstant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		GridToParticleDecelTimeConstant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	float		GridToParticleThresholdMultiplier;

	//* Relative importance of velocity for allocation, 0.0 means not important*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	VelocityWeight;

	//* Relative importance of density for allocation, 0.0 means not important*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	DensityWeight;

	//* Relative importance of temperature for allocation, 0.0 means not important*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	TempWeight;

	//* Relative importance of fuel for allocation, 0.0 means not important*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	FuelWeight;

	//* Minimum velocity magnitude that is considered relevant*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	VelocityThreshold;

	//* Minimum density magnitude that is considered relevant*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	DensityThreshold;

	//* Minimum temperature magnitude that is considered relevant*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	TempThreshold;

	//* Minimum fuel magnitude that is considered relevant*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	FuelThreshold;

	//* Global importance threshold, to scale quality/performance*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block Allocation", meta = (ClampMin = 0.0f, UIMin = 0.f, UIMax = 1.0f))
	float	ImportanceThreshold;

	/** Gravity vector for use by buoyancy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	FVector		Gravity;

	/** Higher values reduce velocity faster (exponential decay curve)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damping", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		VelocityDamping;

	/** Higher values reduce density faster (exponential decay curve)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damping", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		DensityDamping;

	/** Fade velocity rate in units / sec*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		VelocityFade;

	/** Fade density rate in units / sec*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		DensityFade;

	/** Higher values make a sharper appearance, but with more artifacts*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MacCormack Correction", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		VelocityMacCormackBlendFactor;

	/** Higher values make a sharper appearance, but with more artifacts*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MacCormack Correction", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		DensityMacCormackBlendFactor;

	/** Higher values increase rotation, reduce laminar flow*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vorticity", meta = (ClampMin = 0.0f, ClampMax = 20.0f))
	float		VorticityStrength;

	/** 0.f means zero velocity magnitude influence on vorticity*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vorticity", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		VorticityVelocityMask;

	/** If true, run older less accurate pressure solver*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pressure")
	uint32		bPressureLegacyMode : 1;

	/** Minimum temperature for combustion*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combustion", meta = (ClampMin = 0.0f, ClampMax = 5.0f))
	float		IgnitionTemperature;

	/** Cooling rate, exponential*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combustion", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
	float		CoolingRate;

	/** Enum indicating what type of object this should be considered as */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<enum ECollisionChannel> ObjectType;

	/** Custom Channels for Responses*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	struct FCollisionResponseContainer	ResponseToChannels;

	/** Alpha scale*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float		RenderingAlphaScale;

	/** Color curve*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Export, Category = "Rendering")
	class UCurveLinearColor* ColorMap;

	/** Color curve minimum X value*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (UIMin = -1.0f, UIMax = 1.0f))
	float		ColorMapMinX;

	/** Color curve maximum X value*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (UIMin = -1.0f, UIMax = 1.0f))
	float		ColorMapMaxX;

	/** Rendering mode*/
	UPROPERTY(EditAnywhere, Category = "Rendering", meta = (ClampMin = 0, ClampMax = 5))
	uint32		RenderingMode;

	/** Adaptive ScreenPercentage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering")
	uint32		bAdaptiveScreenPercentage : 1;

	/** Target Frame Time for Adaptive, in ms */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (UIMin = 0.f, UIMax = 50.f))
	float		AdaptiveTargetFrameTime;

	/** Maximum ScreenPercentage, Default Value is Adaptive is disabled. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (UIMin = 0.1f, UIMax = 1.0f))
	float		MaxScreenPercentage;

	/** Minimum ScreenPercentage when Adaptive is enabled. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (UIMin = 0.1f, UIMax = 1.0f))
	float		MinScreenPercentage;

	/** Debug rendering*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering")
	uint32		bDebugWireframe : 1;
	static bool sGlobalDebugDraw;
	static uint32 sGlobalRenderingMode;

	// Helper methods

	FORCEINLINE static float GetFlowToUE4Scale() { return 100.0f; }

	FORCEINLINE static uint32 GetVirtualGridDimension(TEnumAsByte<EFlowGridDimension> GridDimension) { return static_cast<uint32>(1 << GridDimension); }

	FORCEINLINE static float GetVirtualGridExtent(float GridCellSize, TEnumAsByte<EFlowGridDimension> GridDimension) 
	{ 
		return GridCellSize * GetVirtualGridDimension(GridDimension) * 0.5f;
	}

	FORCEINLINE float GetVirtualGridDimension() const { return GetVirtualGridDimension(VirtualGridDimension); }
	FORCEINLINE float GetVirtualGridExtent() const { return GetVirtualGridExtent(GridCellSize, VirtualGridDimension); }
};

// NvFlow end