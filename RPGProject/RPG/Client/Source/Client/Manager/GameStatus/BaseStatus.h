// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientObject.h"
#include "ClientTypes/GameFrameworkDefines.h"
#include "BaseStatus.generated.h"


/**
 * 
 */
UCLASS()
class CLIENT_API UBaseStatus : public UClientObject
{
	GENERATED_BODY()
public:
	EGameStatus	Status;

	virtual void OnEnter( EGameStatus LastStatus );
	virtual void OnLeave();
	
};
