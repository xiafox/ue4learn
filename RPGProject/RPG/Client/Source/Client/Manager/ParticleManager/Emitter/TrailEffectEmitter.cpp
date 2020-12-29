// Fill out your copyright notice in the Description page of Project Settings.

#include "TrailEffectEmitter.h"
// UE4
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
// 数据类型


void ATrailEffectEmitter::StartPlay()
{
	Super::StartPlay();

}

void ATrailEffectEmitter::DelayDestroySetting()
{
}

void ATrailEffectEmitter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

