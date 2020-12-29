// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParticleModuleBaseEX.h"
#include "ParticleModuleColorOverLifeEX.generated.h"

/*
* 文件名称：ParticleModuleColorOverLifeEX.h
* 功能说明：
* 文件作者：zhangfudong
* 目前维护：zhangfudong
* 创建时间：2018-08-09
*/
struct FParticleEmitterInstance;
class AEffectEmitter;

UCLASS(editinlinenew, hidecategories = Object)
class UParticleModuleColorOverLifeEX : public UParticleModuleBaseEX
{
	GENERATED_UCLASS_BODY()

	// 新模块，支持粒子渐变，设置透明度
	virtual void Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase) override;
	virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;

	virtual bool CanTickInAnyThread()
	{
		return false;
	}
};

UCLASS(editinlinenew, hidecategories = Object, meta = (DisplayName = "Get Particle Transform"))
class  UParticleModuleTransformEX : public UParticleModuleBaseEX
{
	GENERATED_UCLASS_BODY()

	// 获取粒子的位置和旋转
	virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;

	// 获取模型方向发出的射线碰到的点
	FVector GetLocation(FParticleEmitterInstance* Owner, FVector& Location, FRotator& Rotation);

	virtual bool CanTickInAnyThread()
	{
		return false;
	}

public:
	///粒子模板
	UPROPERTY(EditAnywhere, Category = Particles)
	UParticleSystem* Template;

	///是否按方向查询碰撞
	UPROPERTY(EditAnywhere, Category = Particles)
	bool bTrace = true;

	///射线碰撞类型
	UPROPERTY(EditAnywhere, Category = Particles)
	TArray<TEnumAsByte<ECollisionChannel>> CollisionType;

	///射线检测的距离
	UPROPERTY(EditAnywhere, Category = Particles)
	float TraceLength = 10000.f;

	///模型的正方向偏移角度
	UPROPERTY(EditAnywhere, Category = Particles)
	FRotator RotationOffset = FRotator(90.f, 0.f, 0.f);

	///是否变换缩放X
	UPROPERTY(EditAnywhere, Category = Particles)
	bool bTransformScaleX = false;

	///是否变换缩放Y
	UPROPERTY(EditAnywhere, Category = Particles)
	bool bTransformScaleY = false;

	///是否变换缩放Z
	UPROPERTY(EditAnywhere, Category = Particles)
	bool bTransformScaleZ = true;

	///模型的长宽高
	UPROPERTY(EditAnywhere, Category = Particles)
	FVector MeshSize;
};
