// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClientObject.h"
#include "Entity.h"
#include "EntityInterfaceObject.generated.h"


/**
* 接口基类
*/
UCLASS(Blueprintable, BlueprintType)
class UEntityInterfaceObject : public UClientObject
{
	GENERATED_BODY()

public:
	UEntityInterfaceObject();
	UEntityInterfaceObject(int32 entityId);
	~UEntityInterfaceObject();
	/**
	*加载接口
	*
	*@param KBEEntities 所有继承UInterfaceObject的接口
	*@param name 接口名
	*@param type 接口类型
	*@param ent 接口Entity
	*@param entid 接口EntityID
	*
	*@return 无
	*/
	static void LoadInterface(TMap<FString, UEntityInterfaceObject *> &KBEEntities, KBEngine::Entity *ent);

	void BaseCall(const FString &methodname, const KBEngine::FVariantArray &arguments);
	void CellCall(const FString &methodname, const KBEngine::FVariantArray &arguments);

	virtual void onEnterWorld();///< 当Entity进入世界时，此方法被调用
	virtual void onLeaveWorld(); ///< 当Entity离开世界（被销毁时）时，此方法被调用
	virtual void OnEnterScenes() {};///<开始场景

	virtual void GetMethodMap(TArray<FString>& Methods) {};
	virtual void GetPropertyMap(TArray<FString>& Propertys) {};
	
	void OnRemoteMethodCall(const FString& Method, const FString& Params);
	void OnUpdateProperty(const FString& Property, const FVariant& newVal, const FVariant& oldVal, const FString& Params);

	UFUNCTION(BlueprintImplementableEvent, Category = "EntityInterfaceObject")
		void BP_onEnterWorld();///<BlueprintImplementableEvent， 当Entity进入世界时，此方法被调用
	UFUNCTION(BlueprintImplementableEvent, Category = "EntityInterfaceObject")
		void BP_onLeaveWorld();///< BlueprintImplementableEvent，当Entity离开世界（被销毁时）时，此方法被调用

	KBEngine::Entity *GetEntity();///<获取Entity
	UFUNCTION(BlueprintCallable, Category = "EntityInterfaceObject")
		AActor *GetActor();///<获取Actor
	void SetActor(const AActor *Actor);///<设置OwnerActor


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EntityInterfaceObject")
		int32 EntityID;///<EntityID
private:
	UPROPERTY()
		AActor *OwnerActor;///<OwnerActor
};


