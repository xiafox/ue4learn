#include "LoginManager.h"
#include "../Client.h"
#include "ClientGameInstance.h"
#include "../Entity/ClientEntity.h"
#include "../Entity/Account/Account.h"
#include "../Entity/Role/Role.h"


#pragma optimize("", off)

ULoginManager::ULoginManager()
{

}

ULoginManager::~ULoginManager()
{

}

ULoginManager* ULoginManager::GetInstance()
{
	client_checkNoReentry();
	return NewObject<ULoginManager>();
}

void ULoginManager::Login(FString account, FString password)
{
	if (currLoginStage == ELoginStage::TryLoginStage)return;

	SetLoginStage(ELoginStage::TryLoginStage);
	NotifyServerLogin(account, password);
}

void ULoginManager::CreateRole(FString playerName, int32 profession, int32 camp, int32 gender)
{
	KBEngine::Entity* entity = UClientGameInstance::Instance->KBEComponents->pKBEApp->Player();
	if (entity != nullptr)
	{
		ClientEntity* pEntity_Player = static_cast<ClientEntity*>(entity);
		if (pEntity_Player->IsPlayerAccount())
		{
			Account* account = static_cast<Account*>(pEntity_Player);
			account->BASE_AddRole(playerName, profession, camp, gender);
		}
	}
}

void ULoginManager::DeleteRole(FString dbid)
{
	KBEngine::Entity* entity = UClientGameInstance::Instance->KBEComponents->pKBEApp->Player();
	if (entity != nullptr)
	{
		ClientEntity* pEntity_Player = static_cast<ClientEntity*>(entity);
		if (pEntity_Player->IsPlayerAccount())
		{
			Account* account = static_cast<Account*>(pEntity_Player);
			account->BASE_DeleteRole(FSTRING_TO_UINT64(dbid));
		}
	}
}

void ULoginManager::EnterGame(FString dbid)
{
	KBEngine::Entity* entity = UClientGameInstance::Instance->KBEComponents->pKBEApp->Player();
	if (entity != nullptr)
	{
		ClientEntity* pEntity_Player = static_cast<ClientEntity*>(entity);
		if (pEntity_Player->IsPlayerAccount())
		{
			Account* account = static_cast<Account*>(pEntity_Player);
			account->BASE_EnterGame(FSTRING_TO_UINT64(dbid));
		}
	}
}

void ULoginManager::Logoff()
{
	KBEngine::Entity* entity = UClientGameInstance::Instance->KBEComponents->pKBEApp->Player();
	if (entity != nullptr)
	{
		ClientEntity* pEntity_Player = static_cast<ClientEntity*>(entity);
		if (pEntity_Player->IsPlayerAccount())
		{
			Account* account = static_cast<Account*>(pEntity_Player);
			account->BASE_Logoff();
		}
		else
		{
			Role* role = static_cast<Role*>(pEntity_Player);
			role->BASE_Logoff();
		}
	}
}

void ULoginManager::Logout()
{
	KBEngine::Entity* entity = UClientGameInstance::Instance->KBEComponents->pKBEApp->Player();
	if (entity != nullptr)
	{
		ClientEntity* pEntity_Player = static_cast<ClientEntity*>(entity);
		if (pEntity_Player->IsPlayerRole())
		{
			Role* role = static_cast<Role*>(pEntity_Player);
			role->BASE_Logout();
		}
	}
}

void ULoginManager::OnQueryRoles(const FROLEINFO_T& InRoleInfos)
{
	//保存服务器发送过来的玩家角色信息数据
	SetRoleInfos(InRoleInfos);
	//角色数据大于0，设置游戏状态为角色选择加载阶段，否则设置游戏状态为角色创建加载阶段
	if (UClientGameInstance::Instance->GameStatusMgr->GetCurrGameState() == EGameStatus::InWorld)
	{
		//从游戏世界返回角色选择，请求角色信息回调,设置游戏状态为返回角色选择加载阶段
		UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::BackToSelectRoleLoading);
	}
	else
	{
		//登陆游戏请求角色信息回调
		if (roleInfoDatas.Num() > 0)
		{
			UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::SelectRoleLoading);
		}
		else
		{
			UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::CreateRoleLoading);
		}
	}
}

void ULoginManager::OnAddRole(const FROLEINFO& RoleInfo)
{
	//保存服务器发送过来的玩家角色数据
	AddRoleInfo(RoleInfo);
	//创建角色直接进入游戏
	EnterGame(RoleInfo.DBid);
}

void ULoginManager::OnDeleteRole(const uint64& dbid)
{
	//删除客户端保存的数据
	DeleteRoleInfo(dbid);
	//选择关卡旋转转盘
}

void ULoginManager::OnLoginSuccessed()
{
	SetLoginStage(ELoginStage::AccountLoginStage);
	//请求角色信息
	KBEngine::Entity* entity = UClientGameInstance::Instance->KBEComponents->pKBEApp->Player();
	if (entity != nullptr)
	{
		ClientEntity* pEntity_Player = static_cast<ClientEntity*>(entity);
		if (pEntity_Player->IsPlayerAccount())
		{
			Account* account = static_cast<Account*>(pEntity_Player);
			account->BASE_QueryRoles();
		}
	}
}

void ULoginManager::OnLoginFailed()
{
	SetLoginStage(ELoginStage::NoLoginStage);
}

void ULoginManager::OnDisconnect()
{
	SetLoginStage(ELoginStage::NoLoginStage);
	//设置游戏状态为账号登录加载阶段(主动退出和服务器掉线)
	UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::LoginLoading);
}

void ULoginManager::OnPlayerEnterWorld()
{
	SetLoginStage(ELoginStage::RoleLoginStage);
	//设置游戏状态为角色创建加载阶段
	//UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::InWorldLoading);
}

bool ULoginManager::GetIsLoginStatus()
{
	return (currLoginStage == ELoginStage::AccountLoginStage || currLoginStage == ELoginStage::RoleLoginStage);
}

ELoginStage ULoginManager::GetCurrLoginStage()
{
	return currLoginStage;
}

void ULoginManager::NotifyServerLogin(FString account, FString password)
{
	TArray<uint8> datas;
	UClientGameInstance::Instance->KBEComponents->Login(account, password, datas);
}

void ULoginManager::OnTryLoginTimeOut()
{
	if (currLoginStage == ELoginStage::TryLoginStage)
	{
		SetLoginStage(ELoginStage::NoLoginStage);
	}
}

void ULoginManager::SetLoginStage(ELoginStage inLoginStage)
{
	currLoginStage = inLoginStage;
	OnLoginStageChanged(inLoginStage);
}

void ULoginManager::OnLoginStageChanged(ELoginStage newLoginStage)
{
	if (newLoginStage == ELoginStage::TryLoginStage)
	{
		UClientGameInstance::Instance->GetTimerManager().SetTimer(TryLoginHander, this, &ULoginManager::OnTryLoginTimeOut, 5.0f, false);
	}
	else if (newLoginStage == ELoginStage::NoLoginStage || newLoginStage == ELoginStage::AccountLoginStage)
	{
		if (TryLoginHander.IsValid())
		{
			UClientGameInstance::Instance->GetTimerManager().ClearTimer(TryLoginHander);
		}
	}
	else
	{
		return;
	}
}

TArray<FROLEINFO> ULoginManager::GetRoleInfos()
{
	return roleInfoDatas;
}

bool ULoginManager::GetRoleInfoByDBid(const FString& dbid, FROLEINFO& OutRoleInfo)
{
	for (auto tempRoleInfo : roleInfoDatas)
	{
		if (tempRoleInfo.DBid == dbid)
		{
			OutRoleInfo = tempRoleInfo;
			return true;
		}
	}
	return false;
}

void ULoginManager::EnterCreateRole()
{
	UClientGameInstance::Instance->GameStatusMgr->ChangeToNewStatus(EGameStatus::CreateRoleLoading);
}

void ULoginManager::SetRoleInfos(const FROLEINFO_T& RoleInfos)
{
	roleInfoDatas = RoleInfos.infos;
}

void ULoginManager::AddRoleInfo(const FROLEINFO& RoleInfo)
{
	roleInfoDatas.Emplace(RoleInfo);
}

void ULoginManager::DeleteRoleInfo(uint64 dbid)
{
	for (auto tempRoleInfo: roleInfoDatas)
	{
		if (FSTRING_TO_UINT64(tempRoleInfo.DBid) == dbid)
		{
			roleInfoDatas.Remove(tempRoleInfo);
			return;
		}
	}
}
#pragma optimize("", on)