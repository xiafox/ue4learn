// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Particles/TypeData/ParticleModuleTypeDataBase.h"


#include "ParticleModuleBaseEX.h"
#include "ParticleModuleSkeletalMesh.generated.h"

/*
* 文件名称：ParticleModuleTypeDataSkeletalMesh.h
* 功能说明：
* 文件作者：zhengxuemei
* 目前维护：
* 创建时间：2019-02-19
*/

struct FSkMeshParticlePayload 
{
	uint64		MeshId;
};

UCLASS(editinlinenew/*, MinimalAPI*/, hidecategories = Object, meta = (DisplayName = "SkeletalMesh"))
class /*PARTICLEEDITOREX_API*/ UParticleModuleSkeletalMesh : public UParticleModuleBaseEX
{
	GENERATED_UCLASS_BODY()

	virtual bool CanTickInAnyThread() override
	{
		return false;
	}

	/** Just to show the Mesh, please check it off in game. */
	UPROPERTY(EditAnywhere, Category = Mesh)
	bool VisibleMesh;
	
	UPROPERTY(EditAnywhere, Category = Mesh)
	int32 TranslucentSortPriority;

	//////////////////////////////////////////////////////////////////////////
	/** Initializes the default values for this property */
	//void InitializeDefaults();


	//~ Begin UObject Interface
 #if WITH_EDITOR
	 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	 #endif // WITH_EDITOR
		virtual void PostInitProperties() override;
		//~ End UObject Interface
	
	
		//Begin UParticleModule Interface
		virtual void Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase) override;
		virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;
		virtual uint32 RequiredBytes(UParticleModuleTypeDataBase* TypeData) override;
		//virtual void SetToSensibleDefaults(UParticleEmitter* Owner) override;
	virtual EModuleType	GetModuleType() const override { return EPMT_Spawn; }
	//virtual void Render3DPreview(FParticleEmitterInstance* Owner, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	//End UParticleModule Interface

	void SpawnEx(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, struct FRandomStream* InRandomStream, FBaseParticle* ParticleBase);

	//virtual bool CanTickInAnyThread() override;


private:

	uint64 SpawnSkMesh(const FSkMeshParticlePayload& Payload, FBaseParticle& Particle, FParticleEmitterInstance* Owner);
	void UpdateSkMesh(USkeletalMeshComponent* MeshComponent, const FMeshRotationPayloadData* Payload, const FBaseParticle& Particle, bool bLocalSpace);
	void UpdateMaterial(const FBaseParticle& Particle);

	// 判断当前发射器是不是FParticleSkeletalMeshEmitterInstance
	bool IsSkMeshEmitterInstance(FParticleEmitterInstance* Owner);
	   
	TArray<UMaterialInstanceDynamic* > dyMaterials;
	//~ End UParticleModuleTypeDataBase Interface
};
