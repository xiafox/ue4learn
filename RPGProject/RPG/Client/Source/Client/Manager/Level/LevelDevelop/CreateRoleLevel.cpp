// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateRoleLevel.h"
#include "ClientGameInstance.h"
#include "Manager/Level/LevelManager.h"

void ACreateRoleLevel::BeginPlay()
{
	Super::BeginPlay();
}

void ACreateRoleLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACreateRoleLevel::CreateDynamicRes()
{
	bIsDynamicResFinished = true;
}

void ACreateRoleLevel::OnCreateModelFinished()
{
	bIsDynamicResFinished = true;
	OnLoadLevelFinished();
}

void ACreateRoleLevel::OnLoadLevelFinished()
{
	UClientGameInstance::Instance->LevelManager->OnEndTeleportCreateRoleLevel();

}
