// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleSkeletalMeshEmitterInstances.h"
// UE4
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleHelper.h"
#include "Kismet/KismetSystemLibrary.h"



DECLARE_CYCLE_STAT(TEXT("MeshEmitterInstance InitParams GT"), STAT_skMeshEmitterInstance_InitParameters, STATGROUP_Particles);

DECLARE_CYCLE_STAT_EXTERN(TEXT("Update Bounds Time GT"), STAT_ParticleUpdateBounds2, STATGROUP_Particles, );
DEFINE_STAT(STAT_ParticleUpdateBounds2);

/** Constructor	*/
FParticleSkeletalMeshEmitterInstance::FParticleSkeletalMeshEmitterInstance() :
	FParticleMeshEmitterInstance(),
	SkMeshTypeData(NULL),
	SkeletalMeshOffset(sizeof(FBaseParticle))

{
}

FParticleSkeletalMeshEmitterInstance::~FParticleSkeletalMeshEmitterInstance()
{
	for (int32 i = 0; i < Meshes.Num(); ++i)
	{
		USkeletalMeshComponent* MeshComponent = Meshes[i];
		{
			MeshComponent->Modify();
			MeshComponent->DestroyComponent(false);
		}
	}
	Meshes.Reset();
}

void FParticleSkeletalMeshEmitterInstance::InitParameters(UParticleEmitter* InTemplate, UParticleSystemComponent* InComponent)
{
	SCOPE_CYCLE_COUNTER(STAT_skMeshEmitterInstance_InitParameters);

	FParticleEmitterInstance::InitParameters(InTemplate, InComponent);

	// Get the type data module
	UParticleLODLevel* LODLevel = InTemplate->GetLODLevel(0);
	check(LODLevel);
	SkMeshTypeData = CastChecked<UParticleModuleTypeDataSkeletalMesh>(LODLevel->TypeDataModule);
	check(SkMeshTypeData);
	
	// Grab cached mesh rotation flag from ParticleEmitter template
	MeshRotationActive = InTemplate->bMeshRotationActive;
}

// void FParticleSkeletalMeshEmitterInstance::Tick(float DeltaTime, bool bSuppressSpawning)
// {
// 	for (int32 i = 0; i < SkMeshes.Num(); ++i)
// 	{
// 		USkeletalMeshComponent* MeshComponent = SkMeshes[i].MeshComponent;
// 		if (MeshComponent && Particle.RelativeTime > 1.0f/*MeshComponent->lifetimer > MeshComponent->lifetime*/)
// 		{
// 			MeshComponent->Modify();
// 			MeshComponent->DestroyComponent(false);
// 			Meshes.RemoveAtSwap(i);
// 			--i;
// 		}
// 	}
// 	FParticleMeshEmitterInstance::Tick(DeltaTime, bSuppressSpawning);
// }

void FParticleSkeletalMeshEmitterInstance::UpdateBoundingBox(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_ParticleUpdateBounds2);
	//@todo. Implement proper bound determination for mesh emitters.
	// Currently, just 'forcing' the mesh size to be taken into account.
	if ((Component != NULL) && (ActiveParticles > 0))
	{
		bool bUpdateBox = ((Component->bWarmingUp == false) &&
			(Component->Template != NULL) && (Component->Template->bUseFixedRelativeBoundingBox == false));

		// Take scale into account
		FVector Scale = Component->GetComponentTransform().GetScale3D();

		// Get the static mesh bounds
		FBoxSphereBounds MeshBound;
		if (Component->bWarmingUp == false)
		{
			if (SkMeshTypeData->Mesh)
			{
				MeshBound = SkMeshTypeData->Mesh->GetBounds();
			}
			else
			{
				//UE_LOG(LogParticles, Log, TEXT("MeshEmitter with no mesh set?? - %s"), Component->Template ? *(Component->Template->GetPathName()) : TEXT("??????"));
				MeshBound = FBoxSphereBounds(FVector(0, 0, 0), FVector(0, 0, 0), 0);
			}
		}
		else
		{
			// This isn't used anywhere if the bWarmingUp flag is false, but GCC doesn't like it not touched.
			FMemory::Memzero(&MeshBound, sizeof(FBoxSphereBounds));
		}

		UParticleLODLevel* LODLevel = GetCurrentLODLevelChecked();

		const bool bUseLocalSpace = LODLevel->RequiredModule->bUseLocalSpace;

		const FMatrix ComponentToWorld = bUseLocalSpace
			? Component->GetComponentToWorld().ToMatrixWithScale()
			: FMatrix::Identity;

		FVector	NewLocation;
		float	NewRotation;
		if (bUpdateBox)
		{
			ParticleBoundingBox.Init();
		}

		// For each particle, offset the box appropriately 
		FVector MinVal(HALF_WORLD_MAX);
		FVector MaxVal(-HALF_WORLD_MAX);

		FPlatformMisc::Prefetch(ParticleData, ParticleStride * ParticleIndices[0]);
		FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[0] * ParticleStride) + PLATFORM_CACHE_LINE_SIZE);

		bool bSkipDoubleSpawnUpdate = !SpriteTemplate->bUseLegacySpawningBehavior;
		for (int32 i = 0; i < ActiveParticles; i++)
		{
			DECLARE_PARTICLE(Particle, ParticleData + ParticleStride * ParticleIndices[i]);
			FPlatformMisc::Prefetch(ParticleData, ParticleStride * ParticleIndices[i + 1]);
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride) + PLATFORM_CACHE_LINE_SIZE);

			// Do linear integrator and update bounding box
			Particle.OldLocation = Particle.Location;

			bool bJustSpawned = (Particle.Flags & STATE_Particle_JustSpawned) != 0;
			Particle.Flags &= ~STATE_Particle_JustSpawned;

			//Don't update position for newly spawned particles. They already have a partial update applied during spawn.
			bool bSkipUpdate = bJustSpawned && bSkipDoubleSpawnUpdate;

			if ((Particle.Flags & STATE_Particle_Freeze) == 0 && !bSkipUpdate)
			{
				if ((Particle.Flags & STATE_Particle_FreezeTranslation) == 0)
				{
					NewLocation = Particle.Location + DeltaTime * Particle.Velocity;
				}
				else
				{
					NewLocation = Particle.Location;
				}
				if ((Particle.Flags & STATE_Particle_FreezeRotation) == 0)
				{
					NewRotation = Particle.Rotation + DeltaTime * Particle.RotationRate;
				}
				else
				{
					NewRotation = Particle.Rotation;
				}
			}
			else
			{
				// Don't move it...
				NewLocation = Particle.Location;
				NewRotation = Particle.Rotation;
			}

			FVector LocalExtent = MeshBound.GetBox().GetExtent() * Particle.Size * Scale;

			NewLocation += PositionOffsetThisTick;
			Particle.OldLocation += PositionOffsetThisTick;

			// Do angular integrator, and wrap result to within +/- 2 PI
			Particle.Rotation = FMath::Fmod(NewRotation, 2.f*(float)PI);
			Particle.Location = NewLocation;

			if (bUpdateBox)
			{
				FVector PositionForBounds = NewLocation;

				if (bUseLocalSpace)
				{
					// Note: building the bounding box in world space as that gives tighter bounds than transforming a local space AABB into world space
					PositionForBounds = ComponentToWorld.TransformPosition(NewLocation);
				}

				MinVal[0] = FMath::Min<float>(MinVal[0], PositionForBounds.X - LocalExtent.X);
				MaxVal[0] = FMath::Max<float>(MaxVal[0], PositionForBounds.X + LocalExtent.X);
				MinVal[1] = FMath::Min<float>(MinVal[1], PositionForBounds.Y - LocalExtent.Y);
				MaxVal[1] = FMath::Max<float>(MaxVal[1], PositionForBounds.Y + LocalExtent.Y);
				MinVal[2] = FMath::Min<float>(MinVal[2], PositionForBounds.Z - LocalExtent.Z);
				MaxVal[2] = FMath::Max<float>(MaxVal[2], PositionForBounds.Z + LocalExtent.Z);
			}
		}

		if (bUpdateBox)
		{
			ParticleBoundingBox = FBox(MinVal, MaxVal);
		}
	}
}
// 
// uint32 FParticleSkeletalMeshEmitterInstance::RequiredBytes()
// {
// 	uint32 uiBytes = FParticleEmitterInstance::RequiredBytes();//FParticleMeshEmitterInstance::RequiredBytes();
// // 
// // 	MeshRotationOffset = PayloadOffset + uiBytes;
// // 	uiBytes += sizeof(FMeshRotationPayloadData);
// 
// 	if (SkMeshTypeData)
// 	{
// // 		const auto* MeshTD = static_cast<const UParticleModuleTypeDataSkeletalMesh*>(SkMeshTypeData);
// // 		if (SkMeshTypeData->IsMotionBlurEnabled())
// // 		{
// // 			MeshMotionBlurOffset = PayloadOffset + uiBytes;
// // 			uiBytes += sizeof(FMeshMotionBlurPayloadData);
// // 		}
// 
// 		SkeletalMeshOffset = PayloadOffset + uiBytes;
// 	}
// 
// 	return uiBytes;
// }

void FParticleSkeletalMeshEmitterInstance::PostSpawn(FBaseParticle* Particle, float InterpolationPercentage, float SpawnTime)
{
	FParticleEmitterInstance::PostSpawn(Particle, InterpolationPercentage, SpawnTime);
	UParticleLODLevel* LODLevel = GetCurrentLODLevelChecked();
// 
}

bool FParticleSkeletalMeshEmitterInstance::IsDynamicDataRequired(UParticleLODLevel* InCurrentLODLevel)
{
	return false;
// 		SkMeshTypeData->Mesh != NULL
// 		&& SkMeshTypeData->Mesh->HasValidRenderData(false)
// 		&& FParticleEmitterInstance::IsDynamicDataRequired(InCurrentLODLevel);
}

void FParticleSkeletalMeshEmitterInstance::KillParticles()
{
	// 粒子生命时间到了，先将meshes里的模型处理掉
	if (ActiveParticles > 0)
	{
		// Loop over the active particles... If their RelativeTime is > 1.0f (indicating they are dead),
		// move them to the 'end' of the active particle list.
		for (int32 i = ActiveParticles - 1; i >= 0; i--)
		{
			const int32	CurrentIndex = ParticleIndices[i];
			const uint8* ParticleBase = ParticleData + CurrentIndex * ParticleStride;
			FBaseParticle& Particle = *((FBaseParticle*)ParticleBase);


			// 		const float Brightness = BrightnessOverLife.GetValue(Particle.RelativeTime, Owner->Component);
			// 		Data.ColorScale = ColorScaleOverLife.GetValue(Particle.RelativeTime, Owner->Component) * Brightness;

			if (Particle.RelativeTime > 1.0f)
			{
				//DECLARE_PARTICLE(Particle, ParticleData + ParticleStride * ParticleIndices[i]);
				FSkMeshParticlePayload* PayloadData = (FSkMeshParticlePayload*)((uint8*)&Particle + SkeletalMeshOffset);
				for (int32 j = 0; j < Meshes.Num(); ++j)
				{
					USkeletalMeshComponent* MeshComponent = Meshes[j];
						
					if (MeshComponent && (uint64)MeshComponent == PayloadData->MeshId)
					{
						//MeshComponent->SetVisibility(false);
						MeshComponent->Modify();
						MeshComponent->DestroyComponent(false);
						Meshes.RemoveAtSwap(j);
						--j;
						break;
					}
				}
			}
		}
	}
	FParticleEmitterInstance::KillParticles();

}

void FParticleSkeletalMeshEmitterInstance::KillParticle(int32 Index)
{
	int32 KillIndex = ParticleIndices[Index];

	// Handle the kill event, if needed

	const uint8* ParticleBase = ParticleData + KillIndex * ParticleStride;
	FBaseParticle& Particle = *((FBaseParticle*)ParticleBase);

	//DECLARE_PARTICLE(Particle, ParticleData + ParticleStride * ParticleIndices[Index]);
	FSkMeshParticlePayload* PayloadData = (FSkMeshParticlePayload*)((uint8*)&Particle + SkeletalMeshOffset);
	for (int32 j = 0; j < Meshes.Num(); ++j)
	{
		USkeletalMeshComponent* MeshComponent = Meshes[j];

		if (MeshComponent && (uint64)MeshComponent == PayloadData->MeshId)
		{
			//MeshComponent->SetVisibility(false);
			MeshComponent->Modify();
			MeshComponent->DestroyComponent(false);
			Meshes.RemoveAtSwap(j);
			--j;
			break;
		}
	}
	FParticleEmitterInstance::KillParticle(Index);
}

void FParticleSkeletalMeshEmitterInstance::KillParticlesForced(bool bFireEvents /*= false*/)
{
	for (int32 j = 0; j < Meshes.Num(); ++j)
	{
		USkeletalMeshComponent* MeshComponent = Meshes[j];
		if (MeshComponent )
		{
			//MeshComponent->SetVisibility(false);
			MeshComponent->Modify();
			MeshComponent->DestroyComponent(false);
			Meshes.RemoveAtSwap(j);
			--j;
			break;
		}
	}
	FParticleEmitterInstance::KillParticlesForced(bFireEvents);
}

// float FParticleSkeletalMeshEmitterInstance::Spawn(float DeltaTime)
// {
// 
// }

