// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ServerCharacter.h"
#include "Component/RoleJumpComponent.h"
#include "PlayerCharacter.generated.h"


/**
 * 
 */
UCLASS()
class CLIENT_API APlayerCharacter : public AServerCharacter
{
	GENERATED_BODY()
public:
	//-----------------Begin Entity	
	virtual void BindLinkEntityID(int32 ID)override;
	virtual void BindActorToEntity()override;
	//-----------------End Entity	
	//-----------------Begin InitComponent
	virtual void InitCharacterComponentPostEntityBinded()override;///<创建于entity相关的组件

	void InitPosSyncComponent();
	virtual void InitFilterActorComponent() override;
	void InitCameraComponent();
	void InitRoleJumpComponent();

	/*----------------跳跃功能相关---------------------------*/
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	virtual bool CanJumpInternal_Implementation() const override;
	void SetJumpMaxCount(int InJumpMaxCount);
	void NotifyJumpStateChangedToServer(JUMP_STATE JumpState, FVector InVelocity);
	void OnRepJumpState(uint8 JumpState, FVector InVelocity, FVector InPos);

	//-----------------End InitComponent

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void Jump() override;
	virtual void StopJumping() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* CameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate = 45.0f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate = 45.0f;

private:
	class UPosSyncComponent* PosSyncComp = nullptr;
	URoleJumpComponent* RoleJumpComponent;
};
