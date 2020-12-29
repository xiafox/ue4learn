// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../AvatarFilterActorComponent.h"
#include "RoleFilterActorComponent.generated.h"


/**
 * 一个听从服务器指挥，在一定的时间忠实的插值移动到服务器指定的位置，常用于服务器更新其它玩家、怪物的坐标及朝向
 */
UCLASS( )
class CLIENT_API URoleFilterActorComponent : public UAvatarFilterActorComponent
{
	GENERATED_BODY()
		typedef UAvatarFilterActorComponent Supper;

public:
	URoleFilterActorComponent();

	/// Called when the game starts
	virtual void BeginPlay() override;

	/// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnUpdateVolatileData(const FVector& position, const FVector& direction, int32 parentID) override;
	virtual void OnUpdateVolatileDataByParent(const FVector& position, const FVector& direction, int32 parentID) override;
	virtual void SetPosition(const FVector& position, int32 parentID) override;
	virtual void SetDirection(const FVector& direction, int32 parentID) override;
	
private:
	virtual void UpdateCharacterPositionAndDirection(float DeltaTime)override;
	float sameSignDegree(float degree, float closer);
	FVector sameSignDegree(const FVector& degree, const FVector& closer);
private:
	bool IsReceiveNewPositionOrDirection = false;	 
	float TotalTime = 0.0f;
	FRotator LastRotation = FRotator::ZeroRotator;
};
