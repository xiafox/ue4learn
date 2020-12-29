// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KBEngine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ClientBPFunctionLibrary.generated.h"


class UEntityInterfaceObject;

/**
 * 
 */
UCLASS()
class CLIENT_API UClientBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/*----------------角色相关-----------------------*/
		///<得到人物
	UFUNCTION(BlueprintPure, Category = "Golbal")
		static class AServerCharacter* GetPlayerCharacter();

	///<得到人物entity
	static  KBEngine::Entity* GetPlayer();

	///<获取玩家ID
	UFUNCTION(BlueprintPure)
		static int GetPlayerID();

	///<获取interface
	UFUNCTION(BlueprintPure)
		static UEntityInterfaceObject* GetInterfaceByName(const FString& Name, int32 id = 0);

	
	UFUNCTION(BluePrintPure)
		static FString GetStringPropertyValue(const int32& EntityID, const FString& PropName);

	UFUNCTION(BluePrintPure)
		static int32 GetIntPropertyValue(const int32& EntityID, const FString& PropName);

	UFUNCTION(BluePrintPure)
		static float GetFloatPropertyValue(const int32& EntityID, const FString& PropName);

	/*----------------UE4 SpawnActor-----------------------*/
	UFUNCTION(BlueprintCallable)
		static class AActor* BeginDeferredActorSpawnFromClass(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn, AActor* Owner = nullptr);
	
	UFUNCTION(BlueprintCallable)
		static class AActor* FinishSpawningActor(AActor* Actor, const FTransform& SpawnTransform);

	/*----------------字符串功能相关-----------------------*/

	/**
	*BlueprintCallable,FString字符串拆分
	*
	*@param	InString 需要拆分字符串
	*@param	IsElemUnique 是否保持分割出来数组元素唯一
	*@param	SplitChar 分隔符号
	*
	*@return TArray<FString> 引擎提供类似方法ParseIntoArray
	*/
	UFUNCTION(BlueprintCallable, Category = "FSrting|Split")
		static TArray<FString> SplitString(const FString& InString, bool IsElemUnique = false, FString SplitChar = TEXT("|"));

	///<类型
	static FString TypeValidation(const FString InString);

	static UClass* GetUClassByStr(const FString InString);
};
