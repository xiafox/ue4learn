#include "ClientEntity.h"
#include "ClientTypes/VariantExt.h"
#include "Interface/EntityInterfaceObject.h"

KBE_BEGIN_ENTITY_METHOD_MAP(ClientEntity, Super)
KBE_END_ENTITY_METHOD_MAP()

KBE_BEGIN_ENTITY_PROPERTY_MAP(ClientEntity, Super)
KBE_END_ENTITY_PROPERTY_MAP()

ClientEntity::ClientEntity()
{
}

ClientEntity::~ClientEntity()
{
}

void ClientEntity::InitProperties(KBEngine::ScriptModule& scriptModule)
{
	Super::InitProperties(scriptModule);
	UEntityInterfaceObject::LoadInterface(InterfaceObjects, this);
	RegisterRemoteMethod();
	RegisterUpdateProperty();
}

void ClientEntity::OnEnterScenes()
{
	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		It.Value()->OnEnterScenes();
	}
}

void ClientEntity::OnEnterWorld()
{
	Super::OnEnterWorld();

	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		It.Value()->onEnterWorld();
	}
}

void ClientEntity::OnLeaveWorld()
{
	UnBindInterfaceToEntity();

	Super::OnLeaveWorld();	
}

void ClientEntity::OnDestroy()
{
	Super::OnDestroy();
}

void ClientEntity::RegisterRemoteMethod()
{
	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		UEntityInterfaceObject* obj_ = It.Value();
		TArray<FString> methods;
		obj_->GetMethodMap(methods);
		for (auto Name : methods)
		{
			checkf(!InterfaceFuncObj.Contains(Name), TEXT("Repeat define method(%s)!!"), *Name);
			InterfaceFuncObj.Add(Name, obj_);
		}
	}
}


void ClientEntity::RegisterUpdateProperty()
{
	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		UEntityInterfaceObject* obj_ = It.Value();
		TArray<FString> propertys;
		obj_->GetPropertyMap(propertys);
		for (auto Name : propertys)
		{
			checkf(!InterfaceProObj.Contains(Name), TEXT("Repeat define property(%s)"), *Name);
			InterfaceProObj.Add(Name, obj_);
		}
	}
}

void ClientEntity::OnRemoteMethodCall(const FString &name, const TArray<FVariant> &args)
{
	if (InterfaceFuncObj.Contains(name))
	{
		FString ParamStr(name);
		for (FVariant Fv_ : args)
		{
			ParamStr += TEXT(" ") + GetStringFromVariant(Fv_);
		}
		InterfaceFuncObj[name]->OnRemoteMethodCall(name, ParamStr);

	}
}

void ClientEntity::OnUpdateProperty(const FString &name, const FVariant &newVal, const FVariant &oldVal)
{
	Super::OnUpdateProperty(name, newVal, oldVal);
	if (InterfaceProObj.Contains(name))
	{
		FString PropertyName(TEXT("OnRep_") + name);
		FString ParamStr(PropertyName);
		ParamStr += TEXT(" ") + GetStringFromVariant(newVal) + TEXT(" ") + GetStringFromVariant(oldVal);

		InterfaceProObj[name]->OnUpdateProperty(PropertyName, newVal, oldVal, ParamStr);
	}

}

void ClientEntity::UnBindInterfaceToEntity()
{
	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		It.Value()->onLeaveWorld();
		It.Value()->RemoveFromRoot();
	}
	InterfaceObjects.Reset();
}

template<class T>
inline T* ClientEntity::GetInterfaceObject(const FString &name)
{
	UEntityInterfaceObject* interfaceObj = nullptr;
	for (auto It = InterfaceObjects.CreateConstIterator(); It; ++It)
	{
		if (It.Key().Equals(name))
		{
			interfaceObj = It.Value();
			break;
		}
	}
	T* TObj = StaticCast<T*>(interfaceObj);
	return TObj;
} 

bool ClientEntity::IsPlayerRole()
{
	if (IsPlayer() && ClassName() == TEXT("Role"))
	{
		return true;
	}
	return false;
}

bool ClientEntity::IsPlayerAccount()
{
	if (IsPlayer() && ClassName() == TEXT("Account"))
	{
		return true;
	}
	return false;
}
