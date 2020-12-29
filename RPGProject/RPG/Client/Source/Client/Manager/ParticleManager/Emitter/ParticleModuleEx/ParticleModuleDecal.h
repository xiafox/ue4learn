// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Particles/TypeData/ParticleModuleTypeDataBase.h"

#include "ParticleDecalEmitterInstances.h"
#include "ParticleModuleBaseEX.h"
#include "ParticleModuleDecal.generated.h"

/*
* 文件名称：ParticleModuleDecal.h
* 功能说明：
* 文件作者：zhengxuemei
* 目前维护：
* 创建时间：2019-04-19
*/


UCLASS(editinlinenew/*, MinimalAPI*/, hidecategories = Object, meta = (DisplayName = "Decal"))
class /*PARTICLEEDITOREX_API*/ UParticleModuleDecal : public UParticleModuleBaseEX
{
	GENERATED_UCLASS_BODY()

	virtual bool CanTickInAnyThread() override
	{
		return false;
	}

	/** Just to show the Mesh, please check it off in game. */
	UPROPERTY(EditAnywhere, Category = Decal)
	bool VisibleDecal;

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
	virtual EModuleType	GetModuleType() const override { return EPMT_Spawn; }

	void SpawnEx(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, struct FRandomStream* InRandomStream, FBaseParticle* ParticleBase);



private:

	uint64 SpawnDecal(const FDecalParticlePayload& Payload, FBaseParticle& Particle, FParticleEmitterInstance* Owner);
	void UpdateDecal(UDecalComponent* decalComponent, const FBaseParticle& Particle, bool bLocalSpace);
	void UpdateMaterial(const FBaseParticle& Particle);
	// 判断当前发射器是不是FParticleDecalEmitterInstance
	bool IsDecalEmitterInstance(FParticleEmitterInstance* Owner);

	UMaterialInstanceDynamic* dyMaterial;
	//~ End UParticleModuleTypeDataBase Interface
};
