// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalEffect.h"
#include "Particles/Emitter.h"
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleModuleRequired.h"

#include "Particles/ParticleSpriteEmitter.h"
#include "Particles/Parameter/ParticleModuleParameterDynamic.h"

#include "Particles/Spawn/ParticleModuleSpawn.h"
#include "Particles/Size/ParticleModuleSize.h"
#include "Particles/Size/ParticleModuleSizeMultiplyLife.h"
#include "Particles/Lifetime/ParticleModuleLifetime.h"
#include "Particles/Color/ParticleModuleColorOverLife.h"
#include "Particles/Color/ParticleModuleColorScaleOverLife.h"
#include "Particles/Rotation/ParticleModuleRotation.h"
#include "Particles/RotationRate/ParticleModuleRotationRate.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleModule.h"
#include "ClientGameInstance.h"



#pragma optimize("", off)
// Sets default values
ADecalEffect::ADecalEffect()
	:
//	m_EmitterData(NULL),
	m_Lifetimer(0),
	m_World(NULL),
	m_InitialSizeScale(FVector(0.5, 0.5, 0.5))
{
	m_EmitterData = NULL;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_InitialSizeScale = FVector(0.5, 0.5, 0.5);

	DRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DRootComponent"));
	SetRootComponent(DRootComponent);
}

ADecalEffect::~ADecalEffect()
{
	//_StopEmit();
	if (m_EmitterData)
	{
		delete m_EmitterData;
		m_EmitterData = NULL;
	}
}

// Called when the game starts or when spawned
void ADecalEffect::BeginPlay()
{
	//Super::BeginPlay();
	//if (IsValid(UClientGameInstance::Instance))
	//{
	//	OnEndPlay.AddUniqueDynamic(UClientGameInstance::Instance, &UClientGameInstance::OnActorEndPlay);
	//}

	////如果是在游戏世界，走这里的初始化，ADecalEffect::~ADecalEffect()中释放
	//m_EmitterData = new FEmitterData(UDecalManager::GetInstance()->CurDecalData);

	//UClientGameInstance::Instance->CustomTimerManager->SetTimer(m_TimerHandle, this, &ADecalEffect::_CreateDecal, 1.0 / m_EmitterData->SpawnRate, true, m_EmitterData->Delay);

	//if (m_EmitterData->EmitterLoops != 0)
	//{
	//	m_Lifetime = m_EmitterData->EmitterLoops * m_EmitterData->EmitterDuration + m_EmitterData->Delay + m_EmitterData->Lifetime;
	//	UClientGameInstance::Instance->CustomTimerManager->SetTimer(TimerHandle, this, &ADecalEffect::_StopEmit, m_Lifetime - m_EmitterData->Lifetime - m_EmitterData->SpawnRate, false);
	//}
}

// Called every frame
void ADecalEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
//
//	//如果是在编辑器预览窗口，走这里的初始化
//	if (!m_EmitterData)
//	{
//		//ADecalEffect::~ADecalEffect()中释放
//		m_EmitterData = new FEmitterData(UDecalManager::GetInstance()->CurDecalData);
//		UE_LOG(LogScript, Warning, TEXT("ADecalEffect::Tick: Create data in Tick function!"));
//	}
//
//	if (!m_EmitterData)
//	{
//		//CS3_ERROR(TEXT("-->Null Pointer error:ADecalEffect::Tick: m_EmitterData!"));
//		return;
//	}
//	//在第一次Tick生成贴花组件
//	if (m_EmitterData->bOnMesh)
//	{
//		m_EmitterData->bOnMesh = false;
//
//		GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &ADecalEffect::_CreateDecal, 1.0 / m_EmitterData->SpawnRate, true, m_EmitterData->Delay);
//
//		if (m_EmitterData->EmitterLoops != 0)
//		{
//			m_Lifetime = m_EmitterData->EmitterLoops * m_EmitterData->EmitterDuration + m_EmitterData->Delay + m_EmitterData->Lifetime;
//			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADecalEffect::_StopEmit, m_Lifetime - m_EmitterData->Lifetime - m_EmitterData->SpawnRate, false);
//		}
//	}
//
//
//	m_Lifetimer += DeltaTime;
//	// 	if (m_EmitterData->EmitterLoops != 0 && m_Lifetimer > m_Lifetime)
//	// 	{
//	// 		//m_StopEmitterTimer = true;
//	// 		//_StopEmit();
//	// 	}
//	// 	if (m_EmitterData->EmitterLoops != 0)
//	// 	{
//	// 		if (m_Lifetimer > m_EmitterData->EmitterDuration* m_EmitterData->EmitterLoops)
//	// 		{
//	// 			//GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);
//	// 		}
//	// 
//	// 	}
//
//
//	for (int i = 0; i < m_DecalComps.Num(); i++)
//	{
//		FParticle* par = m_DecalComps[i];
//		par->Lifetimer += DeltaTime;
//		if (par->Lifetimer < par->Lifetime)
//		{
//			float F = FMath::Abs(par->Lifetime - 0.f) < 0.01f ? 0 : par->Lifetimer / par->Lifetime;
//
//			FVector vColor, vColorScale;
//			float fAlpha, fSize, fDynamic, fAlphaScale;
//			// 这里说明一下，之前是直接处理Distribution的数据，后来打包后发现Distribution的数据没有了
//			// 原因是发射器module的数据已经cook到了LookupTable里，打包后为了节省资源就不再保存Distribution，所以改为了读取Lookup Table数据
//			// zhengxuemei 2017年9月20日14:13:19 详见：https://udn.unrealengine.com/questions/390981/%E6%89%93%E5%8C%85%E5%90%8E%E8%AF%BB%E5%8F%96%E7%B2%92%E5%AD%90%E6%95%B0%E6%8D%AE%E5%87%BA%E9%94%99.html
//
//#if WITH_EDITOR // 编辑器里不能用isCreated()判断
//			vColor = m_EmitterData->ColorOverLifeDist.Distribution ? m_EmitterData->ColorOverLifeDist.GetValue(F) : FVector(1.0f, 1.0f, 1.0f);
//			fAlpha = m_EmitterData->AlphaOverLifeDist.Distribution ? m_EmitterData->AlphaOverLifeDist.GetValue(F) : 1.0f;
//			fSize = m_EmitterData->SizeByLifeDist.Distribution ? m_EmitterData->SizeByLifeDist.GetValue(F).X : 1.0f;
//			fDynamic = m_EmitterData->DynamicParam1Dist.Distribution ? m_EmitterData->DynamicParam1Dist.GetValue(F) : 0.0f;
//			vColorScale = m_EmitterData->ColorScaleOverLifeDist.Distribution ? m_EmitterData->ColorScaleOverLifeDist.GetValue(F) : FVector(1.0f, 1.0f, 1.0f);
//			fAlphaScale = m_EmitterData->AlphaScaleOverLifeDist.Distribution ? m_EmitterData->AlphaScaleOverLifeDist.GetValue(F) : 1.0f;
//#else			//打包后Distribution为空
//			vColor = m_EmitterData->ColorOverLifeDist.IsCreated() ? m_EmitterData->ColorOverLifeDist.GetValue(F) : FVector(1.0f, 1.0f, 1.0f);
//			fAlpha = m_EmitterData->AlphaOverLifeDist.IsCreated() ? m_EmitterData->AlphaOverLifeDist.GetValue(F) : 1.0f;
//			fSize = m_EmitterData->SizeByLifeDist.IsCreated() ? m_EmitterData->SizeByLifeDist.GetValue(F).X : 1.0f;
//			fDynamic = m_EmitterData->DynamicParam1Dist.IsCreated() ? m_EmitterData->DynamicParam1Dist.GetValue(F) : 0.0f;
//			vColorScale = m_EmitterData->ColorScaleOverLifeDist.IsCreated() ? m_EmitterData->ColorScaleOverLifeDist.GetValue(F) : FVector(1.0f, 1.0f, 1.0f);
//			fAlphaScale = m_EmitterData->AlphaScaleOverLifeDist.IsCreated() ? m_EmitterData->AlphaScaleOverLifeDist.GetValue(F) : 1.0f;
//#endif
//			// 设置颜色、大小、旋转,此处的m_InitialSizeScale和原有的意义不同，X,Y代表放缩，Z代表高度
//			vColor *= vColorScale;
//			fAlpha *= fAlphaScale;
//			par->DynamicMaterial->SetVectorParameterValue(TEXT("ColorParam"), FLinearColor(vColor.X, vColor.Y, vColor.Z, fAlpha));
//			par->DynamicMaterial->SetScalarParameterValue(TEXT("DynamicParam1"), fDynamic);
//			par->DecalComp->SetRelativeScale3D(FVector(fSize * m_InitialSizeScale.X, fSize* m_InitialSizeScale.Y, 1));
//
//			par->DecalComp->AddRelativeRotation(FRotator(0, 0, m_EmitterData->InitialRotationRate * 360 * DeltaTime /*/ m_EmitterData->Lifetime*/));
//			par->DecalComp->SetVisibility(true);
//		}
//		else // 生命到时删除粒子//只要par->Lifetime > 0 生命时间到了就必须删除
//		{
//			par->DecalComp->DestroyComponent();
//			delete par;
//			m_DecalComps.RemoveAt(i);
//		}
//	}
}

bool ADecalEffect::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ADecalEffect::Destroyed()
{
	/*_StopEmit();
	for (int i = 0; i < m_DecalComps.Num(); i++)
	{
		m_DecalComps[i]->DecalComp->DestroyComponent();
		delete m_DecalComps[i];
	}
	m_DecalComps.Empty();*/
}

void ADecalEffect::PauseDecal()
{
	/*this->SetActorTickEnabled(false);
	UClientGameInstance::Instance->CustomTimerManager->PauseTimer(TimerHandle);
	UClientGameInstance::Instance->CustomTimerManager->PauseTimer(m_TimerHandle);*/
}

void ADecalEffect::SetVectorParameter(float P_Alpha)
{
	/*	for (int i = 0; i < m_DecalComps.Num(); i++)
		{
			FParticle* par = m_DecalComps[i];
			FLinearColor LinearColor;
			par->DynamicMaterial->GetVectorParameterValue(TEXT("ColorParamEX"), LinearColor);
			par->DynamicMaterial->SetVectorParameterValue(TEXT("ColorParamEX"), FLinearColor(LinearColor.R, LinearColor.G, LinearColor.B, P_Alpha));
		}*/
// 		FString ss = FString::SanitizeFloat(P_Alpha);
// 		FMessageLog("PIE").Warning(FText::FromString(ss));
}

void ADecalEffect::SetScalarParameter(float P_Scalar)
{
	/*for (int i = 0; i < m_DecalComps.Num(); i++)
	{
		FParticle* par = m_DecalComps[i];
		par->DynamicMaterial->SetScalarParameterValue(TEXT("DynamicParam1EX"), P_Scalar);
	}*/
}

TArray<FLinearColor> ADecalEffect::GetVectorParameter()
{
	TArray<FLinearColor> Vectorvalue;
	/*if (m_DecalComps.Num() < 0)
	{
		return Vectorvalue;
	}
	for (int i = 0; i < m_DecalComps.Num(); i++)
	{
		FLinearColor paricleValue;
		FParticle* par = m_DecalComps[i];
		par->DynamicMaterial->GetVectorParameterValue(TEXT("ColorParamEX"), paricleValue);
		if (paricleValue != FLinearColor(0,0,0.0))
		{
			Vectorvalue.Add(paricleValue);
		}

	}*/
	return Vectorvalue;
}

TArray<float> ADecalEffect::GetScalarParameter()
{
	TArray<float> ScaleValue;
	if (m_DecalComps.Num() < 0)
	{ 
		return ScaleValue;
	}
	for (int i = 0; i < m_DecalComps.Num(); i++)
	{
		float particlevalue;
		FParticle* par = m_DecalComps[i];
		par->DynamicMaterial->GetScalarParameterValue(TEXT("DynamicParam1EX"), particlevalue);
		if (FMath::Abs(particlevalue - 0.f) > 0.01f)
		{
			ScaleValue.Add(particlevalue);
		}
	}
	return ScaleValue;
}

void ADecalEffect::_CreateDecal()
{
	////FString ItemInfo = FString::Printf(TEXT("CreateDecal,%d"), ii++);
	////GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, ItemInfo);
	//FRotator Rot = FRotator(-90, 0, m_EmitterData->InitialRotation);

	//UDecalComponentEX* decal = SpawnCS3Decal(m_EmitterData->DecalMaterial, FVector(m_EmitterData->InitialHeight, m_EmitterData->InitialSize.X, m_EmitterData->InitialSize.Y), FVector(0, 0, 0), Rot, 0);

	//if (!decal)
	//{
	//	//CS3_ERROR(TEXT("-->Null Pointer error:ADecalEffect::_CreateDecal: decal!"));
	//	return;
	//}

	//decal->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//decal->bAbsoluteScale = false;

	//decal->SetRelativeScale3D(m_InitialSizeScale);
	//decal->SetVisibility(false);
	////ADecalEffect::Destroyed()中释放
	//m_DecalComps.Add(new FParticle(decal, m_EmitterData->Lifetime, decal->CreateDynamicMaterialInstance()));

}

void ADecalEffect::_StopEmit()
{
	if (IsValid(m_World))
	{
		m_World->GetTimerManager().ClearTimer(m_TimerHandle);
		m_World->GetTimerManager().ClearTimer(TimerHandle);
	}
}

UDecalComponentEX* ADecalEffect::SpawnCS3Decal(class UMaterialInterface* DecalMaterial, FVector DecalSize, FVector Location, FRotator Rotation, float LifeSpan)
{
	UWorld* World = GEngine->GetWorldFromContextObject(GetWorld(), EGetWorldErrorMode::LogAndReturnNull);
	AActor* Actor = Cast<AActor>(World->GetWorldSettings());
	UDecalComponentEX* DecalComp = NewObject<UDecalComponentEX>((Actor ? Actor : (UObject*)World));
	DecalComp->bAllowAnyoneToDestroyMe = true;
	//DecalComp->DecalMaterial = DecalMaterial;
	DecalComp->SetDecalMaterial(DecalMaterial);
	DecalComp->DecalSize = DecalSize;
	DecalComp->RegisterComponentWithWorld(World);

	if (LifeSpan > 0.f)
	{
		DecalComp->SetLifeSpan(LifeSpan);
	}
	DecalComp->SetWorldLocationAndRotation(Location, Rotation);
	return DecalComp;
}

//////////////////////////////////////////////////////////////////////////
#define CHECK_RET_F(exp) if (!exp)	return false;
#define CHECK_RET_T(exp) if (!exp)	return true;
FEmitterData::FEmitterData(UParticleEmitter* emit)
	: DecalMaterial(NULL),
	EmitterDuration(0),
	EmitterLoops(0),
	SpawnRate(0),
	BurstCount(0),
	InitialSize(0, 0, 0),
	InitialHeight(100.0f),
	Lifetime(0),
	InitialRotation(0.0f),
	InitialRotationRate(0.0f),
	Delay(0.0f)
{
	if (emit &&emit->LODLevels.Num() > 0)
	{
		UParticleLODLevel* LODLevel = emit->LODLevels[0];
		if (LODLevel)
		{
			if (!_ParseRequired(LODLevel->RequiredModule))
				return;
			if (!_ParseSpawn(LODLevel->SpawnModule))
				return;

			for (int j = 0; j < LODLevel->Modules.Num(); j++)
			{
				UParticleModule* module = LODLevel->Modules[j];
				if (_ParseInitialSizeModule(module)) continue;
				if (_ParseLifetime(module)) continue;
				if (_ParseColorOverLife(module)) continue;
				if (_ParseSizeByLife(module)) continue;
				if (_ParseInitialRotation(module)) continue;
				if (_ParseInitialRotationRate(module)) continue;
				if (_ParseInitialColorScaleOverLife(module)) continue;
				if (_ParseDynamicParameter(module)) continue;
			}
		}
	}
}

FEmitterData::FEmitterData(const FEmitterData* Other)
{
	DecalMaterial = Other->DecalMaterial;
	EmitterDuration = Other->EmitterDuration;
	EmitterLoops = Other->EmitterLoops;	// 是否是持续粒子 0持续 1只发射一次
	SpawnRate = Other->SpawnRate;	// 发射频率
	BurstCount = Other->BurstCount;	// 发射次数
	InitialSize = Other->InitialSize;
	InitialHeight = Other->InitialHeight;
	Lifetime = Other->Lifetime;
	ColorOverLifeDist = Other->ColorOverLifeDist;
	AlphaOverLifeDist = Other->AlphaOverLifeDist;
	SizeByLifeDist = Other->SizeByLifeDist;
	InitialRotation = Other->InitialRotation;
	InitialRotationRate = Other->InitialRotationRate;
	ColorScaleOverLifeDist = Other->ColorScaleOverLifeDist;
	AlphaScaleOverLifeDist = Other->AlphaScaleOverLifeDist;
	DynamicParam1Dist = Other->DynamicParam1Dist;
	Delay = Other->Delay;
	bOnMesh = Other->bOnMesh;
}

bool FEmitterData::_ParseRequired(UParticleModule* module)
{
	UParticleModuleRequired* RequiredModule = Cast<UParticleModuleRequired>(module);
	CHECK_RET_F(RequiredModule);

	DecalMaterial = RequiredModule->Material;

	Delay = RequiredModule->EmitterDelay /*+ Component->EmitterDelay*/;
	if (RequiredModule->bEmitterDelayUseRange)
	{
		const float	Rand = FMath::FRand();
		Delay = RequiredModule->EmitterDelayLow +
			((RequiredModule->EmitterDelay - RequiredModule->EmitterDelayLow) * Rand)/* + Component->EmitterDelay*/;
	}
	EmitterDuration = RequiredModule->EmitterDuration;
	EmitterLoops = RequiredModule->EmitterLoops;
	return true;
}

bool FEmitterData::_ParseSpawn(UParticleModule* module)
{
	UParticleModuleSpawn* SpawnModule = Cast<UParticleModuleSpawn>(module);
	CHECK_RET_F(SpawnModule);

	SpawnRate = SpawnModule->Rate.GetValue();
	if (SpawnModule->BurstList.Num() > 0)
	{
		BurstCount = SpawnModule->BurstList[0].Count;
	}
	return true;
}

bool FEmitterData::_ParseInitialSizeModule(UParticleModule* module)
{
	UParticleModuleSize* SizeModule = Cast<UParticleModuleSize>(module);
	CHECK_RET_F(SizeModule);
	CHECK_RET_T(module->bEnabled);
	InitialSize = SizeModule->StartSize.GetValue();
	InitialSize.Y = InitialSize.X;
	return true;
}

bool FEmitterData::_ParseLifetime(UParticleModule* module)
{
	UParticleModuleLifetime* LifetimeModule = Cast<UParticleModuleLifetime>(module);
	CHECK_RET_F(LifetimeModule);
	CHECK_RET_T(module->bEnabled);

	Lifetime = LifetimeModule->Lifetime.GetValue();
	return true;
}

bool FEmitterData::_ParseColorOverLife(UParticleModule* module)
{
	UParticleModuleColorOverLife* ColorModule = Cast<UParticleModuleColorOverLife>(module);
	CHECK_RET_F(ColorModule);
	CHECK_RET_T(module->bEnabled);
	ColorOverLifeDist = ColorModule->ColorOverLife;
	AlphaOverLifeDist = ColorModule->AlphaOverLife;
	return true;
}

bool FEmitterData::_ParseSizeByLife(UParticleModule* module)
{
	UParticleModuleSizeMultiplyLife* SizeByLifeModule = Cast<UParticleModuleSizeMultiplyLife>(module);
	CHECK_RET_F(SizeByLifeModule);
	CHECK_RET_T(module->bEnabled);
	SizeByLifeDist = SizeByLifeModule->LifeMultiplier;
	return true;
}

bool FEmitterData::_ParseInitialRotation(UParticleModule* module)
{
	UParticleModuleRotation* InitialRotationModule = Cast<UParticleModuleRotation>(module);
	CHECK_RET_F(InitialRotationModule);
	CHECK_RET_T(module->bEnabled);
	InitialRotation = InitialRotationModule->StartRotation.GetValue();
	return true;
}

bool FEmitterData::_ParseInitialRotationRate(UParticleModule* module)
{
	UParticleModuleRotationRate* RotationRateModule = Cast<UParticleModuleRotationRate>(module);
	CHECK_RET_F(RotationRateModule);
	CHECK_RET_T(module->bEnabled);
	InitialRotationRate = RotationRateModule->StartRotationRate.GetValue();
	return true;
}

bool FEmitterData::_ParseInitialColorScaleOverLife(UParticleModule* module)
{
	UParticleModuleColorScaleOverLife* ColorScaleModule = Cast<UParticleModuleColorScaleOverLife>(module);
	CHECK_RET_F(ColorScaleModule);
	CHECK_RET_T(module->bEnabled);// 此项不启用也设置，用来解决surface和decal材质类型不一样
	ColorScaleOverLifeDist = ColorScaleModule->ColorScaleOverLife;
	AlphaScaleOverLifeDist = ColorScaleModule->AlphaScaleOverLife;
	return true;
}

bool FEmitterData::_ParseDynamicParameter(UParticleModule* module)
{
	UParticleModuleParameterDynamic* ParamModule = Cast<UParticleModuleParameterDynamic>(module);
	CHECK_RET_F(ParamModule);

	if (ParamModule->DynamicParams.Num() > 0)
	{
		DynamicParam1Dist = ParamModule->DynamicParams[0].ParamValue;
		return true;
	}

	return false;
}

#pragma optimize("", on)
