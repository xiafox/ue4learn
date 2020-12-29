// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FilterActorComponent.h"
#include "KBEngine.h"
#include "AvatarFilterActorComponent.generated.h"

static const float STOP_MOVE_DIST = 10.0f;		//厘米
static const float STOP_ROTATOR_ANGLE = 10.0f;	//度

struct StoredInput
{
	double		time_;
	int32		parentID_;
	FVector		position_;
	FVector		direction_;
public:
	void ResetData() 
	{
		this->direction_ = FVector::ZeroVector;
		this->parentID_ = 0;
		this->position_ = FVector::ZeroVector;
		this->time_ = 0.0f;
	}

	bool IsValidData()
	{
		if (this->direction_ == FVector::ZeroVector && this->position_ == FVector::ZeroVector &&FMath::IsNearlyZero(this->time_))
		{
			return false;
		}
		return true;
	}
};

/**
 * 一个听从服务器指挥，在一定的时间忠实的插值移动到服务器指定的位置，常用于服务器更新其它玩家、怪物的坐标及朝向
 */
UCLASS( )
class CLIENT_API UAvatarFilterActorComponent : public UFilterActorComponent
{
	GENERATED_BODY()

		typedef UFilterActorComponent Supper;

public:
	UAvatarFilterActorComponent();

	/// Called when the game starts
	virtual void BeginPlay() override;

	/// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnUpdateVolatileData(const FVector& position, const FVector& direction, int32 parentID) override;
	virtual void OnUpdateVolatileDataByParent(const FVector& position, const FVector& direction, int32 parentID) override;
	virtual void SetPosition(const FVector& position, int32 parentID) override;
	virtual void SetDirection(const FVector& direction, int32 parentID) override;
	virtual void OnGotParentEntity(KBEngine::Entity* parentEnt) override;
	virtual void OnLoseParentEntity() override;
	virtual void SetPosBySerPos(const int32 vehicleID, const FVector& position, const FVector& direction);

protected:
	virtual void UpdateCharacterPositionAndDirection(float DeltaTime);

protected:
	StoredInput	StoredInputs;
	bool bCanFilterTick = true;
	bool IsNeedToUpdate = false;
};
