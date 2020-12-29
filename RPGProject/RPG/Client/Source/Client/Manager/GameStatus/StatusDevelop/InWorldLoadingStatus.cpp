// Fill out your copyright notice in the Description page of Project Settings.

#include "InWorldLoadingStatus.h"
#include "ClientGameInstance.h"
#include "Manager/Loader/LoaderManager.h"

void UInWorldLoadingStatus::OnEnter(EGameStatus LastStatus)
{
	Super::OnEnter(LastStatus);
	UClientGameInstance::Instance->LoaderManager->ChangeToNewLoader(ELoadingType::InWorldLoading);
}

void UInWorldLoadingStatus::OnLeave()
{

}