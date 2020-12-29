// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleDecalEmitterInstances.h"
// UE4
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleHelper.h"
#include "Kismet/KismetSystemLibrary.h"


/** Constructor	*/
FParticleDecalEmitterInstance::FParticleDecalEmitterInstance() :
	FParticleEmitterInstance(),
	DecalOffset(sizeof(FBaseParticle))
{
}

FParticleDecalEmitterInstance::~FParticleDecalEmitterInstance()
{
	for (int32 i = 0; i < Decals.Num(); ++i)
	{
		UDecalComponent* decalComponent = Decals[i];
		{
			decalComponent->Modify();
			decalComponent->DestroyComponent(false);
		}
	}
	Decals.Reset();
}


void FParticleDecalEmitterInstance::KillParticles()
{
	// 粒子生命时间到了，先将Decals里的贴花处理掉
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
				FDecalParticlePayload* PayloadData = (FDecalParticlePayload*)((uint8*)&Particle + DecalOffset);
				for (int32 j = 0; j < Decals.Num(); ++j)
				{
					UDecalComponent* decalComponent = Decals[j];
						
					if (decalComponent && (uint64)decalComponent == PayloadData->DecalId)
					{
						decalComponent->Modify();
						decalComponent->DestroyComponent(false);
						Decals.RemoveAtSwap(j);
						--j;
						break;
					}
				}
			}
		}
	}
	FParticleEmitterInstance::KillParticles();

}

void FParticleDecalEmitterInstance::KillParticle(int32 Index)
{
	int32 KillIndex = ParticleIndices[Index];

	// Handle the kill event, if needed

	const uint8* ParticleBase = ParticleData + KillIndex * ParticleStride;
	FBaseParticle& Particle = *((FBaseParticle*)ParticleBase);

	if (Particle.RelativeTime > 1.0f)
	{
		//DECLARE_PARTICLE(Particle, ParticleData + ParticleStride * ParticleIndices[Index]);
		FDecalParticlePayload* PayloadData = (FDecalParticlePayload*)((uint8*)&Particle + DecalOffset);
		for (int32 j = 0; j < Decals.Num(); ++j)
		{
			UDecalComponent* decalComponent = Decals[j];

			if (decalComponent && (uint64)decalComponent == PayloadData->DecalId)
			{
				decalComponent->Modify();
				decalComponent->DestroyComponent(false);
				Decals.RemoveAtSwap(j);
				--j;
				break;
			}
		}
		FParticleEmitterInstance::KillParticle(Index);

	}
}

void FParticleDecalEmitterInstance::KillParticlesForced(bool bFireEvents /*= false*/)
{
	for (int32 j = 0; j < Decals.Num(); ++j)
	{
		UDecalComponent* decalComponent = Decals[j];
		if (decalComponent )
		{
			decalComponent->Modify();
			decalComponent->DestroyComponent(false);
			Decals.RemoveAtSwap(j);
			--j;
			break;
		}
	}
	FParticleEmitterInstance::KillParticlesForced(bFireEvents);
}

