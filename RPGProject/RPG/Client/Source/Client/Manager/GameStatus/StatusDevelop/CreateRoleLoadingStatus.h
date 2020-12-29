// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/GameStatus/LoadingBaseStatus.h"
#include "CreateRoleLoadingStatus.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UCreateRoleLoadingStatus : public ULoadingBaseStatus
{
	GENERATED_BODY()
public:
	virtual void OnEnter(EGameStatus LastStatus)override;
	virtual void OnLeave()override;
};
