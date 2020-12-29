// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "KBEngine.h"
#include "../../ClientActorComponent.h"
#include "PosSyncComponent.generated.h"

/*
* 功能说明：角色同步移动位置组件（主要向服务器发送同步主角当前移动位置）
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class  UPosSyncComponent : public UClientActorComponent
{
	GENERATED_BODY()

public:	
	UPosSyncComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
