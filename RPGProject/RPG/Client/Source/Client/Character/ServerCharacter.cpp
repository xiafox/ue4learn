// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerCharacter.h"
#include "ClientBPFunctionLibrary.h"
#include "Component/EquipComponent/EquipComponent.h"
#include "Interface/ModelInterfaceObject.h"
#include "Action/ClientAnimInstance.h"
#include "ClientGameInstance.h"

AServerCharacter::AServerCharacter(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	EntityId = 0;
	bIsInitialized = false;
	InitCharacterDefaultComponent();
}

void AServerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsDestroyed)
	{
		///<延时销毁
		FTimerHandle DeferredTimerHander;
		UClientGameInstance::Instance->GetTimerManager().SetTimer(DeferredTimerHander, this, &AServerCharacter::DeferredDestroy, 0.1f, false);
	}
}

void AServerCharacter::InitCharacterDefaultComponent()
{

}

void AServerCharacter::InitCharacterComponentPostActorCreated()
{
	Super::InitCharacterComponentPostActorCreated();
}

void AServerCharacter::InitCharacterComponentPostEntityBinded()
{
	InitFilterActorComponent();
}

void AServerCharacter::InitFilterActorComponent()
{

}

void AServerCharacter::BindActorToEntity()
{	
#if WITH_EDITOR
	SetActorLabel(GetFName().ToString() + FString("(") + FString::FromInt(EntityId) + FString(")"));
#endif
	///<设置初始化完成
	bIsInitialized = true;
	///<创建与Entity有关组件
	InitCharacterComponentPostEntityBinded();
	
	///<创建外观
	CreateAppearance();
	
}

void AServerCharacter::PreUnBindActorToEntity()
{

}

void AServerCharacter::DeferredDestroy()
{
	Destroy();
}

void AServerCharacter::UnBindActorToEntity()
{
	PreUnBindActorToEntity();
	BindLinkEntityID(0);
	bIsDestroyed = true;
	
}


KBEngine::Entity* AServerCharacter::GetSelfEntity()
{
	return KBEngine::KBEngineApp::app->FindEntity(EntityId);
}

bool AServerCharacter::ActorIsPlayer()
{
	return EntityId == UClientBPFunctionLibrary::GetPlayerID();
}


void AServerCharacter::onUpdateVolatileData(const FVector& position, const FVector& direction)
{
	if (GetFilterComponent() != nullptr && GetSelfEntity() != nullptr)
	{
		GetFilterComponent()->OnUpdateVolatileData(position, direction, GetSelfEntity()->ParentID());
	}
}



void AServerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode /*= 0*/)
{
	Super::OnMovementModeChanged(PrevMovementMode, PrevCustomMode);

	if (USkeletalMeshComponent * sMesh = GetMesh())
	{
		UClientAnimInstance* animInst = Cast<UClientAnimInstance>(sMesh->GetAnimInstance());
		UCharacterMovementComponent* movement = GetCharacterMovement();
		if (animInst && movement)
		{
			animInst->OnMovementModeChanged(PrevMovementMode, movement->MovementMode, PrevCustomMode, movement->CustomMovementMode);
		}
	}
}

void AServerCharacter::OnStateChange(const int32 & NewState, const int32 & OldState)
{
	if (USkeletalMeshComponent * sMesh = GetMesh())
	{
		UClientAnimInstance* animInst = Cast<UClientAnimInstance>(sMesh->GetAnimInstance());
		UCharacterMovementComponent* movement = GetCharacterMovement();
		if (animInst && movement)
		{
			animInst->OnStateChange(NewState,OldState);
		}
	}
}

void AServerCharacter::OnEffectStateChange(const int32& NewEffect, const int32& OldEffect)
{
	if (USkeletalMeshComponent * sMesh = GetMesh())
	{
		UClientAnimInstance* animInst = Cast<UClientAnimInstance>(sMesh->GetAnimInstance());
		UCharacterMovementComponent* movement = GetCharacterMovement();
		if (animInst && movement)
		{
			animInst->OnEffectStateChange(NewEffect, OldEffect);
		}
	}
}

void AServerCharacter::CreateAppearance()
{

	UModelInterfaceObject* MInterface = Cast<UModelInterfaceObject>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("ModelInterfaceObject"), EntityId));
	if (MInterface == nullptr)return;

	TMap<MODEL_PART_TYPE, FString> ModelParts = MInterface->GetModelPartDatas();
	EquipComponent->CreateModel(ModelParts);

	float modelScale = UClientBPFunctionLibrary::GetFloatPropertyValue(EntityId, TEXT("modelScale"));
	if (modelScale > 0.0)
	{
		SetModelScale(modelScale);
	}
}

void AServerCharacter::CreateAppearance(const TMap<MODEL_PART_TYPE, FString>& ModelParts)
{
	EquipComponent->CreateModel(ModelParts);
}

void AServerCharacter::ChangeModelPart(const MODEL_PART_TYPE& ModelPart, const FString& ModelID)
{
	EquipComponent->ChangeModelPart(ModelPart, ModelID);
}

void AServerCharacter::SetModelScale(const float& ModelScale)
{
	SetActorScale3D(FVector(ModelScale, ModelScale, ModelScale));
}


void AServerCharacter::BindLinkEntityID(int32 ID)
{	
	EntityId = ID;
	this->SetRemoteRoleForBackwardsCompat(ROLE_AutonomousProxy);
}