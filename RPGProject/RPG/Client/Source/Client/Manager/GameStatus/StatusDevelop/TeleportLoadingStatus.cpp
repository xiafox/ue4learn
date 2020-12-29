// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportLoadingStatus.h"
#include "ClientGameInstance.h"
#include "Manager/Loader/LoaderManager.h"

void UTeleportLoadingStatus::OnEnter(EGameStatus LastStatus)
{
	Super::OnEnter(LastStatus);
	UClientGameInstance::Instance->LoaderManager->ChangeToNewLoader(ELoadingType::TeleportLoading);
}

void UTeleportLoadingStatus::OnLeave()
{

}
