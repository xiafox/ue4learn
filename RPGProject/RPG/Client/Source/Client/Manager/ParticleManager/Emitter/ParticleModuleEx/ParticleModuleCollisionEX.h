// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParticleModuleBaseEX.h"
#include "ParticleModuleCollisionEX.generated.h"

/*
* 文件名称：ParticleModuleCollisionEX.h
* 功能说明：
* 文件作者：zhangfudong
* 目前维护：zhangfudong
* 创建时间：2018-09-11
*/

struct FParticleEmitterInstance;

UCLASS(editinlinenew, hidecategories = Object, meta = (DisplayName = "BeginForceUpdate"))
class  UBeginParticleModuleCollisionEX : public UParticleModuleBaseEX
{
	GENERATED_UCLASS_BODY()

	// 强制粒子碰撞更新开始
	virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;

	virtual bool CanTickInAnyThread()
	{
		return false;
	}
	
public:
	uint32 bCurrentDropDetail;
};

UCLASS(editinlinenew, hidecategories = Object, meta = (DisplayName = "EndForceUpdate"))
class  UEndParticleModuleCollisionEX : public UParticleModuleBaseEX
{
	GENERATED_UCLASS_BODY()
	
	// 强制粒子碰撞更新完毕
	virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;

	virtual bool CanTickInAnyThread()
	{
		return false;
	}

};

