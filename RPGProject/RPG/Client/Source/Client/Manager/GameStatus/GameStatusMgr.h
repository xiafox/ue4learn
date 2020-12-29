// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager/BaseManager.h"
#include "ClientTypes/GameFrameworkDefines.h"
#include "GameStatusMgr.generated.h"


class UBaseStatus;

/**
 * 
 */
UCLASS()
class CLIENT_API UGameStatusMgr : public UBaseManager
{
	GENERATED_BODY()

public:
	UGameStatusMgr();
	~UGameStatusMgr();

	void ChangeToNewStatus(EGameStatus InStatus);
	static UGameStatusMgr* GetInstance();
	///<获得当前游戏状态
	UFUNCTION(BlueprintCallable, Category = "GameStatusMgr")
	EGameStatus GetCurrGameState();

	void OnSetSpaceData(uint32 spaceID, const FString &key, const FString &value);

	bool CheckLevelLoadFinished();
public:
	UPROPERTY()
	UBaseStatus *CurrStatus;

	UPROPERTY()
	TMap< EGameStatus, UBaseStatus*> AllGameStatus;

};
