#include "ParticleModuleDecal.h"
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ParticleModuleTypeDataDecal.h"
#include "ParticleDecalEmitterInstances.h"

UParticleModuleDecal::UParticleModuleDecal(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
	bSpawnModule = true;
	bUpdateModule = true;
}
					
#if WITH_EDITOR
void UParticleModuleDecal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
}
#endif 
// WITH_EDITOR

void UParticleModuleDecal::PostInitProperties()
{
	Super::PostInitProperties();
}

void UParticleModuleDecal::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase)
{
	//发射器是DecalEmitterInstance才spawn
	if (!IsDecalEmitterInstance(Owner))
		return;
	// 光效希望看见这个贴花的大小，不强制显示模型的情况下，PIE和Game才生成
	if (!VisibleDecal)
	{
		// world类型是PIE和Game的时候spawn，否则可能会出现一个多余的静态模型
		UWorld* world = Owner->GetWorld();
		check(world);
		if (!(EWorldType::Game == world->WorldType || EWorldType::PIE == world->WorldType))
			return;
	}
	SpawnEx(Owner, Offset, SpawnTime, NULL, ParticleBase);
}

uint32 UParticleModuleDecal::RequiredBytes(UParticleModuleTypeDataBase* TypeData)
{
	return sizeof(FDecalParticlePayload);
}

void UParticleModuleDecal::SpawnEx(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, struct FRandomStream* InRandomStream, FBaseParticle* ParticleBase)
{
	SPAWN_INIT;
	PARTICLE_ELEMENT(FDecalParticlePayload, ParData);
	ParData.DecalId = SpawnDecal(ParData, Particle, Owner);
}

uint64 UParticleModuleDecal::SpawnDecal(const FDecalParticlePayload& Payload, FBaseParticle& Particle, FParticleEmitterInstance* Owner)
{
	uint64 Id = 0;
	if (!Owner)
	{
		return 0;
	}
	UParticleSystemComponent* ParticleSystem = Owner->Component;
	if (!ParticleSystem)
	{
		return 0;
	}
	AActor* Container = ParticleSystem->GetOwner();

	if (!Container || Container->IsPendingKillPending())
	{
		return 0;
	}
	// Construct the new component and attach as needed		
	UDecalComponent* decalComponent = NewObject<UDecalComponent>(Container, UDecalComponent::StaticClass());

	if (decalComponent)
	{
		Id = (uint64)decalComponent;
		USceneComponent* RootComponent = Container->GetRootComponent();
		USceneComponent* AttachParent = ParticleSystem->GetAttachParent();
		if (AttachParent)
		{
			decalComponent->SetupAttachment(AttachParent, ParticleSystem->GetAttachSocketName());
		}
		else if (RootComponent)
		{
			decalComponent->SetupAttachment(RootComponent);
		}
		else
		{
			decalComponent->SetupAttachment(ParticleSystem);
		}
		decalComponent->RegisterComponent();
		decalComponent->SetVisibility(false);

		FParticleDecalEmitterInstance* Inst = static_cast<FParticleDecalEmitterInstance*>(Owner);
		if (Inst == nullptr)
			return 0;
		Inst->Decals.Add(decalComponent);

		// 指定材质
		if (decalComponent->GetNumMaterials() > 0)
		{
			decalComponent->SetMaterial(0, Inst->CurrentMaterial);
			dyMaterial = decalComponent->CreateDynamicMaterialInstance();
		}
	}
	return Id;
}

void UParticleModuleDecal::UpdateDecal(UDecalComponent* decalComponent, const FBaseParticle& Particle, bool bLocalSpace)
{
	if (bLocalSpace)
	{
		decalComponent->SetRelativeLocation(Particle.Location);
		decalComponent->SetRelativeRotation(FRotator(-90, 0, Particle.Rotation));
		decalComponent->SetRelativeScale3D(Particle.Size);
	}
	else
	{
		decalComponent->SetWorldLocation(Particle.Location);
		decalComponent->SetWorldRotation(FRotator(-90, 0, Particle.Rotation));
		decalComponent->SetWorldScale3D(Particle.Size);
	}
	if (!decalComponent->IsVisible())
		decalComponent->SetVisibility(true);
}

void UParticleModuleDecal::UpdateMaterial(const FBaseParticle& Particle)
{
	//Particle.Color
	dyMaterial->SetVectorParameterValue(TEXT("ColorParam"), Particle.Color);
}

bool UParticleModuleDecal::IsDecalEmitterInstance(FParticleEmitterInstance* Owner)
{
	check(Owner);
	UParticleLODLevel* LODLevel = Owner->SpriteTemplate->GetLODLevel(0);
	check(LODLevel);
	UParticleModuleTypeDataDecal* DecalTypeData = Cast<UParticleModuleTypeDataDecal>(LODLevel->TypeDataModule);
	return DecalTypeData != nullptr;
}

void UParticleModuleDecal::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
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
	if (!IsDecalEmitterInstance(Owner))
		return;

	FParticleDecalEmitterInstance* Inst = static_cast<FParticleDecalEmitterInstance*>(Owner);
	if (Inst == nullptr)
		return;
	FPlatformMisc::Prefetch(Owner->ParticleData, (Owner->ParticleIndices[0] * Owner->ParticleStride));
	FPlatformMisc::Prefetch(Owner->ParticleData, (Owner->ParticleIndices[0] * Owner->ParticleStride) + PLATFORM_CACHE_LINE_SIZE);
	const bool bUseLocalSpace = Owner->UseLocalSpace();

	BEGIN_UPDATE_LOOP;
	{
		PARTICLE_ELEMENT(FDecalParticlePayload, Data);
		// 		const float Brightness = BrightnessOverLife.GetValue(Particle.RelativeTime, Owner->Component);
		// 		Data.ColorScale = ColorScaleOverLife.GetValue(Particle.RelativeTime, Owner->Component) * Brightness;
		//if (bHighQualityLights && (Data.MeshId != 0))
		if (Data.DecalId != 0)
		{
			// 			FString DebugString = FString::Printf(TEXT("ActiveMeshes :(%d) RelativeTime:(%f)"),	Data.MeshId, Particle.RelativeTime);
			// 			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Red, DebugString, false);
			//for now we can do this
			UDecalComponent* decalComponent = (UDecalComponent*)Data.DecalId;
			UpdateDecal(decalComponent, Particle, bUseLocalSpace);
			UpdateMaterial( Particle);
		}
	}
	END_UPDATE_LOOP;	
}
