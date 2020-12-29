// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Particles/TypeData/ParticleModuleTypeDataBase.h"


#include "ParticleModuleTypeDataDecal.generated.h"

/*
* 文件名称：ParticleModuleTypeDataDecal.h
* 功能说明：
* 文件作者：zhengxuemei
* 目前维护：
* 创建时间：2019-04-19
*/

UCLASS(editinlinenew/*, MinimalAPI*/, hidecategories = Object, meta = (DisplayName = "Type Decal Data"))
class /*PARTICLEEDITOREX_API*/ UParticleModuleTypeDataDecal : public UParticleModuleTypeDataBase
{
	GENERATED_UCLASS_BODY()
		
// 
 	virtual void PostLoad();

// 	//~ Begin UObject Interface
 	virtual bool IsPostLoadThreadSafe() const override;
#if WITH_EDITOR
	virtual void	PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
// 
  	virtual FParticleEmitterInstance*	CreateInstance(UParticleEmitter* InEmitterParent, UParticleSystemComponent* InComponent) override;

};
