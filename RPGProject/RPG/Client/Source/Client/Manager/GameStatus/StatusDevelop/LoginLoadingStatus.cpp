// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginLoadingStatus.h"
#include "ClientGameInstance.h"
#include "Manager/Loader/LoaderManager.h"

void ULoginLoadingStatus::OnEnter(EGameStatus LastStatus)
{
	Super::OnEnter(LastStatus);
	UClientGameInstance::Instance->LoaderManager->ChangeToNewLoader(ELoadingType::LoginLoading);
}

void ULoginLoadingStatus::OnLeave()
{

}
