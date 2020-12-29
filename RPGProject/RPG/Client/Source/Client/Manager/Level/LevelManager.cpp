// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "Client.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "ClientGameInstance.h"


#pragma optimize("", off)

ULevelManager::ULevelManager()
{
	
}

ULevelManager::~ULevelManager()
{
}

ULevelManager* ULevelManager::GetInstance()
{
	client_checkNoReentry();
	return NewObject<ULevelManager>();
}

void ULevelManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void ULevelManager::TeleportLoginLevel()
{
	oldLevel_ = currLevel_;
	currLevel_ = TEXT("L_Login");
	bIsLoadFinished = false;
	UGameplayStatics::OpenLevel(UClientGameInstance::Instance, TEXT("L_Login_Logic"), true, TEXT(""));

}

void ULevelManager::TeleportSelectRoleLevel()
{
	oldLevel_ = currLevel_;
	currLevel_ = TEXT("L_SelectRole");
	bIsLoadFinished = false;
	UGameplayStatics::OpenLevel(UClientGameInstance::Instance, TEXT("L_SelectRole_Logic"), true, TEXT(""));
}

void ULevelManager::TeleportCreateRoleLevel()
{
	oldLevel_ = currLevel_;
	currLevel_ = TEXT("L_CreateRole");
	bIsLoadFinished = false;
	UGameplayStatics::OpenLevel(UClientGameInstance::Instance, TEXT("L_CreateRole_Logic"), true, TEXT(""));	
}

void ULevelManager::TeleportInWorldLevel()
{
	FName InSpaceScriptID = TEXT("");
	///<获取当前地图id
	if (UClientGameInstance::KBEComponents && UClientGameInstance::KBEComponents->pKBEApp->pBaseApp())
	{
		InSpaceScriptID = FName(*UClientGameInstance::KBEComponents->pKBEApp->pBaseApp()->GetSpaceData(TEXT("SPACE_SCRIPT_ID")));
		if (currLevel_.Compare(InSpaceScriptID))
		{
			oldLevel_ = currLevel_;
			currLevel_ = InSpaceScriptID;
			bIsLoadFinished = false;
			UGameplayStatics::OpenLevel(UClientGameInstance::Instance, InSpaceScriptID, true, TEXT(""));
			return;
		}
	}

	OnEndTeleportInWorldLevel();
}

void ULevelManager::OnEndTeleportLoginLevel()
{
	bIsLoadFinished = true;
	//<通知加载器
	OnLoadFinished.Broadcast();
}

void ULevelManager::OnEndTeleportSelectRoleLevel()
{
	bIsLoadFinished = true;
	///<通知加载器
	OnLoadFinished.Broadcast();
}

void ULevelManager::OnEndTeleportCreateRoleLevel()
{
	bIsLoadFinished = true;
	///<通知加载器
	OnLoadFinished.Broadcast();
}

void ULevelManager::OnEndTeleportInWorldLevel()
{
	bIsLoadFinished = true;
	///<通知加载器
	OnLoadFinished.Broadcast();
}

FName ULevelManager::GetCurrLevelName() const
{
	return currLevel_;
}

bool ULevelManager::GetIsLoadingFinished()
{
	return bIsLoadFinished;
}
#pragma optimize("", on)