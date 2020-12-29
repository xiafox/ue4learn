// Fill out your copyright notice in the Description page of Project Settings.

#include "BackToSelectRoleLoadingStatus.h"
#include "ClientGameInstance.h"
#include "Manager/Loader/LoaderManager.h"

void UBackToSelectRoleLoadingStatus::OnEnter(EGameStatus LastStatus)
{
	Super::OnEnter(LastStatus);
	UClientGameInstance::Instance->LoaderManager->ChangeToNewLoader(ELoadingType::BackToSelectRoleLoading);
}

void UBackToSelectRoleLoadingStatus::OnLeave()
{

}
