// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientGameInstance.h"
#include "KBEMain.h"
#include "Entity/ClientPersonality.h"
#include "Manager/GameStatus/GameStatusMgr.h"
#include "Manager/LoginManager.h"
#include "Manager/Level/LevelManager.h"
#include "Manager/Loader/LoaderManager.h"
#include "Manager/Cfg/CfgManager.h"
#include "Entity/EntityDeclare.h"
#include "Manager/ParticleManager/ParticleManager.h"
#include "Manager/ResourceManager.h"


UClientGameInstance* UClientGameInstance::Instance = nullptr;
FKBEMain* UClientGameInstance::KBEComponents = nullptr;


void UClientGameInstance::Init()
{
	Super::Init();
	UClientGameInstance::Instance = this;

	KBEComponents = new FKBEMain(new ClientPersonality());

	GameStatusMgr = UGameStatusMgr::GetInstance();
	LoginManager = ULoginManager::GetInstance();
	LevelManager = ULevelManager::GetInstance();
	LoaderManager = ULoaderManager::GetInstance();
	CfgManager = UCfgManager::GetInstance();
	ParticleManager = UParticleManager::GetInstance();
	ResourceManager = UResourceManager::GetInstance();
	EntityDeclare();
}

void UClientGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
}

void UClientGameInstance::FinishDestroy()
{
	Super::FinishDestroy();
}

void UClientGameInstance::Shutdown()
{
	if (KBEComponents)
	{
		delete KBEComponents;
		KBEComponents = nullptr;
	}
	GameStatusMgr = nullptr;
	LoginManager = nullptr;
	LevelManager = nullptr;
	LoaderManager = nullptr;
	CfgManager = nullptr;
	Super::Shutdown();
}
