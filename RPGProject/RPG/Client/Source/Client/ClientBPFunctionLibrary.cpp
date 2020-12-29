// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientBPFunctionLibrary.h"
#include "ClientGameInstance.h"
#include "Entity/ServerGameEntity.h"
#include "Character/ServerCharacter.h"
#include "Entity.h"
#include "Manager/Cfg/CfgManager.h"
#include "Manager/Cfg/ConfigData/StrToClass.h"


class AServerCharacter* UClientBPFunctionLibrary::GetPlayerCharacter()
{
	if (UClientGameInstance::KBEComponents->pKBEApp == nullptr)
	{
		return nullptr;
	}
	if (UClientGameInstance::KBEComponents->pKBEApp->Player())
	{
		KBEngine::Entity* Player = UClientGameInstance::KBEComponents->pKBEApp->Player();
		if (Player && Player->IsPlayer())
		{
			AActor* Player_Actor = Player->Actor();
			if (Player_Actor != nullptr && IsValid(Player_Actor))
			{
				AServerCharacter* PlayerCharacter = dynamic_cast<AServerCharacter*>(Player_Actor);
				if (PlayerCharacter != nullptr && IsValid(PlayerCharacter))
				{
					return PlayerCharacter;
				}
			}
		}
	}
	AServerCharacter* ServerCharacter = Cast<AServerCharacter>(UGameplayStatics::GetPlayerCharacter(UClientGameInstance::Instance->GetWorld(), 0));
	if (ServerCharacter)
	{
		return ServerCharacter;
	}
	else
	{
		return nullptr;
	}
}

KBEngine::Entity*  UClientBPFunctionLibrary::GetPlayer()
{
	if (UClientGameInstance::KBEComponents->pKBEApp == nullptr)
	{
		return nullptr;
	}

	return UClientGameInstance::KBEComponents->pKBEApp->Player();
}

int UClientBPFunctionLibrary::GetPlayerID()
{
	if (UClientGameInstance::KBEComponents->pKBEApp== nullptr)
	{
		return 0;
	}
	if (UClientGameInstance::KBEComponents->pKBEApp->Player())
	{
		return UClientGameInstance::KBEComponents->pKBEApp->Player()->ID();
	}
	return 0;

}

UEntityInterfaceObject* UClientBPFunctionLibrary::GetInterfaceByName(const FString& Name, int32 id /*= 0*/)
{
	if (KBEngine::KBEngineApp::app != nullptr)
	{
		KBEngine::Entity* entity;
		if (id == 0)
		{
			entity = KBEngine::KBEngineApp::app->Player();
		}
		else
		{
			entity = KBEngine::KBEngineApp::app->FindEntity(id);
		}
		ClientEntity* cEntity = static_cast<ClientEntity*>(entity);
		if (cEntity)
		{
			return cEntity->GetInterfaceObject<UEntityInterfaceObject>(Name);
		}
	}
	return nullptr;
}

FString UClientBPFunctionLibrary::GetStringPropertyValue(const int32& EntityID, const FString& PropName)
{
	if (KBEngine::KBEngineApp::app)
	{
		KBEngine::Entity* entity = KBEngine::KBEngineApp::app->FindEntity(EntityID);
		if (entity)
		{
			return entity->GetDefinedProperty(PropName);
		}
	}
	return TEXT("");
}

int32 UClientBPFunctionLibrary::GetIntPropertyValue(const int32& EntityID, const FString& PropName)
{
	if (KBEngine::KBEngineApp::app)
	{
		KBEngine::Entity* entity = KBEngine::KBEngineApp::app->FindEntity(EntityID);
		if (entity)
		{
			FVariant propvalue = entity->GetDefinedProperty(PropName);
			switch (propvalue.GetType())
			{
			case EVariantTypes::UInt8:
			{
				return int32(propvalue.GetValue<uint8>());
			}
			case EVariantTypes::UInt16:
			{
				return int32(propvalue.GetValue<uint16>());
			}
			case EVariantTypes::UInt32:
			{
				uint32 value = propvalue.GetValue<uint32>();
				//判断value > 2147483647，就崩溃
				check(value < 2147483648);
				return int32(propvalue.GetValue<uint32>());
			}
			case EVariantTypes::UInt64:
			{
				return int32(propvalue.GetValue<uint64>());
			}
			case EVariantTypes::Int8:
			{
				return int32(propvalue.GetValue<int8>());
			}
			case EVariantTypes::Int16:
			{
				return int32(propvalue.GetValue<int16>());
			}
			case EVariantTypes::Int32:
			{
				return int32(propvalue.GetValue<int32>());
			}
			default:
				check(false);
				break;
			}
		}
	}
	return 0;
}

float UClientBPFunctionLibrary::GetFloatPropertyValue(const int32& EntityID, const FString& PropName)
{
	if (KBEngine::KBEngineApp::app)
	{
		KBEngine::Entity* entity = KBEngine::KBEngineApp::app->FindEntity(EntityID);
		if (entity)
		{
			FVariant propvalue = entity->GetDefinedProperty(PropName);
			switch (propvalue.GetType())
			{
			case EVariantTypes::Float:
			{
				return propvalue.GetValue<float>();
			}
			default:
				check(false);
				break;
			}
		}
	}
	return 0.0;
}

class AActor* UClientBPFunctionLibrary::BeginDeferredActorSpawnFromClass(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride /*= ESpawnActorCollisionHandlingMethod::AlwaysSpawn*/, AActor* Owner /*= nullptr*/)
{
	const UWorld* Worldcontext = UClientGameInstance::Instance->GetWorld();
	return UGameplayStatics::BeginDeferredActorSpawnFromClass(Worldcontext, ActorClass, SpawnTransform, CollisionHandlingOverride, Owner);
}

class AActor* UClientBPFunctionLibrary::FinishSpawningActor(AActor* Actor, const FTransform& SpawnTransform)
{
	return UGameplayStatics::FinishSpawningActor(Actor, SpawnTransform);
}

TArray<FString> UClientBPFunctionLibrary::SplitString(const FString& InString, bool IsElemUnique /*= false*/, FString SplitChar /*= TEXT("|")*/)
{
	TArray<FString> SplitArray;
	FString SplitStr(InString);
	FString Left, Right;
	bool Result = true;
	while (!SplitStr.IsEmpty())
	{
		Result = SplitStr.Split(SplitChar, &Left, &Right, ESearchCase::Type::CaseSensitive);
		if (Result)
		{
			SplitStr = Right;
			if (IsElemUnique)
				SplitArray.AddUnique(Left);
			else
				SplitArray.Add(Left);
		}
		else
		{
			if (IsElemUnique)
				SplitArray.AddUnique(SplitStr);
			else
				SplitArray.Add(SplitStr);
			SplitStr = TEXT("");
		}
	}
	return SplitArray;
}

FString UClientBPFunctionLibrary::TypeValidation(const FString InString)
{
	if (InString == "")
	{
		//CS3_DEBUG(TEXT("-----------------类型转换错误，传入参数为空--------------------%s"), *InString);
		return InString;
	}
	FString point = ".+-";
	FString number = "0123456789";

	FString CopyString = InString;

	if (CopyString[0] == point[1] || CopyString[0] == point[2])
	{
		CopyString.RemoveAt(0, 1);
	}

	int32 count = 0;
	int32 pointCount = 0;
	for (auto element : CopyString)
	{
		for (int i = 0; i < number.Len(); i++)
		{
			if (element == number[i] || element == point[0])
			{
				count = 0;
				if (element == point[0])
				{
					pointCount++;
				}
				break;
			}
			else count++;
		}

		if (count == number.Len())
		{
			//CS3_DEBUG(TEXT("-----------------类型转换错误，有不是小数点和数字的字符--------------------%s"), *InString);
			return InString;
		}
	}

	if (pointCount > 1)
	{
		//CS3_DEBUG(TEXT("-----------------类型转换错误，有两个以上小数点--------------------%s"), *InString);
		return InString;
	}
	return InString;
}

UClass* UClientBPFunctionLibrary::GetUClassByStr(const FString InString)
{
	if (!IsValid(UClientGameInstance::Instance)&&!IsValid(UClientGameInstance::Instance->CfgManager)) return nullptr;
	const UConfigTable* table = UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_STRTOCLASS);
	FSTR_TO_CLASS_DATA* data = const_cast<FSTR_TO_CLASS_DATA*>(table->GetRow<FSTR_TO_CLASS_DATA>(InString));
	if (data == nullptr)
	{
		//CS3_ERROR(TEXT("-->Null Pointer error:UClientBPFunctionLibrary::GetUClassByStr : data!"));
		check(0);
		return nullptr;
	}
	return data->GetTargetClass();
}
