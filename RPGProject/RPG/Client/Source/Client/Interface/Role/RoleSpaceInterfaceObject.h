// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/EntityInterfaceObject.h"
#include "RoleSpaceInterfaceObject.generated.h"

/**
 *
 */
UCLASS()
class CLIENT_API URoleSpaceInterfaceObject : public UEntityInterfaceObject
{
	GENERATED_BODY()

		typedef UEntityInterfaceObject Super;

public:
	/**
	*Define method
	* BlueprintNativeEvent，服务器玩家进入空间回调通知
	*
	*@param SpaceScriptID 空间脚本ID
	*@param SpaceType 空间类型
	*
	*@return 无
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "RoleSpaceInterfaceObject")
		void CLIENT_OnEnterSpaceNotify(const FString& SpaceScriptID, const INT32 SpaceType);

	/**
	*Define method
	* BlueprintNativeEvent，服务器玩家离开空间回调通知
	*
	*@return 无
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "RoleSpaceInterfaceObject")
		void CLIENT_OnLeaveSpaceNotify();

	/**
	*Exposed method
	*BlueprintCallable，玩家请求进入某个空间
	*
	*@param SpaceScriptID 空间脚本ID
	*@param Position 位置
	*@param Direction 朝向
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void CELL_GoToSpace(FString SpaceScriptID, FVector Position, FVector Direction);

	virtual void GetMethodMap(TArray<FString>& Methods) override;
	virtual void GetPropertyMap(TArray<FString>& Propertys) override;

};
