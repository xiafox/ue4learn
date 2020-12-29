// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ClientLevelScriptActor.h"
#include "SelectRoleLevel.generated.h"



UCLASS(BlueprintType)
class ASelectRoleLevel : public AClientLevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void CreateDynamicRes() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnLoadLevelFinished() override;
	void OnCreateModelFinished();
};
