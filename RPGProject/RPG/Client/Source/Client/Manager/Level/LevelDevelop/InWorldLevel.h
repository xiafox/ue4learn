// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../ClientLevelScriptActor.h"
#include "InWorldLevel.generated.h"



UCLASS(BlueprintType, Blueprintable)
class AInWorldLevel : public AClientLevelScriptActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void CreateDynamicRes() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnLoadLevelFinished() override;
	virtual bool CheckLoginRun()override;		//是从登录进来的，还是直接在某关卡播放
};
