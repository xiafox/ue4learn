// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "Particles/TypeData/ParticleModuleTypeDataBase.h"


#include "ParticleModuleTypeDataSkeletalMesh.generated.h"

/*
* 文件名称：ParticleModuleTypeDataSkeletalMesh.h
* 功能说明：
* 文件作者：zhengxuemei
* 目前维护：
* 创建时间：2019-02-19
*/

UCLASS(editinlinenew/*, MinimalAPI*/, hidecategories = Object, meta = (DisplayName = "Type SkeletalMesh Data"))
class /*PARTICLEEDITOREX_API*/ UParticleModuleTypeDataSkeletalMesh : public UParticleModuleTypeDataBase
{
	GENERATED_UCLASS_BODY()

		/** The static mesh to render at the particle positions */
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class UAnimSequenceBase* Animation;

	UPROPERTY(EditAnywhere, Category = Mesh)
		bool AnimationLoop;
	


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
