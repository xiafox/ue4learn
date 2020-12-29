// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleModuleTypeDataSkeletalMesh.h"
// UE4
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ParticleSkeletalMeshEmitterInstances.h"



FParticleEmitterInstance* UParticleModuleTypeDataSkeletalMesh::CreateInstance(UParticleEmitter* InEmitterParent, UParticleSystemComponent* InComponent)
{
	//SetToSensibleDefaults(InEmitterParent);
	FParticleEmitterInstance* Instance = new FParticleSkeletalMeshEmitterInstance();
	check(Instance);

	Instance->InitParameters(InEmitterParent, InComponent);

	//CreateDistribution();

	return Instance;
}
//////////////////////////////////////////////////////////////////////////
void UParticleModuleTypeDataSkeletalMesh::PostLoad()
{
	Super::PostLoad();

	if (Mesh != nullptr)
	{
		Mesh->ConditionalPostLoad();
	}
}

bool UParticleModuleTypeDataSkeletalMesh::IsPostLoadThreadSafe() const
{
	return false;
}

 UParticleModuleTypeDataSkeletalMesh::UParticleModuleTypeDataSkeletalMesh(const FObjectInitializer& ObjectInitializer)
 	: Super(ObjectInitializer)
 {
	 bSpawnModule = true;
	 bUpdateModule = true;
// 	CastShadows = false;
// 	DoCollisions = false;
// 	MeshAlignment = PSMA_MeshFaceCameraWithRoll;
// 	AxisLockOption = EPAL_NONE;
// 	CameraFacingUpAxisOption_DEPRECATED = CameraFacing_NoneUP;
// 	CameraFacingOption = XAxisFacing_NoUp;
// 	bCollisionsConsiderPartilceSize = true;
// 	bUseStaticMeshLODs = true;
// 	LODSizeScale = 1.0f;
 }
#if WITH_EDITOR
void UParticleModuleTypeDataSkeletalMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	if (PropertyThatChanged)
	{
		if (PropertyThatChanged->GetFName() == FName(TEXT("Mesh")))
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
