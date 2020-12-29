// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/Filter/Role/PosSyncComponent.h"
#include "Component/Filter/Role/RoleFilterActorComponent.h"

void APlayerCharacter::BindLinkEntityID(int32 ID)
{
	Super::BindLinkEntityID(ID);
	
}

void APlayerCharacter::BindActorToEntity()
{
	if (ActorIsPlayer())
	{
		UGameplayStatics::GetPlayerController(this, 0)->Possess(this);
	}
	Super::BindActorToEntity();
	
}

void APlayerCharacter::InitCharacterComponentPostEntityBinded()
{
	Super::InitCharacterComponentPostEntityBinded();
	if (ActorIsPlayer())
	{
		InitPosSyncComponent();
		InitCameraComponent();
	}
	InitRoleJumpComponent();
}

void APlayerCharacter::InitPosSyncComponent()
{
	PosSyncComp = NewObject<UPosSyncComponent>(this, "TransFormControl", RF_NoFlags, nullptr, false, nullptr);
	this->AddInstanceComponent(PosSyncComp);
	PosSyncComp->RegisterComponent();
}

void APlayerCharacter::InitFilterActorComponent()
{
	if (ActorIsPlayer())
	{
		//SetFilterComponent(NewObject<UDumbFilterActorComponent>(this, "root", RF_NoFlags, nullptr, false, nullptr));
	}
	else
	{
		SetFilterComponent(NewObject<URoleFilterActorComponent>(this, "root", RF_NoFlags, nullptr, false, nullptr));
		this->AddInstanceComponent(GetFilterComponent());
		GetFilterComponent()->RegisterComponent();
	}
	
}

void APlayerCharacter::InitCameraComponent()
{
	SpringArmComponent = NewObject<USpringArmComponent>(this, TEXT("SpringArmCompent"));
	SpringArmComponent->RegisterComponent();
	SpringArmComponent->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 35.0f));

	CameraComponent = NewObject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	CameraComponent->RegisterComponent();
	CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->SetRelativeLocationAndRotation(FVector(60.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	CameraComponent->SetActive(true);
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(this);
}


void APlayerCharacter::InitRoleJumpComponent()
{
	RoleJumpComponent = NewObject<URoleJumpComponent>(this, TEXT("RoleJumpComponent"));
	if (RoleJumpComponent != nullptr)
	{
		this->AddInstanceComponent(RoleJumpComponent);
		RoleJumpComponent->RegisterComponent();
		RoleJumpComponent->InitComponent(GetCharacterMovement());
	}
	if (!ActorIsPlayer())
	{
		GetCharacterMovement()->SetComponentTickEnabled(false);//不是玩家，移动组件tick先停止
	}
	SetJumpMaxCount(2);//暂时先默认设置2
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode /*= 0*/)
{
	AServerCharacter::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	RoleJumpComponent->OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	bool tempCanJump = AServerCharacter::CanJumpInternal_Implementation();
	///这里面可以写一下自己的逻辑判断能不能跳，比如一些状态下不能跳跃
	return tempCanJump;
}

void APlayerCharacter::SetJumpMaxCount(int InJumpMaxCount)
{
	JumpMaxCount = InJumpMaxCount;
}

void APlayerCharacter::NotifyJumpStateChangedToServer(JUMP_STATE JumpState, FVector InVelocity)
{
	if (GetSelfEntity() == nullptr) return;
	KBEngine::Entity* entity = GetSelfEntity();
	KBEngine::FVariantArray args;
	args.Add(uint8(JumpState));
	args.Add(InVelocity);
	args.Add(GetActorLocation());
	entity->CellCall(TEXT("CELL_changeRoleJumpState"), args);
}

void APlayerCharacter::OnRepJumpState(uint8 JumpState, FVector InVelocity, FVector InPos)
{
	RoleJumpComponent->OnRepJumpState(JumpState, InVelocity, InPos);
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlayerCharacter::TouchStopped);

}

void APlayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void APlayerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void APlayerCharacter::Jump()
{
	AServerCharacter::Jump();
	RoleJumpComponent->Jump();
}

void APlayerCharacter::StopJumping()
{
	AServerCharacter::StopJumping();
	RoleJumpComponent->StopJumping();
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = YawRotation.Vector(); 
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
