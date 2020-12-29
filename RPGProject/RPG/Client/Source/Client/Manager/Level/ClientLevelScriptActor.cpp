// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientLevelScriptActor.h"
#include "ClientGameInstance.h"


AClientLevelScriptActor::AClientLevelScriptActor(const FObjectInitializer& ObjectInitializer)
{
	bIsDynamicResFinished = false;
	PrimaryActorTick.bCanEverTick = true;
}


void AClientLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	if (!CheckLoginRun())	return;
	//避免直接加载卡住主线程，导致界面未及时显示穿帮
	UClientGameInstance::Instance->GetWorld()->GetTimerManager().SetTimer(DelayLoadFinishedHandle, this, &AClientLevelScriptActor::OnLoadLevelDelay, 0.1f, false);
}

void AClientLevelScriptActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void AClientLevelScriptActor::CreateDynamicRes()
{
	bIsDynamicResFinished = true;
}

bool AClientLevelScriptActor::CheckLoginRun()
{
	if (!IsValid(UClientGameInstance::Instance)) return false;	//避免在某些关卡直接播放会崩溃

	return true;
}


void AClientLevelScriptActor::OnLoadLevelFinished()
{
	
}

void AClientLevelScriptActor::OnLoadLevelDelay()
{
	//加载资源关卡
	CreateDynamicRes();
	if (bIsDynamicResFinished)
	{
		OnLoadLevelFinished();
	}	
}
