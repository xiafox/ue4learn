#include "EntityInterfaceObject.h"
#include "ClientGameInstance.h"
#include "Manager/Cfg/CfgManager.h"
#include "Manager/Cfg/ConfigData/EntityInterfaceDefine.h"

UEntityInterfaceObject::UEntityInterfaceObject()
{
	EntityID = 0;
	OwnerActor = nullptr;
}

UEntityInterfaceObject::UEntityInterfaceObject(int32 entityId) :EntityID(entityId)
{
	OwnerActor = nullptr;
}

UEntityInterfaceObject::~UEntityInterfaceObject()
{

}

void UEntityInterfaceObject::onEnterWorld()
{
	BP_onEnterWorld();
}

void UEntityInterfaceObject::onLeaveWorld()
{
	BP_onLeaveWorld();
}

void UEntityInterfaceObject::OnRemoteMethodCall(const FString& Method, const FString& Params)
{
	const FName Message = FName(*Method, FNAME_Find);
	if (Message.IsNone())
	{
		return;
	}

	UFunction* Function = FindFunction(Message);
	if (NULL == Function)
	{
		return;
	}

	FOutputDeviceNull ar;
	CallFunctionByNameWithArguments(*Params, ar, NULL, true);
}

void UEntityInterfaceObject::OnUpdateProperty(const FString& Property, const FVariant& newVal, const FVariant& oldVal, const FString& Params)
{
	auto SetProperty_Fun = [this](const TCHAR* PropertyName, const FVariant& Val) {
		switch (Val.GetType())
		{
		case EVariantTypes::Vector:
			break;
		case EVariantTypes::Float:
		{
			float value = Val.GetValue<float>();

			UFloatProperty *Prop = FindField<UFloatProperty>(GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, value);
			}
			break;
		}
		case EVariantTypes::UInt8:
		{
			int value = Val.GetValue<uint8>();

			UByteProperty *Prop = FindField<UByteProperty>(GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, value);
			}
			break;
		}
		case EVariantTypes::UInt16:
		{
			int value = Val.GetValue<uint16>();

			UIntProperty *Prop = FindField<UIntProperty>(this->GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, value);
			}

			break;
		}
		case EVariantTypes::UInt32:
		{
			int value = Val.GetValue<uint32>();

			UIntProperty *Prop = FindField<UIntProperty>(this->GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, value);
			}

			break;
		}
		case EVariantTypes::Int8:
		{
			int value = Val.GetValue<int8>();

			UByteProperty *Prop = FindField<UByteProperty>(this->GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, value);
			}

			break;
		}
		case EVariantTypes::Int16:
		{
			int value = Val.GetValue<int16>();

			UIntProperty *Prop = FindField<UIntProperty>(this->GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, value);
			}

			break;
		}
		case EVariantTypes::Int32:
		{
			int value = Val.GetValue<int32>();

			UIntProperty *Prop = FindField<UIntProperty>(this->GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, value);
			}

			break;
		}
		case EVariantTypes::String:
		{
			UStrProperty *Prop = FindField<UStrProperty>(this->GetClass(), PropertyName);
			if (Prop != NULL)
			{
				Prop->SetPropertyValue_InContainer(this, *Val.GetValue<FString>());
			}

			break;
		}
		}
	};

	SetProperty_Fun(*Property, newVal);
	OnRemoteMethodCall(Property, Params);
}

void UEntityInterfaceObject::LoadInterface(TMap<FString, UEntityInterfaceObject *> &KBEEntities, KBEngine::Entity *ent)
{
	///<读配置表创建Interface
	const UConfigTable* InterfaceCfg = UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_ENTITY_INTERFACE_DEFINE);
	if (InterfaceCfg && ent)
	{
		 FINTERFACE_DEFINE * InterfaceDefine = const_cast<FINTERFACE_DEFINE*>(InterfaceCfg->GetRow<FINTERFACE_DEFINE>(ent->ClassName()));

		if (InterfaceDefine && InterfaceDefine->InterfaceList.Num() > 0)
		{
			for (const FInterfaceInfos InterfaceInfo : InterfaceDefine->InterfaceList)
			{
				if (InterfaceInfo.ClassEntity == nullptr || InterfaceInfo.ActorPropertyName.IsEmpty())
				{
					continue;
				}
				UClass* InterfaceClass= InterfaceDefine->GetInterfaceInfosClass(InterfaceInfo);
				if (InterfaceClass)
				{
					UEntityInterfaceObject* InterfaceObject = NewObject<UEntityInterfaceObject>((UObject*)GetTransientPackage(),InterfaceClass);
					InterfaceObject->EntityID = ent->ID();
					InterfaceObject->AddToRoot();
					if (KBEEntities.Find(InterfaceInfo.ClassEntity->GetName()) != nullptr)
					{
						KBEEntities[InterfaceInfo.ClassEntity->GetName()]->RemoveFromRoot();
						delete KBEEntities[InterfaceInfo.ClassEntity->GetName()];
					}

					KBEEntities.Add(InterfaceInfo.ActorPropertyName, InterfaceObject);
				}
			}
		}
	}
}

void UEntityInterfaceObject::BaseCall(const FString &methodname, const KBEngine::FVariantArray &arguments)
{
	KBEngine::Entity* entity_ = GetEntity();
	if (entity_)
	{
		entity_->BaseCall(methodname, arguments);
	}
}

void UEntityInterfaceObject::CellCall(const FString &methodname, const KBEngine::FVariantArray &arguments)
{
	KBEngine::Entity* entity_ = GetEntity();
	if (entity_)
	{
		entity_->CellCall(methodname, arguments);
	}
}

KBEngine::Entity * UEntityInterfaceObject::GetEntity()
{
	if (KBEngine::KBEngineApp::app)
	{
		return KBEngine::KBEngineApp::app->FindEntity(EntityID);
	}
	///<获取Entity
	return nullptr;
}

AActor * UEntityInterfaceObject::GetActor()
{
	///<当前还需要判断Actor是否销毁状态（这个ServerCharacter还没有封装）
	if (GetEntity() != nullptr && OwnerActor != nullptr && IsValid(OwnerActor))
	{
		return OwnerActor;
	}
	return nullptr;
}

void UEntityInterfaceObject::SetActor(const AActor *Actor)
{
	OwnerActor = (AActor *)Actor;
}

