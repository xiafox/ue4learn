// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ClientObject.generated.h"


/**
 * 基础对象类
 */
UCLASS(BlueprintType, Blueprintable)
class UClientObject : public UObject
{
	GENERATED_BODY()
public:
	UClientObject();
	~UClientObject();
	virtual void PostInitProperties()override;///<初始化
	virtual void BeginDestroy()override;
	virtual void ShutdownAfterError()override;
};
