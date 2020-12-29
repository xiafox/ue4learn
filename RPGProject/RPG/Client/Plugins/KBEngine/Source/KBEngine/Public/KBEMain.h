// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KBEngineApp.h"
#include "KBEPersonality.h"

/*
插件的入口模块
在这个入口中安装了需要监听的事件(installEvents)，同时初始化KBEngine(initKBEngine)
*/

class KBENGINE_API FKBEMain : public FTickableGameObject
{

	static FKBEMain* instance;

public:
	static FKBEMain* Instance()
	{
		return FKBEMain::instance;
	}

	// Sets default values for this actor's properties
	FKBEMain(KBEngine::KBEPersonality* Imp);
	~FKBEMain();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	virtual bool IsTickable() const { return true; };
	virtual bool IsTickableInEditor() const { return true; };
	virtual bool IsTickableWhenPaused() const { return true; };
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FKBEMain, STATGROUP_Tickables);
	}


	virtual void KBEUpdate();
	virtual void InitKBEngine();

	void SetIpAddr(const FString& Host, const int32& Port);

	void Login(const FString& Account, const FString& PassWords, const TArray<uint8>& Datas);

	void CreateAccount(const FString& username, const FString& password, const TArray<uint8>& datas);
	void ResetPassword(const FString& username);

	void ReLoginBaseapp();
	void BindAccountEmail(const FString& emailAddress);
	void NewPassword(const FString old_password, const FString new_password);

	// 通过EntityID查找与之对应的Entity实例
	KBEngine::Entity* FindEntity(int32 entityID);

	const TMap<int32, KBEngine::Entity*>* GetAllEntities();

public:
	FString host = "127.0.0.1";

	int32 port = 20013;
	
	KBEngine::CLIENT_TYPE clientType = KBEngine::CLIENT_TYPE::CLIENT_TYPE_WIN;

	FString persistentDataPath = "Application.PersistentDataPath";

	bool syncPlayer = true;

	int32 sendBufferMax = 32768;

	int32 recvBufferMax = 65535;

	bool useAliasEntityID = true;

	bool isOnInitCallPropertysSetMethods = true;


	KBEngine::KBEngineApp * pKBEApp = nullptr;
};
