// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "ClientLevelScriptActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AClientLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
public:	
	AClientLevelScriptActor(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void CreateDynamicRes();

	virtual bool CheckLoginRun();		//是从登录进来的，还是直接在某关卡播放
	
	void OnLoadLevelDelay();

	virtual void OnLoadLevelFinished();

protected:
	FTimerHandle DelayLoadFinishedHandle;
	bool bIsDynamicResFinished = false;
};
