// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"
#include "Component/EquipComponent/EquipComponent.h"

// Sets default values
AGameCharacter::AGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	///<初始化属性
	bIsDestroyed = false;
	InitCharacterDefaultComponent();
}

void AGameCharacter::PostActorCreated()
{
	///<初始化组件
	InitCharacterComponentPostActorCreated();
}

void AGameCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGameCharacter::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bIsDestroyed = true;
	TArray<AActor*> Actors;
	this->GetAttachedActors(Actors);
	for (auto attachActor : Actors)
	{
		attachActor->Destroy();
	}
	Super::EndPlay(EndPlayReason);
}

void AGameCharacter::Destroyed()
{	
	Super::Destroyed();
}

void AGameCharacter::InitCharacterDefaultComponent()
{
	InitCapsuleComponent();
	InitCharacterMovementComponent();
}

void AGameCharacter::InitCharacterComponentPostActorCreated()
{	
	InitAppearanceComponent();
}

void AGameCharacter::InitCapsuleComponent()
{

}

void AGameCharacter::InitCharacterMovementComponent()
{

}

void AGameCharacter::InitAppearanceComponent()
{
	EquipComponent = NewObject<UEquipComponent>(this,TEXT("EquipComponent"));
	EquipComponent->RegisterComponent();
}

void AGameCharacter::CreateAppearance()
{

}

void AGameCharacter::CreateApperanceOver()
{

}

bool AGameCharacter::IsDestroyed()
{
	return bIsDestroyed;
}

void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

