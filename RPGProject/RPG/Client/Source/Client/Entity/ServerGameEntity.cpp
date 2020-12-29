
#include "ServerGameEntity.h"
#include "Character/ServerCharacter.h"
#include "ClientGameInstance.h"
#include "Interface/EntityInterfaceObject.h"
#include "Manager/Cfg/CfgManager.h"
#include "Manager/Cfg/ConfigData/StrToClass.h"
#include "Entity/Alias/ModelInfo.h"
#include "Interface/ModelInterfaceObject.h"

KBE_BEGIN_ENTITY_METHOD_MAP(ServerGameEntity, Supper)
KBE_END_ENTITY_METHOD_MAP()

KBE_BEGIN_ENTITY_PROPERTY_MAP(ServerGameEntity, Supper)
DECLARE_PROPERTY_CHANGED_NOTIFY(modelParts, &ServerGameEntity::OnRep_modelParts, KBEngine::FVariantMap)
DECLARE_PROPERTY_CHANGED_NOTIFY(modelExtParts, &ServerGameEntity::OnRep_modelExtParts, KBEngine::FVariantMap)
KBE_END_ENTITY_PROPERTY_MAP()

ServerGameEntity::ServerGameEntity()
{
}

ServerGameEntity::~ServerGameEntity()
{

}

void ServerGameEntity::OnRep_modelParts(const KBEngine::FVariantMap& newModelParts, const KBEngine::FVariantMap& oldModelParts)
{
	FMODEL_PART_MAPS newModel, oldModel;
	newModel.FromVariantMap(newModelParts);
	oldModel.FromVariantMap(oldModelParts);
	UModelInterfaceObject* obj_ = GetInterfaceObject<UModelInterfaceObject>(TEXT("ModelInterfaceObject"));
	if (obj_)
	{
		obj_->OnRep_modelParts(newModel.datas, oldModel.datas);
	}
}

void ServerGameEntity::OnRep_modelExtParts(const KBEngine::FVariantMap& newModelParts, const KBEngine::FVariantMap& oldModelParts)
{
	FMODEL_PART_MAPS newModel, oldModel;
	newModel.FromVariantMap(newModelParts);
	oldModel.FromVariantMap(oldModelParts);
	UModelInterfaceObject* obj_ = GetInterfaceObject<UModelInterfaceObject>(TEXT("ModelInterfaceObject"));
	if (obj_)
	{
		obj_->OnRep_modelExtParts(newModel.datas, oldModel.datas);
	}
}

void ServerGameEntity::OnEnterWorld()
{
	Supper::OnEnterWorld();
	if (UClientGameInstance::Instance && UClientGameInstance::Instance->GameStatusMgr && UClientGameInstance::Instance->GameStatusMgr->CheckLevelLoadFinished())
	{
		OnEnterScenes();
	}
}

void ServerGameEntity::OnLeaveWorld()
{
	UnBindActorToEntity();	
	Supper::OnLeaveWorld();
	UnBindActorToInterface();
}

void ServerGameEntity::OnEnterScenes()
{
	SpwawnServerCharacter();	
	PostSpwawnServerCharacter();
	Supper::OnEnterScenes();
}

void ServerGameEntity::Set_Position(const FVector &oldVal)
{

}

void ServerGameEntity::Set_Direction(const FVector &oldVal)
{

}

void ServerGameEntity::OnUpdateVolatileData()
{
	Supper::OnUpdateVolatileData();
	if (IsValid(Actor()))
	{
		AServerCharacter* ServerActor = Cast<AServerCharacter>(Actor());

		if (IsValid(ServerActor))
		{
			ServerActor->onUpdateVolatileData(LocalPosition(), LocalDirection());
		}
	}
}

void ServerGameEntity::BindActorToEntity(AServerCharacter* InServerCharacter)
{
	Actor(InServerCharacter);
}

void ServerGameEntity::PostBindActorToEntity()
{
	
}

void ServerGameEntity::UnBindActorToEntity()
{
	if (IsValid(Actor()))
	{
		AServerCharacter* ServerActor = Cast<AServerCharacter>(Actor());
		
		if (IsValid(ServerActor))
		{
			ServerActor->UnBindActorToEntity();			
			BindActorToEntity(nullptr);
		}
	}
	else //正常情况不会走到这里，处理异常情况
	{
		BindActorToEntity(nullptr);
	}
}

void ServerGameEntity::BindActorToInterface()
{
	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		It.Value()->SetActor(Actor());
	}
}

void ServerGameEntity::UnBindActorToInterface()
{
	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		It.Value()->SetActor(nullptr);
	}
}

void ServerGameEntity::SpwawnServerCharacter()
{
	UClass* ActorClass = GetEntityModelClassAsset();
	if (ActorClass)
	{
		FTransform SpawnTransform = FTransform();
		SpawnTransform.SetLocation(Position());
		SpawnTransform.SetRotation(FQuat::MakeFromEuler(Direction()));
		const UWorld* Worldcontext = UClientGameInstance::Instance->GetWorld();
		AServerCharacter* ServerCharacter = Cast<AServerCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(Worldcontext, ActorClass, SpawnTransform));
		if (ServerCharacter)
		{
			BindActorToEntity(ServerCharacter);
			ServerCharacter->BindLinkEntityID(ID()); 
			UGameplayStatics::FinishSpawningActor(ServerCharacter, SpawnTransform);

			ServerCharacter->BindActorToEntity();
			PostBindActorToEntity();
		}
	}
}

void ServerGameEntity::PostSpwawnServerCharacter()
{
	///<绑定Actor到Interface
	BindActorToInterface();
	///<处理缓存消息处理

	///<通知服务器客户端Character创建成功

	///<表现处理

}

UClass* ServerGameEntity::GetEntityModelClassAsset()
{
	const UConfigTable* ModelClassAssetCfg = UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_ENTITY_OF_ACTOR_CLASS_ASSET);
	if (ModelClassAssetCfg && this)
	{
		FSTR_TO_CLASS_DATA * ModelClassAsset = const_cast<FSTR_TO_CLASS_DATA*>(ModelClassAssetCfg->GetRow<FSTR_TO_CLASS_DATA>(this->ClassName()));
		if (ModelClassAsset)
		{
			return ModelClassAsset->GetTargetClass();
		}
	}
	return nullptr;
}

