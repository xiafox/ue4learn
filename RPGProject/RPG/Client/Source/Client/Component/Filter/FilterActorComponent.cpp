// Fill out your copyright notice in the Description page of Project Settings.


#include "FilterActorComponent.h"

UFilterActorComponent::UFilterActorComponent()
{
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UFilterActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFilterActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}
