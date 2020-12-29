// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "DecalComponentEX.generated.h"

/*
* 文件名称：CS3DecalComponent.h
* 功能说明：
* 文件作者：zhangfudong
* 目前维护：zhangfudong
* 创建时间：2018-5-31
*/
UCLASS(BlueprintType, Blueprintable, meta = (ShortTooltip = "CS3DecalComponent ."))
class  UDecalComponentEX : public UDecalComponent
{
	GENERATED_BODY()
	
public:

	///重写渲染
	virtual void SendRenderTransform_Concurrent() override;

	///第一帧渲染
	virtual void CreateRenderState_Concurrent() override;
	
	//每一帧更新位置
	void SetComponentTransform();

	//计算实际的Yaw值
	void CalculateDecalRealYaw();

private:
	USceneComponent* ParentComponent;
	float RealYaw = 0;
};

