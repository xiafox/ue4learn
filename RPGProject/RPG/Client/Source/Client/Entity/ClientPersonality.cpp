#include "ClientPersonality.h"
#include "ClientGameInstance.h"
#include "Manager/LoginManager.h"


void ClientPersonality::OnSetSpaceData(uint32 spaceID, const FString &key, const FString &value)
{
	if (key == "SPACE_SCRIPT_ID")
	{
		UClientGameInstance::Instance->GameStatusMgr->OnSetSpaceData(spaceID, key, value);
	}
}

void ClientPersonality::OnDelSpaceData(uint32 spaceID, const FString &key)
{

}

void ClientPersonality::OnAddSpaceGeometryMapping(uint32 spaceID, const FString &respath)
{

}

void ClientPersonality::OnLoginFailed(int32 errCode, const FString& errName, const FString& errDesc, const TArray<uint8>& serverDatas)
{
	ULoginManager* LoginManager = UClientGameInstance::Instance->LoginManager;
	if (LoginManager != nullptr)
	{
		LoginManager->OnLoginFailed();
	}
}

void ClientPersonality::OnDisconnect()
{
	ULoginManager* LoginManager = UClientGameInstance::Instance->LoginManager;
	if (LoginManager != nullptr)
	{
		LoginManager->OnDisconnect();
	}
}

void ClientPersonality::OnReLoginBaseapp(int32 errCode, const FString& errName, const FString& errDesc)
{

}

void ClientPersonality::OnNewPassword(int32 errCode, const FString& errName, const FString& errDesc)
{

}

void ClientPersonality::OnBindAccountEmail(int32 errCode, const FString& errName, const FString& errDesc)
{

}

void ClientPersonality::OnCreateAccountSuccess(const FString& account)
{

}

void ClientPersonality::OnResetPasswordSuccess(const FString& account)
{

}
