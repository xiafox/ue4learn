#include "RoleJumpComponent.h"
#include "GameFramework/Character.h"
#include "UnrealMathUtility.h"
#include "Character/Player/PlayerCharacter.h"
#include "LogMacros.h"
#include "ClientTypes/GameConst.h"
#include "ClientGameInstance.h"

URoleJumpComponent::URoleJumpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URoleJumpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	APlayerCharacter *OwnerCharacter = Cast<APlayerCharacter>(GetOwnerCharacter());
	if (OwnerCharacter->GetSelfEntity() == nullptr) return;
	UClientActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);///注意：得先执行父类Tick
	if (OwnerCharacter->ActorIsPlayer())
	{
		UE_LOG(LogTemp, Log, TEXT("+++++++CheckJumpState:Name=%s,VelocityZ=%f,LocationZ=%f,++DeltaTime=%f"), *OwnerCharacter->GetName(), (CharacterMovement->Velocity.Z), OwnerCharacter->GetActorLocation().Z, DeltaTime);
		CheckJumpState();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("+++++++SimulatedFalling: JumpState=%d ,VelocityZ=%f,TargetVelocityZ=%f,LocationZ=%f,++DeltaTime=%f"), int32(CurJumpState), (CharacterMovement->Velocity.Z), TargetVelocity.Z, GetOwnerCharacter()->GetActorLocation().Z ,DeltaTime);
		if (!IsSmoothAdjust)
		{
			SimulatedFalling(DeltaTime);
		}
	}
}

void URoleJumpComponent::InitComponent(UCharacterMovementComponent *InCharacterMovement)
{
	CharacterMovement = InCharacterMovement;
	GravityZ = CharacterMovement->GetGravityZ();
	SetGravityScale(1.0);
	CurJumpState = JUMP_STATE::NO_JUMP;
}

void URoleJumpComponent::Jump()
{
	if ( GetOwnerCharacter()==nullptr )
	{
		return;
	}
	int32 JumpCurrentCount = GetOwnerCharacter()->JumpCurrentCount;
	// If this is the first jump and we're already falling,
	if ( JumpCurrentCount == 0 )
	{
		if (CharacterMovement->IsFalling())
		{
			SetJumpZVelocity(TWO_JUMP_HEIGHT);
			UpdateRoleJumpState(JUMP_STATE::START_SECOND_JUMP);
		}
		else
		{
			SetJumpZVelocity(ONE_JUMP_HEIGHT);
			UpdateRoleJumpState(JUMP_STATE::START_FIRST_JUMP);
		}
	}
	else if ( JumpCurrentCount == 1 )
	{
		SetJumpZVelocity(TWO_JUMP_HEIGHT);
		UpdateRoleJumpState(JUMP_STATE::START_SECOND_JUMP);
	}
}

void URoleJumpComponent::StopJumping()
{

}

void URoleJumpComponent::UpdateRoleJumpState(JUMP_STATE InJumpState)
{
	if ( CurJumpState != InJumpState)
	{
		CurJumpState = InJumpState;
		OnRoleJumpStateChanged(InJumpState);
	}
}

void URoleJumpComponent::OnRoleJumpStateChanged(JUMP_STATE InJumpState)
{
	APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(GetOwnerCharacter());
	
	if (PlayerCharacter != nullptr && PlayerCharacter->ActorIsPlayer())
	{
		FVector tempVelocity = CharacterMovement->Velocity;
		if (InJumpState == JUMP_STATE::START_FIRST_JUMP || InJumpState == JUMP_STATE::START_SECOND_JUMP)
		{
			tempVelocity.Z = CharacterMovement->JumpZVelocity;
		}
		PlayerCharacter->NotifyJumpStateChangedToServer(InJumpState, tempVelocity);
		UE_LOG(LogTemp, Log, TEXT("+++++++NotifyJumpStateChangedToServer:Name=%s, JumpState=%d ,PlayerVelocityZ=%f,tempVelocityZ=%f,LocationZ=%f"), *PlayerCharacter->GetName(),int32(InJumpState), (CharacterMovement->Velocity.Z), tempVelocity.Z, GetOwnerCharacter()->GetActorLocation().Z);
	}
}

void URoleJumpComponent::SetJumpZVelocity(float InJumpHeight)
{
	float tempJumpZVelocity = FMath::Sqrt((GravityZ * -2.0f) * InJumpHeight);
	CharacterMovement->JumpZVelocity = tempJumpZVelocity;
}

void URoleJumpComponent::SetGravityScale(float InGravityScale)
{
	CharacterMovement->GravityScale = InGravityScale;
}

void URoleJumpComponent::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode /*= 0*/)
{

}

void URoleJumpComponent::CheckJumpState()
{
	ACharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwnerCharacter());
	if (!IsValid(PlayerCharacter)) return;
	if ( !CharacterMovement->IsFalling() )
	{
		UpdateRoleJumpState(JUMP_STATE::NO_JUMP);
	}
	else 
	{
		if (CharacterMovement->Velocity.Z <= 0)
		{
			UpdateRoleJumpState(JUMP_STATE::JUMP_FALLING);
		}
		else if (PlayerCharacter->JumpCurrentCount == 1)
		{
			UpdateRoleJumpState(JUMP_STATE::FIRST_JUMP_UPING);
		}
		else if (PlayerCharacter->JumpCurrentCount == 2)
		{
			UpdateRoleJumpState(JUMP_STATE::SECOND_JUMP_UPING);
		}
	}

}

void URoleJumpComponent::OnRepJumpState(uint8 InJumpState, FVector InVelocity, FVector InPos)
{
	APlayerCharacter *OwnerCharacter = Cast<APlayerCharacter>(GetOwnerCharacter());
	if (OwnerCharacter != nullptr)
	{
		if (OwnerCharacter->ActorIsPlayer())
		{
			
		}
		else
		{
			TargetVelocity = InVelocity;
			TargetPos = InPos;
			UE_LOG(LogTemp, Log, TEXT("+++++++OnRepJumpState:Name=%s, JumpState=%d ,PlayerVelocityZ=%f,TargetVelocityZ=%f"), *OwnerCharacter->GetName(), int32(InJumpState), (CharacterMovement->Velocity.Z), TargetVelocity.Z);
			if (CurJumpState == JUMP_STATE::JUMP_FALLING && InJumpState == 0)
			{
				//掉落刚好落地
				CurJumpState = JUMP_STATE(InJumpState);
				CharacterMovement->Velocity.Z = 0;
				if ( GetOwnerCharacter()->GetActorLocation().Z > TargetPos.Z)
				{
					GetOwnerCharacter()->SetActorLocation(TargetPos);
				}
			}
			else if (InJumpState == 3 || InJumpState == 1)
			{
				//起跳
				GetOwnerCharacter()->SetActorLocation(TargetPos);
				CharacterMovement->Velocity.Z = TargetVelocity.Z;
				CurJumpState = JUMP_STATE(InJumpState);
			}
			else if ( InJumpState == 5 )
			{
				//开始下降
				CurJumpState = JUMP_STATE(InJumpState);
				if (GetOwnerCharacter()->GetActorLocation().Z < TargetPos.Z && CharacterMovement->Velocity.Z > 0)
				{
					GetOwnerCharacter()->SetActorLocation(TargetPos);
					CharacterMovement->Velocity.Z = TargetVelocity.Z;
				}
			}
			else
			{
				CurJumpState = JUMP_STATE(InJumpState);
			}
		}
	}
}

void URoleJumpComponent::SimulatedFalling( float DeltaTime )
{
	if (CurJumpState == JUMP_STATE::NO_JUMP) return;
	else
	{
		// Move
		FHitResult Hit(1.f);
		//const float timeTick = CharacterMovement->GetSimulationTimeStep(DeltaTime, 0);
		const float timeTick = DeltaTime;
		FVector OldVelocity = CharacterMovement->Velocity;
		// Apply gravity
		const FVector Gravity(0.f, 0.f, GravityZ);
		float GravityTime = timeTick;
		FVector NewVelocity = CharacterMovement->NewFallVelocity(OldVelocity, Gravity, GravityTime);
		FVector Adjusted = 0.5f*(OldVelocity + NewVelocity) * timeTick;//调整的位置
		CharacterMovement->SafeMoveUpdatedComponent(Adjusted, CharacterMovement->UpdatedComponent->GetComponentQuat(), true, Hit);
// 		FVector newPos = GetOwnerCharacter()->GetActorLocation() + Adjusted;
// 		GetOwnerCharacter()->SetActorLocation(newPos);
		CharacterMovement->Velocity = NewVelocity;
		if (CharacterMovement->Velocity.Z == 0)
		{
			CurJumpState = JUMP_STATE::NO_JUMP;
		}
		UE_LOG(LogTemp, Log, TEXT("+++++++SimulatedFalling2: JumpState=%d ,VelocityZ=%f,TargetVelocityZ=%f,position=%f,LocationZ=%f"), int32(CurJumpState), (CharacterMovement->Velocity.Z), TargetVelocity.Z, Adjusted.Z, GetOwnerCharacter()->GetActorLocation().Z);
	}
}

void URoleJumpComponent::SmoothAdjustPosition()
{

}

ACharacter* URoleJumpComponent::GetOwnerCharacter()
{
	return Cast<ACharacter>(GetOwner());
}
