// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectEmitter.h"
#include "TrailEffectEmitter.generated.h"


/**
 * 拖尾光效基类
 */
UCLASS(abstract)
class  ATrailEffectEmitter : public AEffectEmitter
{
	GENERATED_BODY()
public:
	///重写，销毁拖尾粒子组件
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	///重写，创建拖尾粒子组件
	virtual void StartPlay() override;

	///重写，暂停拖尾粒子组件
	virtual void DelayDestroySetting() override;

public:
	///拖尾模式
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<enum ETrailWidthMode> WidthScaleMode;

private:
	///拖尾粒子组件
	UPROPERTY()
	UParticleSystemComponent* PSComponent;
};
