// Fill out your copyright notice in the Description page of Project Settings.


#include "InWorldLevel.h"
#include "ClientGameInstance.h"
#include "Manager/Level/LevelManager.h"
#pragma optimize("", off)


void AInWorldLevel::BeginPlay()
{
	Super::BeginPlay();
}

bool AInWorldLevel::CheckLoginRun()
{
	return Super::CheckLoginRun();
}

void AInWorldLevel::CreateDynamicRes()
{
	Super::CreateDynamicRes();
}

void AInWorldLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AInWorldLevel::OnLoadLevelFinished()
{
	UClientGameInstance::Instance->LevelManager->OnEndTeleportInWorldLevel();
}

#pragma optimize("", on)
