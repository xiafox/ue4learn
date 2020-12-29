// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectEmitter.h"
#include "FlyEffectEmitter.generated.h"


struct FOTHER_EFFECT_DATA;

/// 飞行的类型
UENUM()
enum class FLY_TYPE : uint8
{
	NORMAL	= 1		UMETA(DisplayName = "NORMAL"),	///<正常阶段
	SECOND	= 2		UMETA(DisplayName = "SECOND"),	///<第二阶段
	THIRD	= 3		UMETA(DisplayName = "THIRD"),	///<第三阶段
};

/**
 * 飞行光效基类
 */
UCLASS(abstract)
class  AFlyEffectEmitter : public AEffectEmitter
{
	GENERATED_BODY()

public:
	AFlyEffectEmitter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	///获得目标位置
	UFUNCTION(BlueprintPure, Category = "FlyEffect")
	FVector GetFlyTargetPos(bool bInit = false);

	///获取自己的位置
	UFUNCTION(BlueprintPure, Category = "FlyEffect")
	virtual FVector GetFlySourcePos(bool bInit = false);

protected:
	///初始化配置
	virtual void InitFromConfig();

	///初始化位置
	virtual void InitEmitterTransform() override;

	///飞行结束回调
	UFUNCTION()
	virtual void OnFlyFinished();

	///播放飞行抵达光效，不随主光效销毁而销毁
	void PlayTriggerParticle();

	///正常移动
	virtual void NormalMove(float DeltaTime, FVector RisingOffset = FVector(0, 0, 0));

	///第二阶段移动
	virtual void SecondMove(float DeltaTime);

	///获得第二阶段运动的角度
	virtual void GetSecondMoveRotation(FRotator& OutRotation);

	///准备进入下一阶段阶段
	virtual void EnterNextMove();

	///检查并设置移动平台
	bool CheckAndSetEmitterBase();

protected:

	// 完成时间
	float CompletedTime;

	// 第二阶段曲线上升的百分比
	float RisingPercent = 0.0f;

	// 上一帧曲线的百分比
	float LastRisingPercent = 0.0f;

	// 曲线累计偏移高度
	float TotalRisingOffset = 0.0f;

	// 飞行光效的阶段，正常阶段和曲线阶段
	FLY_TYPE FlyType = FLY_TYPE::NORMAL;

	// 如果光效是在移动平台上，并且光效的两个目标都是在相同移动平台上，则持对象是移动平台
	UPROPERTY()
	AActor* ParentActor = nullptr;

	// 配置属性指针缓存
	const FOTHER_EFFECT_DATA* OtherEffectData;

	// 判断是否可以做其他阶段运动
	UPROPERTY(BlueprintReadOnly)
	bool bCanSpecialMove;

	// 判断是否可以做其他阶段运动
	UPROPERTY(BlueprintReadOnly)
	float FlySpeed = 0.f;

	// 做曲线运动的最近距离
	UPROPERTY(BlueprintReadOnly)
	float TransformationDistance;

	// 曲线运动上升的高度
	UPROPERTY(BlueprintReadOnly)
	float RisingDistance;

	// 按时间飞
	UPROPERTY(BlueprintReadOnly)
	bool bFlyOfTime;
};
