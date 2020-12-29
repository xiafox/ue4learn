// Fill out your copyright notice in the Description page of Project Settings.


#include "LoaderManager.h"
#include "Client.h"
#include "ClientGameInstance.h"
#include "Manager/GameStatus/GameStatusMgr.h"
#include "LoaderDevelop/LoginLoader.h"
#include "Manager/Level/LevelManager.h"
#include "LoaderDevelop/CreateRoleLoader.h"
#include "LoaderDevelop/SelectRoleLoader.h"
#include "LoaderDevelop/WorldLoader.h"
#include "LoaderDevelop/TeleportLoader.h"
#include "LoaderDevelop/BackToSelectRoleLoader.h"

#pragma optimize("", off)

ULoaderManager::ULoaderManager()
{

}

ULoaderManager::~ULoaderManager()
{

}

ULoaderManager* ULoaderManager::GetInstance()
{
	client_checkNoReentry();
	return NewObject<ULoaderManager>();
}

void ULoaderManager::ChangeToNewLoader(ELoadingType LoadingType)
{
	if (LoadingType == ELoadingType::LoginLoading)
	{
		///<主要处理掉线后切换到登录加载状态需要判断当前是否处于加载中并不是登录加载时，先取消加载，是则不做处理。
		if (CurrLoader)
		{
			if (CurrLoaderType== ELoadingType::LoginLoading)
			{
				return;
			}
			else
			{
				CurrLoader->CancelLoading();
			}
		}
	}
	OnStartLoadingSpace(LoadingType);
}

void ULoaderManager::OnLoadFinished()
{
	///<通知状态管理器改变状态
	switch (CurrLoaderType)
	{
	case ELoadingType::LoginLoading:
		UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::Login);
		break;
	case ELoadingType::SelectRoleLoading:
		UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::SelectRole);
		break;
	case ELoadingType::BackToSelectRoleLoading:
		UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::SelectRole);
		break;
	case ELoadingType::CreateRoleLoading:
		UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::CreateRole);
		break;
	case ELoadingType::InWorldLoading:
		UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::InWorld);
		break;
	case ELoadingType::TeleportLoading:
		UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::InWorld);
		break;
	}
	CurrLoader = nullptr;
	CurrLoaderType = ELoadingType::None;
	UClientGameInstance::Instance->GetWorld()->GetTimerManager().ClearTimer(ProgressNotifyTimer);
	///<通知界面完成加载 关闭界面
	OnBPProgressNotify(1.0f, false);
}

void ULoaderManager::OnProgressNotify()
{
	if (CurrLoader)
	{
		OnBPProgressNotify(CurrLoader->GetCurProgress());
		return;
	}
	OnBPProgressNotify(0.0f);
}

void ULoaderManager::OnStartLoadingSpace(ELoadingType LoadingType)
{
	if (LoadingType == ELoadingType::None)return;

	switch (LoadingType)
	{
		case ELoadingType::LoginLoading:
			CurrLoader = NewObject<ULoginLoader>();
			break;
		case ELoadingType::SelectRoleLoading:
			CurrLoader = NewObject<USelectRoleLoader>();
			break;
		case ELoadingType::BackToSelectRoleLoading:
			CurrLoader = NewObject<UBackToSelectRoleLoader>();
			break;
		case ELoadingType::CreateRoleLoading:
			CurrLoader = NewObject<UCreateRoleLoader>();
			break;
		case ELoadingType::InWorldLoading:
			CurrLoader = NewObject<UWorldLoader>();
			break;
		case ELoadingType::TeleportLoading:
			CurrLoader = NewObject<UTeleportLoader>();
			break;
	}
	CurrLoaderType = LoadingType;
	///<显示进度条
	UClientGameInstance::Instance->GetWorld()->GetTimerManager().SetTimer(ProgressNotifyTimer, this, &ULoaderManager::OnProgressNotify, 0.1f, true);
	if (CurrLoader)
	{
		CurrLoader->OnLoadFinished.BindUObject(this, &ULoaderManager::OnLoadFinished);
		CurrLoader->StartLoading();
	}
}

void ULoaderManager::CancelLoading()
{	
	if (CurrLoader)
	{
		CurrLoader->CancelLoading();
		CurrLoader = nullptr;
		CurrLoaderType = ELoadingType::None;
		OnBPProgressNotify(0.0f, false);
		UClientGameInstance::Instance->GetWorld()->GetTimerManager().ClearTimer(ProgressNotifyTimer);		
	}
}
#pragma optimize("", on)