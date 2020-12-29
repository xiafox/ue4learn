// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStatus.h"
#include "Engine/EngineTypes.h"
#include "Entity.h"
#include "LoadingBaseStatus.generated.h"



/**
 * 
 */
UCLASS()
class CLIENT_API ULoadingBaseStatus : public UBaseStatus
{
	GENERATED_BODY()
public:

	virtual void OnEnter(EGameStatus LastStatus)override;
	virtual void OnLeave()override;
	bool GetLevelLoadFinished() { return bIsLevelLoadFinished; };

	UFUNCTION(BlueprintCallable)
		virtual void OnLevelLoadFinished();

protected:
	bool bIsLevelLoadFinished = false;
};
