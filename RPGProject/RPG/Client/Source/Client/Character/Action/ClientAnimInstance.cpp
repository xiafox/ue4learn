// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientAnimInstance.h"
#include "ClientGameInstance.h"
#include "ClientTypes/ActionDefine.h"
#include "ClientTypes/GameConst.h"
#include "Character/ServerCharacter.h"
#include "Manager/Cfg/CfgManager.h"
#include "ClientBPFunctionLibrary.h"

#pragma optimize( "", off )

void UClientAnimInstance::NativeInitializeAnimation()
{
	InitDatas();
	AServerCharacter* character = Cast<AServerCharacter>(GetOwningActor());
	if (character)
	{
		// 初始化属性状态，因为entity创建完成 还没创建 AnimInstance
		OwnerCharcter = character;
		int32 entityID = character->EntityId;
		OnStateChange(UClientBPFunctionLibrary::GetIntPropertyValue(entityID, TEXT("state")), 0);
		OnEffectStateChange(UClientBPFunctionLibrary::GetIntPropertyValue(entityID, TEXT("effectState")), 0);

		UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>(character->GetMovementComponent());
		if (movement)
		{
			OnMovementModeChanged(EMovementMode::MOVE_None, movement->MovementMode, 0, movement->CustomMovementMode);
		}
	}
	
}

void UClientAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!IsValid(OwnerCharcter))
	{
		AServerCharacter* character = Cast<AServerCharacter>(GetOwningActor());
		if (character == nullptr)
		{
			return;
		}
		OwnerCharcter = character;
	}
	if (ActionMode == EActionModel::IDLE)
	{
		MoveSpeed = OwnerCharcter->GetVelocity().Size();
		if (MoveSpeed > 0.0)
		{
			NextActionMode = EActionModel::MOVE;
		}
	}
	else if (ActionMode == EActionModel::MOVE)
	{
		MoveSpeed = OwnerCharcter->GetVelocity().Size();
		if (MoveSpeed == 0.0)
		{
			NextActionMode = EActionModel::IDLE;
		}
	}
	MoveDirection = OwnerCharcter->GetActorRotation().Yaw;
	
}

void UClientAnimInstance::PreUpdateAnimation(float DeltaSeconds)
{
	Super::PreUpdateAnimation(DeltaSeconds);

	bool bChangeState = UpdateObjectState();
	bool bChangeAction = UpdateActionModel();
	bool bChangeEffect = UpdateEffectState();
	bool bChangeSkill = UpdateSkillState();
	if ( bChangeState || bChangeAction || bChangeEffect || bChangeSkill)
	{
		//DelaySelectAction();
	}
	
}

void UClientAnimInstance::InitDatas()
{
	if (UClientGameInstance::Instance && UClientGameInstance::Instance->CfgManager)
	{
		const UConfigTable* table = UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_ANIMATION_CONFIG);
		if (table)
		{
			TArray<FANIMATION_CONFIG_DATA*> AnimArr;
			table->GetAllRows(AnimArr);
			for (auto anim : AnimArr)
			{
				if (anim->AnimPath.ToSoftObjectPath().IsValid())
				{
					TSharedPtr<FAnimationData> Data = MakeShareable(new FAnimationData());
					Data->State = anim->State;
					Data->ActionModel = anim->ActionModel;
					Data->EffectState = anim->EffectState;
					Data->BloopAnim = anim->BloopAnim;
					Data->AnimRate = anim->AnimRate;
					Data->StartAnimPos = anim->StartAnimPos;

					FStreamableManager streamableManager;
					UObject* _obj = streamableManager.LoadSynchronous(anim->AnimPath.ToSoftObjectPath());
					checkf(_obj != nullptr, TEXT("Load Anim Path(%s) fail!!"), *anim->AnimPath.ToString());
					
					auto a = _obj->GetClass();
					Data->AnimSeq = Cast<UAnimSequenceBase>(_obj);
					if (Data->AnimSeq == nullptr)
					{
						Data->AnimBlend = Cast<UBlendSpaceBase>(_obj);
					}

					AnimationDatas.Add(Data);
				}
			}
		}
	}
}

void UClientAnimInstance::OnStateChange(const int32& NewState, const int32& OldState)
{
	NextState = (EObjectState)NewState;
}

void UClientAnimInstance::OnEffectStateChange(const int32& NewEffect, const int32& OldEffect)
{
	if (NewEffect != OldEffect)
	{
		NextEffectState = NewEffect;
	}
}

void UClientAnimInstance::OnMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode, uint8 PrevCustomMode, uint8 NewCustomMode)
{
	switch (NewMovementMode)
	{
	case EMovementMode::MOVE_None:
		NextActionMode = EActionModel::IDLE;
		break;
	case EMovementMode::MOVE_NavWalking:
	case EMovementMode::MOVE_Walking:
		NextActionMode = EActionModel::MOVE;
		break;
	case EMovementMode::MOVE_Falling:
		NextActionMode = EActionModel::FALLING;
		break;
			
	default:
		break;
	}
}

void UClientAnimInstance::GetAnimData(FAnimationData& ActionData, EObjectState state, EActionModel actionMode, EEffectState effectState)
{
	for (auto seq : AnimationDatas)
	{
		if (seq->State == state && seq->ActionModel == actionMode && seq->EffectState == effectState)
		{
			ActionData = *seq;
			return;
		}
	}
}

void UClientAnimInstance::OnSkillCast(const int32 & _SkillID)
{
}

void UClientAnimInstance::OnSkillEndCast()
{
}

bool UClientAnimInstance::UpdateObjectState()
{
	if (NextState != EObjectState::STATE_NONE)
	{
		switch (NextState)
		{
		case EObjectState::STATE_FIGHT:
			bFight = true;
			bDead = false;
			break;
		case EObjectState::STATE_DEAD:
			bFight = false;
			bDead = true;
			break;
		default:
			bFight = false;
			bDead = false;
			break;
		}
		State = NextState;
		NextState = EObjectState::STATE_NONE;
		return true;
	}
	return false;
}

bool UClientAnimInstance::UpdateActionModel()
{
	if (NextActionMode != EActionModel::NONE)
	{
		switch (NextActionMode)
		{
		case EActionModel::MOVE:
			bMove = true;
			bFalling = false;
			break;
		case EActionModel::FALLING:
			bMove = false;
			bFalling = true;
			break;
		default:
			bMove = false;
			bFalling = false;
			break;
		}
		ActionMode = NextActionMode;
		NextActionMode = EActionModel::NONE;
		return true;
	}
	return false;
}

bool UClientAnimInstance::UpdateEffectState()
{
	if (NextEffectState != 0)
	{
		// 根据优先级顺序排序,如果优先级修改对应的排序也需要修改
		TArray<bool*> bArrEffects({ &bSleep, &bDizzy, &bFix });
		TArray<int32> iArrEffects(EFFECT_STATE_PRIORITY);

		bool bTrue(false);
		for (auto It = iArrEffects.CreateConstIterator(); It; ++It)
		{
			if (!bTrue)
			{
				bTrue = bool(*It & NextEffectState);
			}
			*bArrEffects[It.GetIndex()] = bTrue;
		}
		EffectState = NextEffectState;
		NextEffectState = 0;
		return true;
	}
	return false;
}

bool UClientAnimInstance::UpdateSkillState()
{
	if (NextSkillState != ESkillActionState::Skill_NO)
	{
		switch (NextSkillState)
		{
		case ESkillActionState::SKILL_ATTACK:
			bAttack = true;
			bChant = false;
			bBeHit = false;
			break;
		case ESkillActionState::SKILL_CHANT:
			bAttack = false;
			bChant = true;
			bBeHit = false;
			break;
		case ESkillActionState::SKILL_BEHIT:
			bAttack = false;
			bChant = false;
			bBeHit = true;
		default:
			bAttack = false;
			bChant = false;
			bBeHit = false;
			break;
		}
		SkillState = NextSkillState;
		NextSkillState = ESkillActionState::Skill_NO;
		return true;
	}
	return false;
}

#pragma optimize( "", on )