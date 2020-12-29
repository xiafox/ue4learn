// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleModuleTypeDataDecal.h"
// UE4
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ParticleSkeletalMeshEmitterInstances.h"



FParticleEmitterInstance* UParticleModuleTypeDataDecal::CreateInstance(UParticleEmitter* InEmitterParent, UParticleSystemComponent* InComponent)
{
	FParticleEmitterInstance* Instance = new FParticleDecalEmitterInstance();
	check(Instance);

	Instance->InitParameters(InEmitterParent, InComponent);
	
	return Instance;
}
//////////////////////////////////////////////////////////////////////////
void UParticleModuleTypeDataDecal::PostLoad()
{
	Super::PostLoad();

}

bool UParticleModuleTypeDataDecal::IsPostLoadThreadSafe() const
{
	return false;
}

UParticleModuleTypeDataDecal::UParticleModuleTypeDataDecal(const FObjectInitializer& ObjectInitializer)
 	: Super(ObjectInitializer)
 {
	 bSpawnModule = true;
	 bUpdateModule = true;
 }
#if WITH_EDITOR
void UParticleModuleTypeDataDecal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	if (PropertyThatChanged)
	{
		if (PropertyThatChanged->GetFName() == FName(TEXT("Decal")))
		{
			UObject* OuterObj = GetOuter();
			check(OuterObj);
			UParticleLODLevel* LODLevel = Cast<UParticleLODLevel>(OuterObj);
			if (LODLevel)
			{
				// The outer is incorrect - warn the user and handle it
// 				UE_LOG(LogParticles, Warning, TEXT("UParticleModuleTypeDataMesh has an incorrect outer... run FixupEmitters on package %s"),
// 					*(OuterObj->GetOutermost()->GetPathName()));
				OuterObj = LODLevel->GetOuter();
				UParticleEmitter* Emitter = Cast<UParticleEmitter>(OuterObj);
				check(Emitter);
				OuterObj = Emitter->GetOuter();
			}
			UParticleSystem* PartSys = CastChecked<UParticleSystem>(OuterObj);

			PartSys->PostEditChangeProperty(PropertyChangedEvent);
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR
