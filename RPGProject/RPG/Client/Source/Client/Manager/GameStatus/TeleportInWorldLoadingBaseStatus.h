// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadingBaseStatus.h"
#include "Engine/EngineTypes.h"
#include "Entity.h"
#include "TeleportInWorldLoadingBaseStatus.generated.h"



/**
 *
 */
UCLASS()
class CLIENT_API UTeleportInWorldLoadingBaseStatus : public ULoadingBaseStatus
{
	GENERATED_BODY()

public:
	virtual void OnEnter(EGameStatus LastStatus)override;
	virtual void OnLeave()override;
	virtual void OnLevelLoadFinished()override;
private:
	void OnTimerEnterScenes();
private:
	FTimerHandle LoaderTimerHander;
	TArray<KBEngine::Entity* > WaitingEnterScenes;
};
