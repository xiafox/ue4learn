// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Particles/ParticleModule.h"
#include "ParticleModuleBaseEX.generated.h"

/*
* 文件名称：ParticleModuleLightBaseEX.h
* 功能说明：扩展基类，粒子要扩展必须有一个虚基类
* 文件作者：zhengxuemei
* 目前维护：
* 创建时间：2019-01-02
*/

UCLASS(editinlinenew, hidecategories = Object, abstract, meta = (DisplayName = "Extend"))
class UParticleModuleBaseEX : public UParticleModule
{
	GENERATED_UCLASS_BODY()

};


