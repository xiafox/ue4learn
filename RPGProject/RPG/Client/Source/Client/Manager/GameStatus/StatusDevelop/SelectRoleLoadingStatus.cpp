// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectRoleLoadingStatus.h"
#include "ClientGameInstance.h"
#include "Manager/Loader/LoaderManager.h"

void USelectRoleLoadingStatus::OnEnter(EGameStatus LastStatus)
{
	Super::OnEnter(LastStatus);
	UClientGameInstance::Instance->LoaderManager->ChangeToNewLoader(ELoadingType::SelectRoleLoading);
}

void USelectRoleLoadingStatus::OnLeave()
{

}