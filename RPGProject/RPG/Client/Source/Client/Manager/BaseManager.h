// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ClientObject.h"
#include "BaseManager.generated.h"


UCLASS(BlueprintType)
class UBaseManager : public UClientObject
{
	GENERATED_BODY()
public:
	virtual void PostInitProperties() override;
};
