#include "SceneDetect.h"
#include "ClientGameInstance.h"
#include "Manager/Level/LevelManager.h"

void USceneDetect::InitData(const FString& Params)
{
	TimeoutTime = 5.0f;
	CProgress = 0.0f;
	bIsLoadFinished = false;
	if (IsValid(UClientGameInstance::Instance))
	{
		if (!Params.Compare(TEXT("Login")))
		{
			UClientGameInstance::Instance->LevelManager->TeleportLoginLevel();
		}
		else if (!Params.Compare(TEXT("SelectRole")))
		{
			UClientGameInstance::Instance->LevelManager->TeleportSelectRoleLevel();
		}
		else if (!Params.Compare(TEXT("CreateRole")))
		{
			UClientGameInstance::Instance->LevelManager->TeleportCreateRoleLevel();
		}
		else if (!Params.Compare(TEXT("BackToSelectRole")))
		{
			UClientGameInstance::Instance->LevelManager->TeleportSelectRoleLevel();
		}
		else if (!Params.Compare(TEXT("World")))
		{
			UClientGameInstance::Instance->LevelManager->TeleportInWorldLevel();
		}
		else if (!Params.Compare(TEXT("Teleport")))
		{
			UClientGameInstance::Instance->LevelManager->TeleportInWorldLevel();
		}
	}
}

void USceneDetect::StartDetect(const FString& Params)
{
	UClientGameInstance::Instance->LevelManager->OnLoadFinished.AddUObject(this, &USceneDetect::OnLevelLoadFinished);
	Super::StartDetect(Params);
}

void USceneDetect::Detect()
{
	CProgress += 0.1f;
	if (CProgress >= 1.0f)
	{
		if (bIsLoadFinished)
		{
			FinishDetect();
			return;
		}
		CProgress = 0.99f;
	}
}

void USceneDetect::CancelDetect()
{
	UClientGameInstance::Instance->LevelManager->OnLoadFinished.RemoveAll(this);
}

void USceneDetect::OnLevelLoadFinished()
{
	bIsLoadFinished = true;
	FinishDetect();
	UClientGameInstance::Instance->LevelManager->OnLoadFinished.RemoveAll(this);
}
