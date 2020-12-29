// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ClientLevelScriptActor.h"
#include "CreateRoleLevel.generated.h"

/*
* 文件名称：CreateRoleLevel.h
* 功能说明：
* 文件作者：lizhenghui
* 目前维护：shuqingping
* 创建时间：2017-06-21
*/

UCLASS(BlueprintType)
class ACreateRoleLevel : public AClientLevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;	
	virtual void CreateDynamicRes() override;	
	virtual void OnLoadLevelFinished() override;

	void OnCreateModelFinished();
};
