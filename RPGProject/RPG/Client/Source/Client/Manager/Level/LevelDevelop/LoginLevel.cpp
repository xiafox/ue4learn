// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginLevel.h"
#include "ClientGameInstance.h"
#include "Manager/Level/LevelManager.h"

void ALoginLevel::BeginPlay()
{
	Super::BeginPlay();
	
}
bool  ALoginLevel::CheckLoginRun()
{
	return Super::CheckLoginRun();	
}

void ALoginLevel::CreateDynamicRes()
{
	OpenLoginUI();
	Super::CreateDynamicRes();
}

void ALoginLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALoginLevel::OnLoadLevelFinished()
{
	UClientGameInstance::Instance->LevelManager->OnEndTeleportLoginLevel();	
}


void ALoginLevel::OpenLoginUI()
{
	
}
