// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/GameStatus/TeleportInWorldLoadingBaseStatus.h"
#include "InWorldLoadingStatus.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UInWorldLoadingStatus : public UTeleportInWorldLoadingBaseStatus
{
	GENERATED_BODY()
public:
	virtual void OnEnter(EGameStatus LastStatus)override;
	virtual void OnLeave()override;
};
