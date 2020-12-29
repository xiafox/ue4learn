// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Manager/BaseManager.h"
#include "LevelManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLevelLoadFinished);

UCLASS(Blueprintable)
class ULevelManager : public UBaseManager
{
	GENERATED_BODY()

public:
	ULevelManager();
	~ULevelManager();
	static ULevelManager* GetInstance();
	virtual void BeginDestroy()override;

	void TeleportLoginLevel(); ///<跳转到登录关卡

	void TeleportSelectRoleLevel(); ///<跳转到角色选择关卡

	void TeleportCreateRoleLevel(); ///<跳转到角色创建关卡

	void TeleportInWorldLevel(); ///<跳转到游戏世界关卡

		//加载登录关卡完毕回调
	void OnEndTeleportLoginLevel();

	//加载角色选择关卡完毕回调
	void OnEndTeleportSelectRoleLevel();

	//加载角色创建关卡完毕回调
	void OnEndTeleportCreateRoleLevel();

	void OnEndTeleportInWorldLevel();

	FName GetCurrLevelName() const;
	bool  GetIsLoadingFinished();
public:
	FOnLevelLoadFinished OnLoadFinished;
private:
	FName oldLevel_ = TEXT("");///<旧的关卡名
	FName currLevel_ = TEXT("");///<当前关卡名
	bool bIsLoadFinished = false;
};
