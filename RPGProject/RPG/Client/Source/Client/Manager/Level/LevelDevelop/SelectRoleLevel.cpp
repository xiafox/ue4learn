// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectRoleLevel.h"
#include "ClientGameInstance.h"
#include "Manager/Level/LevelManager.h"


void ASelectRoleLevel::BeginPlay()
{
	Super::BeginPlay();
}

void ASelectRoleLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASelectRoleLevel::CreateDynamicRes()
{
	bIsDynamicResFinished = true;
}

void ASelectRoleLevel::OnCreateModelFinished()
{
	bIsDynamicResFinished = true;
	OnLoadLevelFinished();
}

void ASelectRoleLevel::OnLoadLevelFinished()
{
	UClientGameInstance::Instance->LevelManager->OnEndTeleportSelectRoleLevel();
}