// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadingBaseStatus.h"
#include "ClientGameInstance.h"
#include "Manager/Level/LevelManager.h"



void ULoadingBaseStatus::OnEnter(EGameStatus LastStatus)
{
	Super::OnEnter(LastStatus);
	bIsLevelLoadFinished = false;	
	UClientGameInstance::Instance->LevelManager->OnLoadFinished.AddUObject(this, &ULoadingBaseStatus::OnLevelLoadFinished);

}

void ULoadingBaseStatus::OnLeave()
{
	UClientGameInstance::Instance->LevelManager->OnLoadFinished.RemoveAll(this);
	Super::OnLeave();
}

void ULoadingBaseStatus::OnLevelLoadFinished()
{
	if (!bIsLevelLoadFinished)
	{
		bIsLevelLoadFinished = true;
	}
}

