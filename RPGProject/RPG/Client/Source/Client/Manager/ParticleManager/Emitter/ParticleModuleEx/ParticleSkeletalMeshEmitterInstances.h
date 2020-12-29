// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "ParticleEmitterInstances.h"
#include "ParticleModuleTypeDataSkeletalMesh.h"

/*
* 文件名称：ParticleSkeletalMeshEmitterInstances.h
* 功能说明：
* 文件作者：zhengxuemei
* 目前维护：
* 创建时间：2019-02-19
*/

// struct SkMeshData
// {
// 	class USkeletalMeshComponent* MeshComponent;
// 	float lifetime;
// 	float lifetimer;
// 	SkMeshData(class USkeletalMeshComponent* SkMesh, float lifetime = 0.5f)
// 	{
// 		MeshComponent = SkMesh;
// 		this->lifetime = lifetime;
// 	}
// };

struct FParticleSkeletalMeshEmitterInstance : public FParticleMeshEmitterInstance
{
	UParticleModuleTypeDataSkeletalMesh* SkMeshTypeData;

	int32 SkeletalMeshOffset;

	FParticleSkeletalMeshEmitterInstance();

	virtual ~FParticleSkeletalMeshEmitterInstance();

	virtual void InitParameters(UParticleEmitter* InTemplate, UParticleSystemComponent* InComponent) override;

/*	virtual void Tick(float DeltaTime, bool bSuppressSpawning) override;*/

	virtual void UpdateBoundingBox(float DeltaTime) override;

//	virtual uint32 RequiredBytes() override;

	virtual void PostSpawn(FBaseParticle* Particle, float InterpolationPercentage, float SpawnTime) override;

	virtual bool IsDynamicDataRequired(UParticleLODLevel* CurrentLODLevel) override;

	virtual void KillParticles() override;
	virtual void KillParticle(int32 Index) override;
	virtual void KillParticlesForced(bool bFireEvents = false) override;

	TArray<class USkeletalMeshComponent*> Meshes;
	//TArray<SkMeshData> SkMeshes;


	/**
	 *	Spawn particles for this emitter instance
	 *	@param	DeltaTime		The time slice to spawn over
	 *	@return	float			The leftover fraction of spawning
	 */
	//virtual float Spawn(float DeltaTime) override;
};
