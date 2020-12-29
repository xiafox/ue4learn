#include "TeleportInWorldLoadingBaseStatus.h"
#include "../../ClientGameInstance.h"
#include "KBEMain.h"
#include "Entity.h"
#include "Entity/ClientEntity.h"


void UTeleportInWorldLoadingBaseStatus::OnEnter(EGameStatus LastStatus)
{
	WaitingEnterScenes.Empty();
	Super::OnEnter(LastStatus);
}

void UTeleportInWorldLoadingBaseStatus::OnLeave()
{
	UClientGameInstance::Instance->GetTimerManager().ClearTimer(LoaderTimerHander);
	WaitingEnterScenes.Empty();
	Super::OnLeave();
}

void UTeleportInWorldLoadingBaseStatus::OnLevelLoadFinished()
{
	Super::OnLevelLoadFinished();

	if (UClientGameInstance::KBEComponents && UClientGameInstance::KBEComponents->pKBEApp->pBaseApp())
	{
		TMap<int32, KBEngine::Entity*> EntitiesMap = *(UClientGameInstance::KBEComponents->pKBEApp->pBaseApp()->Entities());

		for (auto& Elem : EntitiesMap)
		{
			if (Elem.Value->InWorld())
			{
				WaitingEnterScenes.Add(Elem.Value);
			}
		}
		KBEngine::Entity* Player = UClientGameInstance::KBEComponents->pKBEApp->pBaseApp()->Player();
		if (Player)
		{
			//将要进入的对象排序
			WaitingEnterScenes.Sort([Player](KBEngine::Entity& First, KBEngine::Entity& Second)
			{
				return(FVector::Dist(First.Position(), Player->Position()) > FVector::Dist(Second.Position(), Player->Position()));
			});
		}

		UClientGameInstance::Instance->GetTimerManager().SetTimer(LoaderTimerHander, this, &UTeleportInWorldLoadingBaseStatus::OnTimerEnterScenes, 0.02f, true);
	}
}

void UTeleportInWorldLoadingBaseStatus::OnTimerEnterScenes()
{
	if (WaitingEnterScenes.Num() > 0)
	{
		KBEngine::Entity* Obj = WaitingEnterScenes.Pop();
		if (Obj->InWorld() && (ClientEntity*)Obj)
		{
			((ClientEntity*)Obj)->OnEnterScenes();
		}
	}
	else
	{
		if (LoaderTimerHander.IsValid())
		{
			UClientGameInstance::Instance->GetTimerManager().ClearTimer(LoaderTimerHander);
		}
	}
}

