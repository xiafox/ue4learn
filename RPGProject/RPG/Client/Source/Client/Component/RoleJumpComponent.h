#pragma once
#include "ClientActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "RoleJumpComponent.generated.h"

/**
* 跳跃状态
*/
UENUM(BlueprintType)
enum class JUMP_STATE : uint8
{
	///非跳跃状态
	NO_JUMP = 0,
	///开始一段跳
	START_FIRST_JUMP,
	///正处在一段跳起上升中
	FIRST_JUMP_UPING,
	///开始二段跳
	START_SECOND_JUMP,
	///正处在二段跳起上升中
	SECOND_JUMP_UPING,
	///掉落下降中
	JUMP_FALLING,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLIENT_API URoleJumpComponent : public UClientActorComponent
{
	GENERATED_BODY()

public:
	URoleJumpComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitComponent(UCharacterMovementComponent *InCharacterMovement);

	void Jump();
	void StopJumping();
	///更新跳跃状态
	void UpdateRoleJumpState(JUMP_STATE InJumpState);
	UFUNCTION(BluePrintPure, Category = "Player|JumpState")
		JUMP_STATE GetCurJumpState() { return CurJumpState; };
	void OnRoleJumpStateChanged(JUMP_STATE InJumpState);
	///设置跳跃时的初始速度（瞬时垂直加速度）速度
	void SetJumpZVelocity(float InJumpHeight);
	///设置重力缩放值
	void SetGravityScale(float InGravityScale);
	///跳跃模型发生改变时
	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0);
	///同步跳跃状态
	void OnRepJumpState(uint8 InJumpState, FVector InVelocity, FVector InPos);

private:
	///检测是否在跳跃后的下落中
	void CheckJumpState();
	///不是本客户端的玩家执行模拟掉落
	void SimulatedFalling( float DeltaTime );
	void SmoothAdjustPosition();

	ACharacter* GetOwnerCharacter();
	UCharacterMovementComponent* GetCharacterMovement() { return CharacterMovement; };

public:
	float GravityZ;
private:
	UCharacterMovementComponent *CharacterMovement;
	JUMP_STATE CurJumpState;
	FVector TargetVelocity;
	FVector TargetPos;
	FTimerHandle SmoothMoveTimer;
	bool IsSmoothAdjust;
};