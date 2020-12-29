// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../ClientTableRowBase.h"
#include "Engine/StreamableManager.h"
#include "EntityInterfaceDefine.generated.h"


/**
* 
*/
USTRUCT(BlueprintType)
struct FInterfaceInfos
{
	GENERATED_BODY()

public:
	//接口的UClass属性类
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UEntityInterfaceObject> ClassEntity;

	//对应接口的名称
	UPROPERTY(EditAnywhere)
		FString ActorPropertyName;

};

/**
*entity的接口配置结构体
*功能：用于配置各类entity所具体的接口
*/
USTRUCT(BlueprintType)
struct FINTERFACE_DEFINE : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray<FInterfaceInfos> InterfaceList;

	UClass* GetInterfaceInfosClass(FInterfaceInfos InterfaceInfo)
	{
		UClass* targetClass = InterfaceInfo.ClassEntity.Get();
		if (targetClass == nullptr)
		{
			FStreamableManager StreamManager;
			return StreamManager.LoadSynchronous(InterfaceInfo.ClassEntity);
			
		}
		return targetClass;
	}
};
