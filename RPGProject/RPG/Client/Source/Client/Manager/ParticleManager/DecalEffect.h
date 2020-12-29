// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Distributions/DistributionVector.h"
#include "Distributions/DistributionFloat.h"
#include "DecalEffect.generated.h"

/*
* 文件名称：DecalEffect.h
* 功能说明：
* 文件作者：zhengxuemei
* 目前维护：zhangfudong
* 创建时间：2016-10-20
*/

class UDecalComponentEX;
class UParticleEmitter;
class UParticleModule;

USTRUCT(BlueprintType)
struct FEmitterData
{
	GENERATED_BODY()

		UPROPERTY()
		UMaterialInterface* DecalMaterial = nullptr;
	UPROPERTY()
		float EmitterDuration = 0.0f; ///< 持续发射时间（EmitterLoops>0才有效）
	UPROPERTY()
		int32 EmitterLoops = 0;	///< 发射次数  0持续发射
	UPROPERTY()
		float SpawnRate = 0.0f;	///< 生成粒子频率，1秒n个
	UPROPERTY()
		int32 BurstCount = 0;		///< 生成粒子次数（Rate=0才有效）

	UPROPERTY()
		FVector InitialSize = FVector::ZeroVector;
	UPROPERTY()
		float InitialHeight = 0.0f;///<贴花高度
	UPROPERTY()
		float Lifetime = 0.0f;
	UPROPERTY()
		FRawDistributionVector ColorOverLifeDist;
	UPROPERTY()
		FRawDistributionFloat AlphaOverLifeDist;
	UPROPERTY()
		FRawDistributionVector SizeByLifeDist;

	UPROPERTY()
		float InitialRotation= 0.0f;
	UPROPERTY()
		float InitialRotationRate = 0.0f;///< 一秒钟转几次
	UPROPERTY()
		FRawDistributionVector ColorScaleOverLifeDist;
	UPROPERTY()
		FRawDistributionFloat AlphaScaleOverLifeDist;

	UPROPERTY()
		FRawDistributionFloat DynamicParam1Dist;
	UPROPERTY()
		float Delay = 0.0f;
	UPROPERTY()
		bool bOnMesh = false;




	FEmitterData()
	{
	}
	FEmitterData(UParticleEmitter* emit);

	FEmitterData(const FEmitterData* Other);
	//FParticleDecalData& operator=(const FParticleDecalData& Other);

private:
	bool _ParseRequired(UParticleModule* module);
	bool _ParseSpawn(UParticleModule* module);
	bool _ParseInitialSizeModule(UParticleModule* module);
	bool _ParseLifetime(UParticleModule* module);
	bool _ParseColorOverLife(UParticleModule* module);
	bool _ParseSizeByLife(UParticleModule* module);
	bool _ParseInitialRotation(UParticleModule* module);
	bool _ParseInitialRotationRate(UParticleModule* module);
	bool _ParseInitialColorScaleOverLife(UParticleModule* module);
	bool _ParseDynamicParameter(UParticleModule* module);

};
//////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FParticle
{
	GENERATED_BODY()

	UPROPERTY()
		UDecalComponentEX* DecalComp =nullptr;
	UPROPERTY()
		float Lifetimer = 0.0f;
	UPROPERTY()
		float Lifetime = 0.0f;
	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial = nullptr;

	FParticle() {};
	FParticle(UDecalComponentEX* Comp, float life, UMaterialInstanceDynamic* mat)
	{
		DecalComp = Comp;
		Lifetime = life;
		Lifetimer = 0;
		DynamicMaterial = mat;
	}
	~FParticle()
	{
// 		if (IsValid(DecalComp))
// 		{
// 			DecalComp->DestroyComponent();
// 		}
	}
};
//////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, Blueprintable, meta = (ShortTooltip = "Decal Effect."))
class  ADecalEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADecalEffect();
	~ADecalEffect();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual bool ShouldTickIfViewportsOnly() const override;// 在world不启动的时候也可以每帧Tick

	virtual void Destroyed() override;

	void PauseDecal();

	void SetVectorParameter(float P_Alpha);///设置所有材质的Vector属性参数
	void SetScalarParameter(float P_Scalar);///设置所有材质的Scalar属性参数
	TArray<FLinearColor> GetVectorParameter();///获取材质Vector参数
	TArray<float> GetScalarParameter();///获取材质Scalar参数
	float m_Lifetime;
	float m_Lifetimer;


	FEmitterData* m_EmitterData;

private:
	void _CreateDecal();
	void _StopEmit();

	UDecalComponentEX* SpawnCS3Decal(class UMaterialInterface* DecalMaterial, FVector DecalSize, FVector Location, FRotator Rotation, float LifeSpan);

	UWorld* m_World;
	FVector m_InitialSizeScale;	// 默认scale0.5 和粒子大小一样
	FTimerHandle TimerHandle;
	FTimerHandle m_TimerHandle;
	TArray<FParticle*> m_DecalComps;

public:
	///根节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* DRootComponent;
};
