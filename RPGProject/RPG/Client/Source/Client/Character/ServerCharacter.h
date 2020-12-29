// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GameCharacter.h"
#include "Entity.h"
#include "ClientTypes/ModelDefine.h"
#include "ServerCharacter.generated.h"


/**
 * 
 */

UCLASS()
class CLIENT_API AServerCharacter : public AGameCharacter
{
	GENERATED_BODY()
public:
	AServerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Tick(float DeltaSeconds) override;
	//-----------------Begin InitComponent
	virtual void InitCharacterDefaultComponent()override;
	virtual void InitCharacterComponentPostActorCreated()override;
	virtual void InitCharacterComponentPostEntityBinded();///<创建于entity相关的组件
	virtual void InitFilterActorComponent();
	//-----------------End InitComponent

	//-----------------Begin Entity	
	virtual void BindActorToEntity();

	virtual void UnBindActorToEntity();///<解除绑定的操作

	virtual void BindLinkEntityID(int32 ID);
	KBEngine::Entity* GetSelfEntity();///<获得Actor绑定的Entity
	UFUNCTION(BlueprintPure, Category = "ServerCharacter")
		bool ActorIsPlayer();

	//-----------------End Entity

	//-----------------Begin Status
	bool GetIsInitialized() { return bIsInitialized; };

	//-----------------End Status


	virtual void onUpdateVolatileData(const FVector& position, const FVector& direction);
	///设置FilterActorComponent
	FORCEINLINE void SetFilterComponent(class UFilterActorComponent* _comp)
	{
		MFilterComponent = _comp;
	}

	FORCEINLINE  class UFilterActorComponent* GetFilterComponent()
	{
		return MFilterComponent;
	}

	/**
	 * Called from CharacterMovementComponent to notify the character that the movement mode has changed.
	 * @param	PrevMovementMode	Movement mode before the change
	 * @param	PrevCustomMode		Custom mode before the change (applicable if PrevMovementMode is Custom)
	 */
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviCustomMode = 0);

	virtual void OnStateChange(const int32& NewState, const int32& OldState);

	virtual void OnEffectStateChange(const int32& NewEffect, const int32& OldEffect);

	virtual void CreateAppearance() override;
	virtual void CreateAppearance(const TMap<MODEL_PART_TYPE, FString>& ModelParts);

	virtual void ChangeModelPart(const MODEL_PART_TYPE& ModelPart, const FString& ModelID);

	virtual void SetModelScale(const float& ModelScale);

protected:
	//-----------------Begin Entity
	virtual void PreUnBindActorToEntity();///<在解除绑定之前的事件
	//-----------------End Entity

private:
	void DeferredDestroy();///<延时销毁
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 EntityId = 0;///<EntityId

protected:
	class UFilterActorComponent* MFilterComponent = nullptr;

private:
	bool bIsInitialized = false;
};
