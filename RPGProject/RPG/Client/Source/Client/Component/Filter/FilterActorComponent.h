// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../ClientActorComponent.h"
#include "KBEngine.h"
#include "FilterActorComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLIENT_API UFilterActorComponent : public UClientActorComponent
{
	GENERATED_BODY()

public:	
	UFilterActorComponent();
	//~ Begin UActorComponent Interface.
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface.

	virtual void OnUpdateVolatileData(const FVector& position, const FVector& direction, int32 parentID) { }
	virtual void OnUpdateVolatileDataByParent(const FVector& position, const FVector& direction, int32 parentID) { }
	virtual void SetPosition(const FVector& position, int32 parentID) { }
	virtual void SetDirection(const FVector& direction, int32 parentID) { }
	virtual void SpeedChangedNotify(float speed) { }
	virtual void OnGotParentEntity(KBEngine::Entity* parentEnt) { }
	virtual void OnLoseParentEntity() { }


	void uTick(bool b) { canTick_ = b; }
	bool uTick() { return canTick_; }
private:
	bool canTick_ = true;
};
