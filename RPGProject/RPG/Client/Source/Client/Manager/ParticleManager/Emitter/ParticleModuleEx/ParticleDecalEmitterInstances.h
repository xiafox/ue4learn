// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "ParticleEmitterInstances.h"
#include "Particles/TypeData/ParticleModuleTypeDataBase.h"

/*
* 文件名称：ParticleDecalEmitterInstances.h
* 功能说明：
* 文件作者：zhengxuemei
* 目前维护：
* 创建时间：2019-04-19
*/

struct FDecalParticlePayload
{
	uint64		DecalId;
};

struct FParticleDecalEmitterInstance : public FParticleEmitterInstance
{

	FParticleDecalEmitterInstance();

	virtual ~FParticleDecalEmitterInstance();

	virtual void KillParticles() override;
	virtual void KillParticle(int32 Index) override;
	virtual void KillParticlesForced(bool bFireEvents = false) override;

	TArray<class UDecalComponent*> Decals;
	int32 DecalOffset;

};
