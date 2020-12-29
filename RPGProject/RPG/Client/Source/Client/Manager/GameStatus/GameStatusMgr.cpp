// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStatusMgr.h"
#include "Manager/GameStatus/StatusDevelop/LoginStatus.h"
#include "Manager/GameStatus/StatusDevelop/LoginLoadingStatus.h"
#include "Manager/GameStatus/StatusDevelop/SelectRoleStatus.h"
#include "Manager/GameStatus/StatusDevelop/SelectRoleLoadingStatus.h"
#include "Manager/GameStatus/StatusDevelop/BackToSelectRoleLoadingStatus.h"
#include "Manager/GameStatus/StatusDevelop/CreateRoleStatus.h"
#include "Manager/GameStatus/StatusDevelop/CreateRoleLoadingStatus.h"
#include "Manager/GameStatus/StatusDevelop/InWorldStatus.h"
#include "Manager/GameStatus/StatusDevelop/InWorldLoadingStatus.h"
#include "Manager/GameStatus/StatusDevelop/TeleportStatus.h"
#include "Manager/GameStatus/StatusDevelop/TeleportLoadingStatus.h"
#include "Client.h"

#pragma optimize("", off)
UGameStatusMgr::UGameStatusMgr()
{
	AllGameStatus.Add(EGameStatus::Login, Cast<UBaseStatus>(NewObject<ULoginStatus>()));
	AllGameStatus.Add(EGameStatus::LoginLoading, Cast<UBaseStatus>(NewObject<ULoginLoadingStatus>()));
	AllGameStatus.Add(EGameStatus::SelectRole, Cast<UBaseStatus>(NewObject<USelectRoleStatus>()));
	AllGameStatus.Add(EGameStatus::SelectRoleLoading, Cast<UBaseStatus>(NewObject<USelectRoleLoadingStatus>()));
	AllGameStatus.Add(EGameStatus::BackToSelectRoleLoading, Cast<UBaseStatus>(NewObject<UBackToSelectRoleLoadingStatus>()));
	AllGameStatus.Add(EGameStatus::CreateRole, Cast<UBaseStatus>(NewObject<UCreateRoleStatus>()));
	AllGameStatus.Add(EGameStatus::CreateRoleLoading, Cast<UBaseStatus>(NewObject<UCreateRoleLoadingStatus>()));
	AllGameStatus.Add(EGameStatus::InWorld, Cast<UBaseStatus>(NewObject<UInWorldStatus>()));
	AllGameStatus.Add(EGameStatus::InWorldLoading, Cast<UBaseStatus>(NewObject<UInWorldLoadingStatus>()));
	AllGameStatus.Add(EGameStatus::Teleport, Cast<UBaseStatus>(NewObject<UTeleportStatus>()));
	AllGameStatus.Add(EGameStatus::TeleportLoading, Cast<UBaseStatus>(NewObject<UTeleportLoadingStatus>()));
	CurrStatus = AllGameStatus[EGameStatus::Login];
}

UGameStatusMgr::~UGameStatusMgr()
{
}

UGameStatusMgr* UGameStatusMgr::GetInstance()
{
	client_checkNoReentry();
	return NewObject<UGameStatusMgr>();
}

EGameStatus UGameStatusMgr::GetCurrGameState()
{
	return CurrStatus->Status;
}

void UGameStatusMgr::OnSetSpaceData(uint32 spaceID, const FString &key, const FString &value)
{
	EGameStatus CurrGameStatus = *(AllGameStatus.FindKey(CurrStatus));
	if (CurrGameStatus == EGameStatus::CreateRole || CurrGameStatus == EGameStatus::SelectRole)
	{
		ChangeToNewStatus(EGameStatus::InWorldLoading);
	}
	else if (CurrGameStatus == EGameStatus::InWorld)
	{
		ChangeToNewStatus(EGameStatus::TeleportLoading);
	}
}


bool UGameStatusMgr::CheckLevelLoadFinished()
{
	if (CurrStatus->Status == EGameStatus::InWorld)
	{
		return true;
	}
	else if (Cast<ULoadingBaseStatus>( CurrStatus))
	{
		return Cast<ULoadingBaseStatus>(CurrStatus)->GetLevelLoadFinished();
	}
	return false;
}

void UGameStatusMgr::ChangeToNewStatus( EGameStatus InStatus )
{
	EGameStatus oldGameStatus = *(AllGameStatus.FindKey(CurrStatus));
	if (CurrStatus != nullptr)
	{
		CurrStatus->OnLeave();
	}
	CurrStatus = AllGameStatus[InStatus];
	CurrStatus->Status = InStatus;
	CurrStatus->OnEnter(oldGameStatus);
}

#pragma optimize("", on)