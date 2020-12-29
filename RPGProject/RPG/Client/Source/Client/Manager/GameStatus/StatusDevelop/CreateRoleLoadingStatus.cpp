// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateRoleLoadingStatus.h"
#include "ClientGameInstance.h"
#include "Manager/Loader/LoaderManager.h"

void UCreateRoleLoadingStatus::OnEnter(EGameStatus LastStatus)
{
	Super::OnEnter(LastStatus);
	UClientGameInstance::Instance->LoaderManager->ChangeToNewLoader(ELoadingType::CreateRoleLoading);
}

void UCreateRoleLoadingStatus::OnLeave()
{

}