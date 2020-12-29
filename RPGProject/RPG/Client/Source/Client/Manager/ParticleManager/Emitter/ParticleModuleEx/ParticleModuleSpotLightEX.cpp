// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleModuleSpotLightEX.h"
// UE4
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"




UParticleModuleBaseEX::UParticleModuleBaseEX(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
UParticleModuleSpotLightEX::UParticleModuleSpotLightEX(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bSpawnModule = true;
	bUpdateModule = true;
	bCurvesAsColor = true;
	bUseInverseSquaredFalloff = true;
	SpawnFraction = 1;
	bSupported3DDrawMode = true;
	b3DDrawMode = true;
	// Particle lights don't affect volumetric fog by default, since they are likely to be moving fast and volumetric fog relies on a strong temporal filter
	VolumetricScatteringIntensity = 0.0f;
}

void UParticleModuleSpotLightEX::InitializeDefaults()
{
	if (!ColorScaleOverLife.IsCreated())
	{
		ColorScaleOverLife.Distribution = NewObject<UDistributionVectorConstant>(this, TEXT("DistributionColorScaleOverLife"));
	}

	if (!BrightnessOverLife.IsCreated())
	{
		BrightnessOverLife.Distribution = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionBrightnessOverLife"));
	}

	if (!RadiusScale.IsCreated())
	{
		RadiusScale.Distribution = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionRadiusScale"));
	}

	if (!LightExponent.IsCreated())
	{
		LightExponent.Distribution = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionLightExponent"));
	}

	if (!InnerConeAngle.IsCreated())
	{
		InnerConeAngle.Distribution = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionInnerConeAngle"));
	}

	if (!OuterConeAngle.IsCreated())
	{
		OuterConeAngle.Distribution = NewObject<UDistributionFloatConstant>(this, TEXT("DistributionOuterConeAngle"));
	}

	if (!bHighQualityLights)
	{
		bShadowCastingLights = false;
	}
}

void UParticleModuleSpotLightEX::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		InitializeDefaults();
	}
}

#if WITH_EDITOR
void UParticleModuleSpotLightEX::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	InitializeDefaults();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

bool UParticleModuleSpotLightEX::CanTickInAnyThread()
{
	return !bHighQualityLights && BrightnessOverLife.OkForParallel() && ColorScaleOverLife.OkForParallel() && RadiusScale.OkForParallel() && LightExponent.OkForParallel();
}

static TAutoConsoleVariable<int32> CVarParticleLightQuality(
	TEXT("r.ParticleLightQuality"),
	2,
	TEXT("0: No lights. 1:Only simple lights. 2:Simple+HQ lights"),
	ECVF_Scalability
);

void UParticleModuleSpotLightEX::SpawnEx(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, struct FRandomStream* InRandomStream, FBaseParticle* ParticleBase)
{
	int32 ParticleLightQuality = CVarParticleLightQuality.GetValueOnAnyThread();
	if (ParticleLightQuality > 0)
	{
		SPAWN_INIT;
		PARTICLE_ELEMENT(FSpotLightParticlePayload, LightData);
		const float Brightness = BrightnessOverLife.GetValue(Particle.RelativeTime, Owner->Component, InRandomStream);
		LightData.ColorScale = ColorScaleOverLife.GetValue(Particle.RelativeTime, Owner->Component, 0, InRandomStream) * Brightness;
		LightData.RadiusScale = RadiusScale.GetValue(Owner->EmitterTime, Owner->Component, InRandomStream);
		// Exponent of 0 is interpreted by renderer as inverse squared falloff
		LightData.LightExponent = bUseInverseSquaredFalloff ? 0 : LightExponent.GetValue(Owner->EmitterTime, Owner->Component, InRandomStream);
		LightData.InnerConeAngle = InnerConeAngle.GetValue(Particle.RelativeTime, Owner->Component, InRandomStream);
		LightData.OuterConeAngle = OuterConeAngle.GetValue(Particle.RelativeTime, Owner->Component, InRandomStream);
		const float RandomNumber = InRandomStream ? InRandomStream->GetFraction() : FMath::SRand();
		LightData.bValid = RandomNumber < SpawnFraction;
		LightData.bAffectsTranslucency = bAffectsTranslucency;
		LightData.bHighQuality = bHighQualityLights;
		LightData.LightId = 0;

		if (bHighQualityLights && ParticleLightQuality > 1)
		{
			LightData.LightId = SpawnHQLight(LightData, Particle, Owner);
		}
	}
}

uint64 UParticleModuleSpotLightEX::SpawnHQLight(const FSpotLightParticlePayload& Payload, const FBaseParticle& Particle, FParticleEmitterInstance* Owner)
{
	uint64 LightId = 0;
	if (!Owner)
	{
		return 0;
	}
	UParticleSystemComponent* ParticleSystem = Owner->Component;
	if (!ParticleSystem)
	{
		return 0;
	}
	AActor* HQLightContainer = ParticleSystem->GetOwner();
	if (!HQLightContainer || HQLightContainer->IsPendingKillPending())
	{
		return 0;
	}

	// Construct the new component and attach as needed				
	USpotLightComponent* LightComponent = NewObject<USpotLightComponent>(HQLightContainer, NAME_None, RF_Transient);
	if (LightComponent)
	{
		LightId = (uint64)LightComponent;

		USceneComponent* RootComponent = HQLightContainer->GetRootComponent();
		USceneComponent* AttachParent = ParticleSystem->GetAttachParent();
		if (AttachParent)
		{
			LightComponent->SetupAttachment(AttachParent, ParticleSystem->GetAttachSocketName());
		}
		else if (RootComponent)
		{
			LightComponent->SetupAttachment(RootComponent);
		}
		// spotlight方向旋转向下
		LightComponent->SetRelativeRotation(FRotator(-90, 0, 0));
		LightComponent->CreationMethod = ParticleSystem->CreationMethod;
		LightComponent->LightingChannels = LightingChannels;
		LightComponent->bUseInverseSquaredFalloff = bUseInverseSquaredFalloff;
		LightComponent->bAffectTranslucentLighting = bAffectsTranslucency;
		LightComponent->VolumetricScatteringIntensity = VolumetricScatteringIntensity;
		LightComponent->SetCastShadows(bShadowCastingLights);

		LightComponent->RegisterComponent();
		Owner->HighQualityLights.Add(LightComponent);

		int32 ScreenAlignment;
		FVector ComponentScale;
		Owner->GetScreenAlignmentAndScale(ScreenAlignment, ComponentScale);
		UpdateHQLight(LightComponent, Payload, Particle, ScreenAlignment, ComponentScale, Owner->UseLocalSpace(), nullptr, false);
	}
	return LightId;
}

void UParticleModuleSpotLightEX::UpdateHQLight(USpotLightComponent* LightComponent, const FSpotLightParticlePayload& Payload, const FBaseParticle& Particle, int32 ScreenAlignment, FVector ComponentScale, bool bLocalSpace, FSceneInterface* OwnerScene, bool bDoRTUpdate)
{
	if (bLocalSpace)
	{
		LightComponent->SetRelativeLocation(Particle.Location);
	}
	else
	{
		LightComponent->SetWorldLocation(Particle.Location);
	}

	FLinearColor DesiredFinalColor = FVector(Particle.Color) * Particle.Color.A * Payload.ColorScale;
	if (bUseInverseSquaredFalloff)
	{
		// For compatibility reasons, the default units are ELightUnits::Unitless. If this change, this needs to be updated.
		ensure(LightComponent->IntensityUnits == ELightUnits::Unitless);

		// Non HQ lights are drawn in 0.0001 Candelas units according to the lighting in DeferredLightPixelShaders.usf
		// the 1/100^2 factor comes from the radial attenuation being computed in cm instead of in meters.
		static const float ShaderUnitConversion = UPointLightComponent::GetUnitsConversionFactor(ELightUnits::Candelas, ELightUnits::Unitless) / (100.f * 100.f);
		DesiredFinalColor *= ShaderUnitConversion;
	}

	//light color on HQ lights is just a uint32 and our light scalars can be huge.  To preserve the color control and range from the particles we need to normalize
	//around the full range multiplied value, and set the scalar intensity such that it will bring things back into line later.
	FVector AdjustedColor(DesiredFinalColor.R, DesiredFinalColor.G, DesiredFinalColor.B);
	float Intensity = AdjustedColor.Size();
	AdjustedColor.Normalize();

	//light module currently needs to run AFTER any size modification modules to get a value that matches 'simple' lights.
	FVector2D Size;
	Size.X = FMath::Abs(Particle.Size.X * ComponentScale.X);
	Size.Y = FMath::Abs(Particle.Size.Y * ComponentScale.Y);
	if (ScreenAlignment == PSA_Square || ScreenAlignment == PSA_FacingCameraPosition || ScreenAlignment == PSA_FacingCameraDistanceBlend)
	{
		Size.Y = Size.X;
	}
	float Radius = Payload.RadiusScale * (Size.X + Size.X) * 0.5f;

	//HQ light color is an FColor which is a uint32.  Thus we have to break out the out of range value into the intensity scalar.
	FColor NormalizedColor;
	NormalizedColor.R = (uint8)FMath::Min(AdjustedColor.X * 255.0f, 255.0f);
	NormalizedColor.G = (uint8)FMath::Min(AdjustedColor.Y * 255.0f, 255.0f);
	NormalizedColor.B = (uint8)FMath::Min(AdjustedColor.Z * 255.0f, 255.0f);
	LightComponent->SetIntensity(Intensity);
	LightComponent->SetLightColor(NormalizedColor);
	LightComponent->SetAttenuationRadius(Radius);
	LightComponent->SetLightFalloffExponent(Payload.LightExponent);

	float inner = Payload.InnerConeAngle;
	float outer = Payload.OuterConeAngle;
	LightComponent->SetInnerConeAngle(inner);
	LightComponent->SetOuterConeAngle(outer);

	if (OwnerScene && bDoRTUpdate)
	{
		OwnerScene->UpdateLightTransform(LightComponent);
		OwnerScene->UpdateLightColorAndBrightness(LightComponent);
	}
}

void UParticleModuleSpotLightEX::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase)
{
	SpawnEx(Owner, Offset, SpawnTime, NULL, ParticleBase);
}

void UParticleModuleSpotLightEX::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{
	if ((Owner == NULL) || (Owner->ActiveParticles <= 0) ||
		(Owner->ParticleData == NULL) || (Owner->ParticleIndices == NULL))
	{
		return;
	}

	UWorld* OwnerWorld = Owner->GetWorld();
	FSceneInterface* OwnerScene = nullptr;
	if (OwnerWorld)
	{
		OwnerScene = OwnerWorld->Scene;
	}

	TSet<uint64> ActiveLights;
	UParticleLODLevel* LODLevel = Owner->SpriteTemplate->GetCurrentLODLevel(Owner);
	check(LODLevel);
	FPlatformMisc::Prefetch(Owner->ParticleData, (Owner->ParticleIndices[0] * Owner->ParticleStride));
	FPlatformMisc::Prefetch(Owner->ParticleData, (Owner->ParticleIndices[0] * Owner->ParticleStride) + PLATFORM_CACHE_LINE_SIZE);
	const bool bUseLocalSpace = Owner->UseLocalSpace();
	int32 ScreenAlignment;
	FVector ComponentScale;
	Owner->GetScreenAlignmentAndScale(ScreenAlignment, ComponentScale);

	BEGIN_UPDATE_LOOP;
	{
		PARTICLE_ELEMENT(FSpotLightParticlePayload, Data);
		const float Brightness = BrightnessOverLife.GetValue(Particle.RelativeTime, Owner->Component);
		Data.ColorScale = ColorScaleOverLife.GetValue(Particle.RelativeTime, Owner->Component) * Brightness;

		if (bHighQualityLights && (Data.LightId != 0))
		{
			ActiveLights.Add(Data.LightId);

			//for now we can do this
			USpotLightComponent* PointLightComponent = (USpotLightComponent*)Data.LightId;
			UpdateHQLight(PointLightComponent, Data, Particle, ScreenAlignment, ComponentScale, bUseLocalSpace, OwnerScene, true);
		}
	}
	END_UPDATE_LOOP;

	//remove any dead lights.
	if (bHighQualityLights)
	{
		for (int32 i = 0; i < Owner->HighQualityLights.Num(); ++i)
		{
			UPointLightComponent* PointLightComponent = Owner->HighQualityLights[i];
			if (PointLightComponent && !ActiveLights.Contains((uint64)PointLightComponent))
			{
				PointLightComponent->Modify();
				PointLightComponent->DestroyComponent(false);
				Owner->HighQualityLights.RemoveAtSwap(i);
				--i;
			}
		}
	}
}

uint32 UParticleModuleSpotLightEX::RequiredBytes(UParticleModuleTypeDataBase* TypeData)
{
	return sizeof(FLightParticlePayload);
}

void UParticleModuleSpotLightEX::SetToSensibleDefaults(UParticleEmitter* Owner)
{
	UDistributionVectorConstant* ColorScaleDist = Cast<UDistributionVectorConstant>(ColorScaleOverLife.Distribution);
	if (ColorScaleDist)
	{
		ColorScaleDist->Constant = FVector(1, 1, 1);
		ColorScaleDist->bIsDirty = true;
	}

	UDistributionFloatConstant* BrightnessDist = Cast<UDistributionFloatConstant>(BrightnessOverLife.Distribution);
	if (BrightnessDist)
	{
		BrightnessDist->Constant = 32.0f;
		BrightnessDist->bIsDirty = true;
	}


	UDistributionFloatConstant* RadiusScaleDist = Cast<UDistributionFloatConstant>(RadiusScale.Distribution);
	if (RadiusScaleDist)
	{
		RadiusScaleDist->Constant = 15.0f;
		RadiusScaleDist->bIsDirty = true;
	}

	UDistributionFloatConstant* LightExponentDist = Cast<UDistributionFloatConstant>(LightExponent.Distribution);
	if (LightExponentDist)
	{
		LightExponentDist->Constant = 16.0f;
		LightExponentDist->bIsDirty = true;
	}
}

void UParticleModuleSpotLightEX::Render3DPreview(FParticleEmitterInstance* Owner, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
#if WITH_EDITOR
	if ((Owner == NULL) || (Owner->ActiveParticles <= 0) ||
		(Owner->ParticleData == NULL) || (Owner->ParticleIndices == NULL))
	{
		return;
	}

	if (bPreviewLightRadius)
	{
		check(IsInGameThread());
		int32 Offset = 0;
		UParticleLODLevel* LODLevel = Owner->SpriteTemplate->GetCurrentLODLevel(Owner);
		const bool bLocalSpace = LODLevel->RequiredModule->bUseLocalSpace;
		const FVector Scale = Owner->Component->GetComponentTransform().GetScale3D();
		const FMatrix LocalToWorld = Owner->EmitterToSimulation * Owner->SimulationToWorld;
		check(LODLevel);

		const uint8* ParticleData = Owner->ParticleData;
		const uint16* ParticleIndices = Owner->ParticleIndices;

		for (int32 i = 0; i < Owner->ActiveParticles; i++)
		{
			DECLARE_PARTICLE_CONST(Particle, ParticleData + Owner->ParticleStride * ParticleIndices[i]);

			const FLightParticlePayload* LightPayload = (const FLightParticlePayload*)(((const uint8*)&Particle) + Owner->LightDataOffset);

			if (LightPayload->bValid)
			{
				const FVector LightPosition = bLocalSpace ? FVector(LocalToWorld.TransformPosition(Particle.Location)) : Particle.Location;
				const FVector Size = Scale * Particle.Size;
				const float LightRadius = LightPayload->RadiusScale * (Size.X + Size.Y) / 2.0f;

				DrawWireSphere(PDI, LightPosition, FColor::White, LightRadius, 18, SDPG_World);
			}
		}
	}

#endif	//#if WITH_EDITOR
}
