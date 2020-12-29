// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ClientTypes/ActionDefine.h"
#include "Manager/Cfg/ConfigData/AnimConfigData.h"
#include "ClientAnimInstance.generated.h"



/**
 * 
 */
UCLASS()
class CLIENT_API UClientAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	/** Called to setup for updates */
	virtual void PreUpdateAnimation(float DeltaSeconds);

	void InitDatas();

	void OnStateChange(const int32& NewState, const int32& OldState);

	void OnEffectStateChange(const int32& NewEffect, const int32& OldEffect);

	/**
	 * Called from CharacterMovementComponent to notify the character that the movement mode has changed.
	 * @param	PrevMovementMode	Movement mode before the change
	 * @param	NewMovementMode		New movement mode
	 * @param	PrevCustomMode		Custom mode before the change (applicable if PrevMovementMode is Custom)
	 * @param	NewCustomMode		New custom mode (applicable if NewMovementMode is Custom)
	 */
	void OnMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode, uint8 PrevCustomMode, uint8 NewCustomMode);

	// 这部分后面还需要优化，先采用此方案
	UFUNCTION(BlueprintPure)
		void GetAnimData(FAnimationData& ActionData, EObjectState state, EActionModel actionMode, EEffectState effectState);

	void OnSkillCast(const int32& _SkillID);

	void OnSkillEndCast();

	FORCEINLINE bool UpdateObjectState();
	FORCEINLINE bool UpdateActionModel();
	FORCEINLINE bool UpdateEffectState();
	FORCEINLINE bool UpdateSkillState();

public:
	// owner character
	UPROPERTY(BlueprintReadOnly)
		class AServerCharacter* OwnerCharcter;

	// 移动速度
	UPROPERTY(BlueprintReadOnly)
		float MoveSpeed;

	// 移动朝向
	UPROPERTY(BlueprintReadOnly)
		float MoveDirection;

	UPROPERTY(BlueprintReadOnly)
		FAnimationData ActionData;

	// 是否在战斗状态
	UPROPERTY(BlueprintReadOnly)
		bool bFight;
	// 是否在死亡状态
	UPROPERTY(BlueprintReadOnly)
		bool bDead;

	// 是否在移动
	UPROPERTY(BlueprintReadOnly)
		bool bMove;
	// 是否在掉落
	UPROPERTY(BlueprintReadOnly)
		bool bFalling;

	// 是否在技能施法阶段
	UPROPERTY(BlueprintReadOnly)
		bool bAttack;
	// 是否在技能吟唱阶段
	UPROPERTY(BlueprintReadOnly)
		bool bChant;
	// 是否在技能受击阶段
	UPROPERTY(BlueprintReadOnly)
		bool bBeHit;

	// 是否在眩晕状态
	UPROPERTY(BlueprintReadOnly)
		bool bDizzy;
	// 是否在定身状态
	UPROPERTY(BlueprintReadOnly)
		bool bFix;
	// 是否在睡眠状态
	UPROPERTY(BlueprintReadOnly)
		bool bSleep;

	// entity行为状态
	EObjectState State;
	// character行为模式
	EActionModel ActionMode;
	// 技能施法阶段
	ESkillActionState SkillState;
	// 技能效果状态
	int32 EffectState;

	// entity下一帧行为状态
	EObjectState NextState;
	// character下一帧行为模式
	EActionModel NextActionMode;
	// 下一帧技能施法阶段
	ESkillActionState NextSkillState;
	// 下一帧技能效果状态
	int32 NextEffectState;

	
private:
	TArray<TSharedPtr<FAnimationData>> AnimationDatas;

	//FTimerHandle DelaySetActionTimer;


	
};
