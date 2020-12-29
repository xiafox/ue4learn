// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectEmitter.h"
// UE4
#include "GameFramework/Character.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleModule.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleEmitter.h"
// 数据类型

// CS3类型
#include "ClientTypes/ConvertUtil.h"
#include "ParticleModuleEx/ParticleModuleColorOverLifeEX.h"
#include "Character/Player/PlayerCharacter.h"
#include "../ParticleManager.h"
#include "Manager/ParticleManager/EmitterData/BaseSplineAsset.h"

#if WITH_EDITOR

#endif //WITH_EDITOR


// 光效显示的距离，10米和40米
#define NEARDIS 1000.f
#define FARDIS  4000.f


#pragma optimize("", off)

AEffectEmitter::AEffectEmitter()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	GetParticleSystemComponent()->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

///Spawn之后BeginPlay之前初始化
void AEffectEmitter::InitBeforeBeginPlay(FString InEffectID, int32 TempUID, AActor* TempFirstTarget, AActor* TempSecondTarget, float TempEffectSendAngle, const FEFFECT_EMITTER_DATA* TempEffectEmitterData, void* TempOnEffectCreate)
{
	// 根据粒子管理器的数据初始化粒子数据
	this->ID = InEffectID;							//当前的粒子ID
	this->UID = TempUID;								//当前粒子的UID	
	this->EffectSendAngle = TempEffectSendAngle;		//服务器传来的光效偏移，对路径生效
	this->EffectEmitterData = TempEffectEmitterData;


	// 根据配置调整光效的目标		
	this->FirstTarget = TempEffectEmitterData->EffectData.bSwap ? TempSecondTarget : TempFirstTarget;
	this->SecondTarget = TempEffectEmitterData->EffectData.bSwap ? TempFirstTarget : TempSecondTarget;
	
	// 光效创建的回调函数
	this->OnEffectCreate = TempOnEffectCreate;

	// 设置模板和资源
	SetTemplate(this->EffectEmitterData->ParticleSystemPtr.Get());
	// 不激活粒子
	GetParticleSystemComponent()->SetActive(false);

	// 根据资源动态得添加组件
	if (TempEffectEmitterData->ModelData.CollisionMesh.Get())
	{
		STComponent = NewObject<UStaticMeshComponent>(this, TEXT("STComponent"));
		STComponent->RegisterComponent();
		STComponent->AttachToComponent(GetParticleSystemComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		STComponent->SetHiddenInGame(true);
		STComponent->OnComponentBeginOverlap.AddDynamic(this, &AEffectEmitter::OnEmitterCollisionBegin);
		STComponent->OnComponentEndOverlap.AddDynamic(this, &AEffectEmitter::OnEmitterCollisionEnd);
		STComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		STComponent->bReceivesDecals = false;
		STComponent->ComponentTags.Add("NoUpdateVisible");

		STComponent->SetStaticMesh(TempEffectEmitterData->ModelData.CollisionMesh.Get());
		STComponent->SetRelativeTransform(TempEffectEmitterData->ModelData.CollisionMeshTransform);
	}

	if (TempEffectEmitterData->ModelData.AnimMesh.Get())
	{
		SKComponent = NewObject<USkeletalMeshComponent>(this, TEXT("SKComponent"));
		SKComponent->RegisterComponent();
		SKComponent->AttachToComponent(GetParticleSystemComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		SKComponent->SetHiddenInGame(true);
		SKComponent->OnComponentBeginOverlap.AddDynamic(this, &AEffectEmitter::OnEmitterCollisionBegin);
		SKComponent->OnComponentEndOverlap.AddDynamic(this, &AEffectEmitter::OnEmitterCollisionEnd);
		SKComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SKComponent->bReceivesDecals = false;

		SKComponent->SetSkeletalMesh(TempEffectEmitterData->ModelData.AnimMesh.Get());
		SKComponent->SetRelativeTransform(TempEffectEmitterData->ModelData.AnimMeshTransform);
	}
}

void AEffectEmitter::PrepareData()
{
	//CS3_DEBUG(TEXT("particle system : Particle UID named %d is beginplay"), UID);

	// 上一波操作中光效可能被隐藏，准备数据的时候重置，下面还会再次设置可见性
	if (!GetParticleSystemComponent()->bVisible)
	{
		GetParticleSystemComponent()->SetVisibility(true);
	}
	if (bHidden)
	{
		SetActorHiddenInGame(false);
	}

	// 初始与配置不一致的数据
	InitFromConfig();

	// 在编辑器模式下播放，给粒子在世界大纲添加标签
#if WITH_EDITOR
	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		SetActorLabel(ActorName.ToString() + FString("(") + ID + FString(")") + FString("(") + INT_TO_FSTRING(UID) + FString(")"));
	}
#endif

	// 初始化位置
	InitEmitterTransform();

	// 添加粒子数据到管理器
	AddDataToManager();

	// 获取设置服务器传来的光效骨骼插槽
	/*if (Cast<AServerCharacter>(FirstTarget))
	{
		Cast<AServerCharacter>(FirstTarget)->GetEffectDynamicData(FirstTransform->Socket, FirstTransform->Transform);
	}*/

	// 判断此光效是否要打开Tick
	if (EffectData->bGraduallyHidden || Spline || EffectData->bUseConstAlpha || EffectData->bSortEffect || EffectData->bAlwaysRender)
	{
		SetActorTickEnabled(true);

		// 是否要进行透明处理
		if (EffectData->bGraduallyHidden || EffectData->bUseConstAlpha)
		{
			AddAlphaModule();
		}
	}

	// 该粒子是否需要排序
	if (EffectData->bSortEffect)
	{
		UClientGameInstance::Instance->ParticleManager->AddToSortList(UID);
	}

	// 绑定光效抵达回掉事件
	OnEffectArrived.AddDynamic(this, &AEffectEmitter::OnEmitterEffectArrived);

	// 触发回调，特殊光效可以用来后期设置位置使用
	(*(const FEffectCreate*)OnEffectCreate).ExecuteIfBound(UID);

	// 播放粒子
	Play();
}

void AEffectEmitter::ClearData()
{
	// 清除路径资源
	if (IsValid(Spline))
	{
		Spline->Destroy();
		Spline = nullptr;
	}

	// 该粒子是否添加了动态模块，移除模块
	if (EffectData->bGraduallyHidden || EffectData->bUseConstAlpha)
	{
		RemoveModule();
	}

	// 该粒子是否需要排序,在没有经过Stop销毁时，做的处理
	if (EffectData->bSortEffect)
	{
		UClientGameInstance::Instance->ParticleManager->RemoveToSortList(UID);
	}

	// 清除管理器数据
	RemoveDataFormManager();

	//CS3_DEBUG(TEXT("particle system :Particle UID named %d is endplay"), UID);
}

void AEffectEmitter::AddDataToManager()
{
	// 把光效加入管理器列表
	UParticleManager* ParticleManager = UClientGameInstance::Instance->ParticleManager;
	FInternal_EffectData& EffectInternalData = ParticleManager->UEffectMap[UID];
	EffectInternalData.EffectInstance = EffectEmitterData->EffectInstance.Get();
	EffectInternalData.Parent = SetEffectVisible();
	EffectInternalData.Owner = this;

	// 根据配置更新光效可见性
	UpdateVisibilityData(true);
}

void AEffectEmitter::RemoveDataFormManager()
{
	// 根据配置更新光效可见性
	UpdateVisibilityData(false);

	// 粒子移除列表，UEffectMap是在管理器中添加的
	UParticleManager* ParticleManager = UClientGameInstance::Instance->ParticleManager;
	ParticleManager->UEffectMap.Remove(UID);


	// 粒子管理器中已经不存在这个UID了，改为-1
	UID = -1;
}

void AEffectEmitter::ClearDataAndAddToBufferPool()
{
	// 清空所有数据，放到粒子缓冲池中
	OnEffectStop.Clear();
	OnEffectArrived.Clear();
	ChildrenEffectUID.Empty();

	CurrentTime = 0.0f;
	OnEffectCreate = nullptr;
	MoveSpeed = 0.0f;
	AnimAsset.Empty();
	ModuleList.Empty();

	UParticleSystemComponent* ParticleComponent = GetParticleSystemComponent();
	ParticleComponent->SetTemplate(nullptr);
	ParticleComponent->SetAbsolute(false, false, false);
	ParticleComponent->SetRelativeTransform(FTransform());
	ParticleComponent->SetComponentTickEnabled(false);


	if (STComponent)
	{
		// 加入缓存池
		STComponent->DestroyComponent();
		STComponent = nullptr;
	}

	if (SKComponent)
	{
		// 加入缓存池
		SKComponent->DestroyComponent();
		SKComponent = nullptr;
	}

	// 清空额外的组件
	TSet<UActorComponent*> Components = GetComponents();
	Components.Remove(GetRootComponent());
	Components.Remove(GetParticleSystemComponent());
	for (auto Component : Components)
	{
		Component->DestroyComponent();
	}
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorScale3D(FVector(1.f));
	
	//UClientGameInstance::Instance->CustomTimerManager->ClearAllTimersForObject(this);
	SetActorTickEnabled(false);
	UClientGameInstance::Instance->ParticleManager->AddToParticleBufferPool(this);

#if WITH_EDITOR
	// 被回收的光效恢复原来的名字，在下一次Tick的时候改变，在GC中使用SetActorLabel会导致崩溃
	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		TWeakObjectPtr<AEffectEmitter> ThisPtr(this);
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([ThisPtr]()
		{
			if (ThisPtr.IsValid())
			{
				AEffectEmitter* Effect = ThisPtr.Get();
				// 是否在缓冲池中
				if (UClientGameInstance::Instance->ParticleManager->CheckOnBufferPool(Effect))
				{
					Effect->SetActorLabel(Effect->ActorName.ToString());
				}
			}
		}));
	}
#endif
	//CS3_DEBUG(TEXT("particle add to buffer pool : %s,%d"), *ID, UID);
}

void AEffectEmitter::BeginPlay()
{
	//if (IsValid(UClientGameInstance::Instance))
	//{
	//	OnEndPlay.AddUniqueDynamic(UClientGameInstance::Instance, &UClientGameInstance::OnActorEndPlay);
	//}

#if WITH_EDITOR
	// 编辑器下保存下Actor的名字，用于设置世界大纲显示的名称
	ActorName = GetFName();

	bDefer = false;
#endif
	// 关闭Tick
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	// 准备所有的粒子数据
	PrepareData();

	Super::BeginPlay();
}

void AEffectEmitter::AddAlphaModule(UParticleSystemComponent* ParticleComponent/* = nullptr*/)
{
	if (!ParticleComponent)
	{
		ParticleComponent = GetParticleSystemComponent();
	}
	if (!ParticleComponent->Template)
		return;
	//给粒子添加透明度模块
	UParticleEmitter* CurEmitters;
	UParticleLODLevel* CurLOD;
	for (int i = 0; i < ParticleComponent->Template->Emitters.Num(); i++)
	{
		CurEmitters = ParticleComponent->Template->Emitters[i];
		
		for (int j = 0; j < CurEmitters->LODLevels.Num(); j++)
		{
			CurLOD = CurEmitters->LODLevels[j];

			if (CurLOD->Modules.Num())
			{
				if (!CurLOD->Modules[CurLOD->Modules.Num() - 1]->IsA(UParticleModuleColorOverLifeEX::StaticClass()))
				{
					UParticleModuleColorOverLifeEX* NewModule = NewObject<UParticleModuleColorOverLifeEX>(CurEmitters->GetOuter());
					// 添加模块到管理列表
					AddModule(NewModule, i, j);
					CurLOD->Modules.Add(NewModule);
					CurLOD->SpawnModules.Add(NewModule);
					CurLOD->UpdateModules.Add(NewModule);
				}
				else
				{
					// 添加模块到管理列表
					AddModule(CurLOD->Modules[CurLOD->Modules.Num() - 1], i, j);
				}
			}
		}
	}
}

AActor* AEffectEmitter::SetEffectVisible()
{
	AActor* Target = FirstTarget;
	AServerCharacter* ServerTarget = Cast<AServerCharacter>(Target);

	bool visible = false;				// 默认显示，因为使用hidden设置的，所以相反

	if (FirstTransform->bBindSocket)	// 判断是否绑定
	{
		Target = nullptr;
	}
	
	if (EffectData->bBindingVisibility)	// 非绑定光效是否绑定可见性
	{
		if (ServerTarget)				// 判断是否是服务器对象
		{
		//	visible = ServerTarget->VisibleStyle == VISIBLE_STYLE::VISIBLE_STYLE_FALSE;
		}
		else if (Target)
		{
			visible = Target->bHidden;
		}
	}

	SetActorHiddenInGame(visible);

	if (!visible && STComponent)
	{
		STComponent->SetVisibility(EffectEmitterData->ModelData.bCollisionMeshVisibility);
		STComponent->SetHiddenInGame(false);
	}

	return Target;
}

void AEffectEmitter::InitFromConfig()
{
	EffectData = &(EffectEmitterData->EffectData);
	
	// 位置
	FirstTransform = &(EffectData->bSwap ? EffectData->SecondTransform : EffectData->FirstTransform);
	SecondTransform = &(EffectData->bSwap ? EffectData->FirstTransform : EffectData->SecondTransform);

	// 生命周期
	LifeTime			= EffectData->LifeTime < 0.f ? 99999.f : EffectData->LifeTime;

	// 透明度
	FadeInTime			= EffectData->FadeInTime < 0 ? 0: EffectData->FadeInTime;
	FadeInTime			= FadeInTime <= LifeTime ? FadeInTime : LifeTime;
	FadeOutTime			= EffectData->FadeOutTime < 0 ? LifeTime : EffectData->FadeOutTime;
	FadeOutTime			= LifeTime - FadeOutTime >= FadeInTime ? FadeOutTime : LifeTime - FadeInTime;
	Alpha				= FMath::Abs(FadeInTime - 0.0f) < 0.01f || !EffectData->bGraduallyHidden ? 1.0f : 0.0f;

	// Mesh相关
	for (auto CurrentAsset : EffectEmitterData->ModelData.AnimAsset)
	{
		AnimAsset.Add(CurrentAsset.Get());
	}

	// 路径
	if (EffectEmitterData->SplineAsset.SplineAsset)
	{
		// 生成路径实例
		InitSplineFromConfig();
	}
}

void AEffectEmitter::InitSplineFromConfig()
{
	const FSPLINE_ASSET& SplineData = EffectEmitterData->SplineAsset;
	FTransform Transform = FTransform();
	Transform.SetLocation(GetActorLocation());
	Transform.SetRotation(GetActorQuat());

	// DeferredSpawn
	Spline = Cast<ABaseSplineAsset>(UClientBPFunctionLibrary::BeginDeferredActorSpawnFromClass(SplineData.SplineAsset.Get(), Transform));
	if (Spline)
	{
		// 在BeginPlay之前初始化一些必要的数据
		Spline->LoopNumber = SplineData.LoopNumber ? SplineData.LoopNumber : 99999999;
		Spline->MoveSpeed = MoveSpeed = SplineData.MoveSpeed;
		Spline->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Spline->SplineComponent->SetAbsolute(SplineData.bAbsoluteLocation, SplineData.bAbsoluteRotation, SplineData.bAbsoluteScale);
		UGameplayStatics::FinishSpawningActor(Spline, Transform);
	}
}

void AEffectEmitter::InitEmitterTransform()
{
	// 设置位置，默认情况下是按给受术者的位置进行设置，是SecondTarget，当然总有例外，后续可能会扩展
	// 目前是按照受术者定义

	ACharacter* Character = Cast<ACharacter>(SecondTarget);
	if (SecondTransform->bBindSocket)
	{
		AttachToComponent(Character ? Character->GetMesh() : SecondTarget->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, FName(*SecondTransform->Socket));
		SetActorRelativeTransform(SecondTransform->Transform, true);
	}
	else
	{
		//不绑定的光效，如果是Charcter设置在Mesh组件的位置，如果是Actor，设置在Actor根组件的位置
		SetActorTransform(Character ? Character->GetMesh()->GetSocketTransform(FName(*SecondTransform->Socket)) : SecondTarget->GetActorTransform());
		AddActorLocalOffset(SecondTransform->Transform.GetLocation());
		AddActorLocalRotation(SecondTransform->Transform.GetRotation());
		SetActorScale3D(SecondTransform->Transform.GetScale3D());
	}

	//如果不是Character，给光效加90°的偏移，因为骨骼和胶囊体的坐标不一致，骨骼相对于胶囊体旋转了-90°
	if (!Character)
	{
		AddActorLocalRotation(FRotator(0, -90, 0));
	}

	//在改变变换方式前保存当前的位置，下面涉及到绑定关系
	FVector CurLocation = GetActorLocation();

	//设置变换方式
	this->GetRootComponent()->SetAbsolute(SecondTransform->bLocationOnWorld, SecondTransform->bRotationOnWorld, SecondTransform->bScaleOnWorld);

	//绑定的光效在绝对位置的情况下为目标位置，不绑定光效在绝对位置下位所填的位置
	if (SecondTransform->bLocationOnWorld)
	{
		if (SecondTransform->bBindSocket)
		{
			SetActorLocation(CurLocation);
		}
		else
		{
			SetActorLocation(SecondTransform->Transform.GetLocation());
		}
	}

	//不绑定的光效在绝对旋转的情况下为所填的旋转
	if (SecondTransform->bRotationOnWorld && !SecondTransform->bBindSocket)
	{
		SetActorRotation(SecondTransform->Transform.GetRotation());
	}

	//没有对缩放进行处理，如果有需要，可以做处理

	//表现组件中添加此光效，为了在Actor销毁时，正确的销毁此对象
	if (Cast<AGameCharacter>(SecondTarget) && (SecondTransform->bBindSocket || EffectData->bBindingVisibility))
	{
		//Cast<AGameCharacter>(SecondTarget)->GetPerformanceComponent()->AddEffect(UID);
	}

	//有路径资源的光效在蓝图中初始化位置
	if (IsValid(Spline))
	{
		InitSplineTransform();
	}
}

void AEffectEmitter::InitSplineTransform()
{
	const FSPLINE_ASSET& SplineData = EffectEmitterData->SplineAsset;

	if (SplineData.bAbsoluteLocation)
	{
		Spline->SetActorLocation(GetTargetLocation());
	}

	if (SplineData.bAbsoluteRotation)
	{
		Spline->SetActorRotation(GetActorRotation());
	}

	Spline->AddActorWorldRotation(FRotator(0, EffectSendAngle, 0));

	// 初始化粒子的位置，在曲线的起点
	if (IsValid(Spline->SplineComponent))
	{
		Spline->MoveOnSpline(GetParticleSystemComponent(), true, 0.0f);
	}
}

void AEffectEmitter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UID != -1)
	{
		ClearData();
	}

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
void AEffectEmitter::Destroyed()
{
	if (UClientGameInstance::Instance && bDefer)
	{
		RemoveFromRoot();
		UClientGameInstance::Instance->ParticleManager->RemoveFromParticleDeferSpawnPool(this);
	}
	Super::Destroyed();
}
#endif

void AEffectEmitter::Play()
{

#if WITH_EDITOR
	if (bIsInEdit)
	{
		StartPlay();
		return;
	}
#endif // WITH_EDITOR


	if (bPlaying)
	{
		return;
	}

	if (EffectData->DelayPlayTime > 0)
	{
		//延时播放
		FTimerHandle DelayHandle;
		if (IsValid(UClientGameInstance::Instance))
		{
			//UClientGameInstance::Instance->CustomTimerManager->SetTimer(DelayHandle, this, &AEffectEmitter::StartPlay, EffectData->DelayPlayTime, false);
		}
	}
	else
	{
		StartPlay();
	}
}

void AEffectEmitter::OnPlay()
{

}


void AEffectEmitter::PlayAnimation()
{
	if (!SKComponent)
	{
		return;
	}

	if (AnimAsset.Num() && AnimAsset[0])
	{
		//播放动作
		SKComponent->PlayAnimation(AnimAsset[0], true);
		SKComponent->SetPlayRate(EffectEmitterData->ModelData.PlayRate);
	}
	SKComponent->SetHiddenInGame(false);
}

void AEffectEmitter::SetBuffBulletData(const int32 BuffIndex, const int32 ProxyID, const int32 OwnerID, const int32 EffectIndex,const int32 EffectCustomIndex)
{
	BulletBuffIndex = BuffIndex;
	BulletBuffProxyId = ProxyID;
	BulletBuffOwnerId = OwnerID;
	BulletBuffEffectIndex = EffectIndex;
	BulletBuffEffectCustomIndex = EffectCustomIndex;
}

void AEffectEmitter::Stop()
{
	//在播放状态才可以停止
	if (!bPlaying)
	{
		return;
	}

	//先关闭碰撞，触发光效离开碰撞逻辑
	SetMeshComponentCollision(false);

	//停止播放
	/*if (IsValid(UClientGameInstance::Instance) && IsValid(UClientGameInstance::Instance->AudioManager))
	{
		UClientGameInstance::Instance->AudioManager->Stop3DSound(AudioComponent);
		AudioComponent = nullptr;
	}	*/

	bPlaying = false;

	// 停止播放粒子
	GetParticleSystemComponent()->SetActive(false);

	//光效停止的回调
	OnEffectStop.Broadcast();

	// 销毁子对象
	if (EffectData->bDestroyChildren)
	{
		for (auto EffectUID : ChildrenEffectUID)
		{
			UClientGameInstance::Instance->ParticleManager->StopEffectByUID(EffectUID);
		}
	}

	// 如果有路径，把光效本身设置到粒子组件的位置，此时粒子已经播放完毕，碰撞已经关闭，改变位置亦不会有其他影响
	if (Spline)
	{
		//CS3_DEBUG(TEXT("particle system : Reset particle location beacuse the particle has spline asset! particle id: %s"), *ID);
		SetActorLocation(GetParticleSystemComponent()->GetComponentLocation());
		GetParticleSystemComponent()->SetRelativeLocation(FVector::ZeroVector);
	}

	// 播放死亡光效
	PlayDeathParticle();

	// 销毁或者延时销毁
	if (EffectData->DelayDestroyTime > 0.0f)//延时销毁
	{
		FTimerHandle DelayDestroyHandle;
		if (IsValid(UClientGameInstance::Instance))
		{
			//UClientGameInstance::Instance->CustomTimerManager->SetTimer(DelayDestroyHandle, this, &AEffectEmitter::DestroyEffect, EffectData->DelayDestroyTime, false);
		}
		DelayDestroySetting();
	}
	else
	{
		DestroyEffect();
	}

	//CS3_DEBUG(TEXT("particle system : stop particle! particle id: %s"), *ID);
}

void AEffectEmitter::StartPlay()
{

#if WITH_EDITOR
	if (bIsInEdit)
	{
		PlayAnimation();					//播放动画
		Activate();							//播放粒子
		PlayChildParticle();				//播放子光效
		return;
	}
#endif

	// 如果是0秒光效，强制给一个时间0.01秒，我们走正规流程
	LifeTime = FMath::Abs(LifeTime - 0.0f) > 0.01f ? LifeTime : 0.01f;

	if (LifeTime > 0.0f)
	{
		// 延时销毁
		FTimerHandle DelayStopHandle;
		if (UClientGameInstance::Instance)
		{
			//UClientGameInstance::Instance->CustomTimerManager->SetTimer(DelayStopHandle, this, &AEffectEmitter::Stop, LifeTime, false);
		}
	}


	bPlaying = true;													//设置播放状态
	PlayAnimation();													//播放动画
	GetParticleSystemComponent()->SetActive(true, true);				//播放粒子
	PlayChildParticle();												//播放子光效
	SetMeshComponentCollision(true);									//开启碰撞

	// 播放粒子附带的音效
	/*if (IsValid(UClientGameInstance::Instance->AudioManager))
	{
		AudioComponent = UClientGameInstance::Instance->AudioManager->Play3DSoundByAsset(this, EffectEmitterData->AudioData.AudioAsset.Get(), FVector::ZeroVector, EAUDIOCOMPTYLE::NONE, EffectEmitterData->AudioData.bLoop, GetParticleSystemComponent());
	}*/
}

void AEffectEmitter::OnEmitterCollisionBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*光效的具体碰撞结果SweepResult无效值，只是进行了碰撞检测，如需要具体结果，请在STComponent移动时把bSweep属性设置为true，但是消耗会变大，必要时可配置处理*/

	if (!bPlaying) return;

	// 判断碰撞的时机
	for (auto CollisionData : EffectEmitterData->CollisionResponse)
	{	// 判断碰撞类型
		if (CollisionData.ResponseType == RESPONSE_TYPE::BEGIN_COLLISION)
		{
			if (!CollisionData.ActorType || Other->IsA(CollisionData.ActorType))
			{
				AServerCharacter* ServerCharacter = Cast<AServerCharacter>(Other);
				// 判断服务器类型
				if ((CollisionData.EntityType != "" && ServerCharacter && ServerCharacter->GetSelfEntity() && *ServerCharacter->GetSelfEntity()->ClassName() == CollisionData.EntityType) || CollisionData.EntityType == "")
				{	// 循环触发方法
					for (auto MethodData : CollisionData.MethodList)
					{
						KBEngine::FVariantArray args;
						// 循环填充参数
						for (auto MethodParams : MethodData.ParamsList)
						{
							if (MethodParams == PARAMS_TYPE::BULLETBUFF && (Cast<UCapsuleComponent>(OtherComp) || (ServerCharacter && OtherComp == ServerCharacter->STMesh_DefaultStatic)))
							{
								CollisionList.Add(Other, 0.0f);
								AddBulletMsgBegin(&args, ServerCharacter);
							}
							else if (MethodParams == PARAMS_TYPE::OtherEntityID)
							{
								args.Add(INT_TO_FSTRING(GetEntityID(ServerCharacter)));
							}
							else if (MethodParams == PARAMS_TYPE::SKILLID)
							{
								args.Add(INT_TO_FSTRING(CollisionData.ResponseSkillID));
							}
						}
						
						//DrawDebugPoint(GetWorld(), SweepResult.ImpactPoint, 5, FColor::Green, false, 50);
						//DrawDebugBox(GetWorld(), OverlappedComp->GetComponentLocation(), Cast<UStaticMeshComponent>(OverlappedComp)->GetStaticMesh()->GetBounds().BoxExtent * 2 * OverlappedComp->GetComponentScale(), OverlappedComp->GetComponentRotation().Quaternion(), FColor::Red, false, 2.f);
						//CS3_DEBUG(TEXT("Drive back to sleep enter:%s,%s,%s"), *OtherComp->GetName(), *Other->GetName(), *OverlappedComp->GetName());
						
						// 调用服务器方法
						KBEngine::Entity* Player = UClientBPFunctionLibrary::GetPlayer();
						ServerGameEntity* pEntity_Player = (ServerGameEntity*)(Player);
						if (pEntity_Player && pEntity_Player->IsPlayerRole())
						{
							// 如果不是子弹Buff，并且碰撞的对象是玩家且不是本地玩家，就不发送
							if (!MethodData.ParamsList.Contains(PARAMS_TYPE::BULLETBUFF) && Cast<APlayerCharacter>(ServerCharacter) && ServerCharacter->EntityId != pEntity_Player->ID())
							{
								continue;
							}
							pEntity_Player->CellCall(MethodData.MethodName, args);
						}
					}
				}
			}
		}
	}
}

void AEffectEmitter::OnEmitterCollisionEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bPlaying) return;

	//移除子弹Buff对象
	if (Cast<AServerCharacter>(Other) && (Cast<UCapsuleComponent>(OtherComp) || OtherComp == Cast<AServerCharacter>(Other)->STMesh_DefaultStatic))
	{
		CollisionList.Remove(Other);
		//CS3_DEBUG(TEXT("Drive back to sleep leave:%s,%s,%s"), *OtherComp->GetName(), *Other->GetName(), *OverlappedComp->GetName());
	}

	//判断碰撞的时机
	for (auto CollisionData : EffectEmitterData->CollisionResponse)
	{	//判断碰撞类型
		if (CollisionData.ResponseType == RESPONSE_TYPE::END_COLLISION)
		{
			if (!CollisionData.ActorType || Other->IsA(CollisionData.ActorType))
			{
				AServerCharacter* ServerCharacter = Cast<AServerCharacter>(Other);
				//判断服务器类型
				if ((CollisionData.EntityType != "" && ServerCharacter && ServerCharacter->GetSelfEntity() && *ServerCharacter->GetSelfEntity()->ClassName() == CollisionData.EntityType) || CollisionData.EntityType == "")
				{	//循环触发方法
					for (auto MethodData : CollisionData.MethodList)
					{
						KBEngine::FVariantArray args;
						//循环填充参数
						for (auto MethodParams : MethodData.ParamsList)
						{
							if (MethodParams == PARAMS_TYPE::OtherEntityID)
							{
								args.Add(INT_TO_FSTRING(GetEntityID(ServerCharacter)));
							}
						}
						//调用服务器方法
						KBEngine::Entity* Player = UClientBPFunctionLibrary::GetPlayer();
						ServerGameEntity* pEntity_Player = (ServerGameEntity*)(Player);
						if (pEntity_Player && pEntity_Player->IsPlayerRole())
						{
							// 如果不是子弹Buff，并且碰撞的对象是玩家且不是本地玩家，就不发送
							if (!MethodData.ParamsList.Contains(PARAMS_TYPE::BULLETBUFF) && Cast<APlayerCharacter>(ServerCharacter) && ServerCharacter->EntityId != pEntity_Player->ID())
							{
								continue;
							}
							pEntity_Player->CellCall(MethodData.MethodName, args);
						}
					}
				}
			}
		}
	}
	
	//碰撞结束处理（暂不启用）
	
	//if (BuffProxyId == -1)
	//{
	//	if (UClientBPFunctionLibrary::GetPlayerCharacter() == Other)
	//	{
	//		/*TArray<FString> Arguments;
	//		FString MethodName = TEXT("onBulletCollisionEnd");
	//		Arguments.Add(INT_TO_FSTRING(BuffOwnerId));
	//		Arguments.Add(INT_TO_FSTRING(UClientBPFunctionLibrary::GetPlayerID()));
	//		UClientBPFunctionLibrary::CellCall(UClientBPFunctionLibrary::GetPlayerID(), MethodName, Arguments);*/
	//	}
	//}
	//else if (BuffProxyId == UClientBPFunctionLibrary::GetPlayerID())
	//{
	//	if (AServerCharacter* Target = Cast<AServerCharacter>(Other))
	//	{
	//		/*TArray<FString> Arguments;
	//		FString MethodName = TEXT("onBulletCollisionEnd");
	//		Arguments.Add(INT_TO_FSTRING(BuffOwnerId));
	//		Arguments.Add(INT_TO_FSTRING(Target->EntityId));
	//		UClientBPFunctionLibrary::CellCall(UClientBPFunctionLibrary::GetPlayerID(), MethodName, Arguments);*/
	//	}
	//}
	
}

void AEffectEmitter::PlayChildParticle()
{
#if WITH_EDITOR
	if (bIsInEdit)
	{
		UParticleManager* ParticleManager = NewObject<UParticleManager>();

		if (nullptr != ParticleManager)
		{
			for (FString ChildID : EffectData->ChildrenEffect)
			{
				/*const FEFFECT_EMITTER_DATA* TempEffectData = ParticleManager->GetEffectData(ChildID);
				ASpawnBase* SpawnBase = Cast<ASpawnBase>(FirstTarget);

				if (SpawnBase && TempEffectData)
				{
					AEffectEmitter* Effect= ParticleManager->PlayParticleOfEdit(FSTRING_TO_FNAME(ChildID), FirstTarget);
					if (Effect)
					{
						SpawnBase->CurEffect.Add(Effect);
					}
				}*/
			}
		}

		return;
	}
#endif //WITH_EDITOR

	// 子光效会跟随销毁
	int32 ChildUID;
	for (FString ChildID : EffectData->ChildrenEffect)
	{
		if (IsValid(UClientGameInstance::Instance))
		{
			ChildUID = UClientGameInstance::Instance->ParticleManager->PlayParticle(ChildID, 0.0f, FirstTarget, SecondTarget);
		}
		this->ChildrenEffectUID.Add(ChildUID);
	}
}

void AEffectEmitter::PlayDeathParticle()
{
	// 死亡光效不会跟随销毁，所以把目标给了世界
	FVector Location;
	for (auto DeathID : EffectData->DeathEffect)
	{
		Location = this->GetActorLocation();

		if (IsValid(UClientGameInstance::Instance))
			UClientGameInstance::Instance->ParticleManager->PlayParticle(DeathID, 0.0f, GetWorld()->GetWorldSettings(), GetWorld()->GetWorldSettings(), FEffectCreate::CreateLambda([Location](int32 UID)
		{
			AActor* Effect = UClientGameInstance::Instance->ParticleManager->FindEmitterByUID(UID);
			if (Effect)
			{
				Effect->AddActorWorldOffset(Location);
			}
		}));
	}
}

void AEffectEmitter::DestroyEffect()
{
	// 获取粒子的类，直接通过配置表get会崩溃
	UClass* Class = nullptr;
	if (UClientGameInstance::Instance->ParticleManager->UEffectMap.Contains(UID))
	{
		Class = UClientGameInstance::Instance->ParticleManager->UEffectMap[UID].EffectInstance;
	}

	ClearData();

	if (UClientGameInstance::Instance->ParticleManager->IsCacheClass(Class))
	{
		ClearDataAndAddToBufferPool();
	}
	else
	{
		Destroy();
	}
}

void AEffectEmitter::OnEmitterEffectArrived()
{
	RPC_onBulletCollisionArrive();
}

void AEffectEmitter::RPC_onBulletCollisionArrive()
{
	KBEngine::FVariantArray args;
	AddBulletMsgBegin(&args, SecondTarget);

	if (!IsValid(UClientGameInstance::Instance))
	{
		return;
	}
		
	KBEngine::Entity* Player = UClientBPFunctionLibrary::GetPlayer();
	ServerGameEntity* pEntity_Player = (ServerGameEntity*)(Player);

	if (pEntity_Player && pEntity_Player->IsPlayerRole())
	{
		/*USkillInterface * SkillInterface = Cast<USkillInterface>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("SkillInterface"), pEntity_Player->ID()));
		if (SkillInterface)
		{
			SkillInterface->onBulletCollisionArrive(args);
		}*/
		//pEntity_Player->CellCall(TEXT("onBulletCollisionArrive"), args);
		//CS3_DEBUG(TEXT("Drive back to sleep arrived:%s"), *ID);
	}
}

void AEffectEmitter::SetMeshComponentCollision(bool State)
{
	// 只是设置了STComponent，因为只有STComponent做碰撞检测
	if (!STComponent)
	{
		return;
	}

	if (State)
	{
		STComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		STComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		STComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEffectEmitter::UpdateTransparency(float DeltaSeconds)
{
	if (EffectData->bGraduallyHidden)
	{
		if (FadeInTime > CurrentTime)
		{
			// 淡入
			Alpha += DeltaSeconds / FadeInTime;
		}
		else if (LifeTime - CurrentTime < FadeOutTime)
		{
			// 淡出
			Alpha -= DeltaSeconds / FadeOutTime;
		}
		else
		{
			Alpha = 1.0f;
		}
	}
}

void AEffectEmitter::Tick(float DeltaTime)
{
	if (bPlaying)
	{
		CurrentTime += DeltaTime;

		// 持续更新粒子发射器数据
		if (EffectData->bAlwaysRender && GetWorld()->TimeSeconds > GetParticleSystemComponent()->LastRenderTime)
		{
			GetParticleSystemComponent()->LastRenderTime = 999.f;		//设置上次渲染的时间
		}
		
		// 粒子排序
		if (EffectData->bSortEffect)
		{
			SortEffect();
		}

		// 光效逐渐消散
		UpdateTransparency(DeltaTime);

		// 持续响应子弹Buff受击
		CycleResponseCollisionEvent(DeltaTime);
		
		// 光效在路径上移动
		if (IsValid(Spline) && IsValid(Spline->SplineComponent))
		{
			Spline->MoveOnSpline(GetParticleSystemComponent(), true, DeltaTime);
		}
	}

	Super::Tick(DeltaTime);
}

void AEffectEmitter::CycleResponseCollisionEvent(float DeltaTime)
{
	const TArray<FEFFECT_COLLISION_RESPONSE>& CollisionResponseData = EffectEmitterData->CollisionResponse;

	// 这个地方只有第一份数据是保持的是否循环触发
	if (CollisionResponseData.Num() && CollisionResponseData[0].bLoopResponse)
	{
		// 子弹Buff持续受击判断
		for (TMap<AActor*, float>::TIterator TBegin = CollisionList.CreateIterator(); TBegin; ++TBegin)
		{
			if (IsValid(TBegin->Key))
			{
				TBegin->Value += DeltaTime;
				if (TBegin->Value > CollisionResponseData[0].ResponseCycle)
				{
					KBEngine::FVariantArray args;
					AddBulletMsgBegin(&args, TBegin->Key);

					KBEngine::Entity* Player = UClientBPFunctionLibrary::GetPlayer();
					ServerGameEntity* pEntity_Player = (ServerGameEntity*)(Player);
					/*if (pEntity_Player && pEntity_Player->IsPlayerRole())
					{
						USkillInterface * SkillInterface = Cast<USkillInterface>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("SkillInterface"), pEntity_Player->ID()));
						if (SkillInterface)
						{
							SkillInterface->RPC_onBulletCollisionBegin(args);
						}
					}*/
					TBegin->Value = 0.0f;
				}
			}
			else
			{
				TBegin.RemoveCurrent();
			}
		}
	}
}

void AEffectEmitter::DelayDestroySetting()
{
	//停止播放动作
	if (SKComponent)
	{
		SKComponent->Stop();
	}
}

void AEffectEmitter::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	///隐藏状态下设置为激活没有用，在刷新粒子的可见性时再次激活粒子
	if (!bNewHidden && bPlaying && !IsActive())
	{
		Activate();
	}
}

void AEffectEmitter::UpdateVisibilityData(bool bCreate)
{
	UParticleManager* ParticleManager = UClientGameInstance::Instance->ParticleManager;
	FInternal_EffectData& Data = ParticleManager->UEffectMap[UID];
	

//-------------------------移除------------------------------
	if (!bCreate)
	{
		// 只有显示的算进去
		if (Data.bVisible)
		{
			FPARTICLE_DIAPLAY_CONFIG& DisPlayData = ParticleManager->CurrentParticleDisplayConfig;
			// 移除粒子
			switch (Data.LODType)
			{
			case LOD_TYPE::NEAR_DIS:
				DisPlayData.NEARCount[Data.Type] -= 1;
				break;
			case LOD_TYPE::MIDDEL_DIS:
				DisPlayData.MIDDELCount[Data.Type] -= 1;
				break;
			case LOD_TYPE::FAR_DIS:
				DisPlayData.FARCount[Data.Type] -= 1;
				break;
			default:
				break;
			}
			DisPlayData.MaxCount -= 1;
		}
		return;
	}

//-------------------------添加------------------------------

	APlayerCharacter* Player = Cast<APlayerCharacter>(UClientBPFunctionLibrary::GetPlayerCharacter());
	// 没有玩家的时候，一般为创建角色的时候，直接给类型
	if (!Player)
	{
		Data.LODType = LOD_TYPE::NEAR_DIS;
		Data.Type = PARTICLE_TYPE::TYPE_PARTICLE_SCENE;
		Data.bVisible = true;

		ParticleManager->CurrentParticleDisplayConfig.NEARCount[Data.Type] += 1;
		ParticleManager->CurrentParticleDisplayConfig.MaxCount += 1;
		return;
	}

	float Distance = (Player->CameraComponent->GetComponentLocation() - GetActorLocation()).Size();

	if (Distance < NEARDIS)
	{
		Data.LODType = LOD_TYPE::NEAR_DIS;
	}
	else if (Distance < FARDIS)
	{
		Data.LODType = LOD_TYPE::MIDDEL_DIS;
	}
	else
	{
		Data.LODType = LOD_TYPE::FAR_DIS;
	}
	
	AServerCharacter* FActor = Cast<AServerCharacter>(FirstTarget);
	AServerCharacter* SActor = Cast<AServerCharacter>(SecondTarget);

	// 本地玩家的宠物
	//AActor* PetActor = UClientBPFunctionLibrary::FindEntity(Player->PetEntityId) ? UClientBPFunctionLibrary::FindEntity(Player->PetEntityId)->Actor() : nullptr;

	// 指定的目标是玩家或玩家的宠物
	if (SActor == Player)// || SActor == PetActor)
	{
		Data.Type = PARTICLE_TYPE::TYPE_PARTICLE_PLAYER;
	}

	// 指定的目标是其他玩家或者其他玩家的宠物
	else if (SActor && (SActor->IsA(APlayerCharacter::StaticClass())))// || SActor->IsA(APetCharacter::StaticClass())))
	{
		Data.Type = PARTICLE_TYPE::TYPE_PARTICLE_OTHERPLAYER;
	}

	// 施法的是玩家
	else if (FActor == Player)
	{
		Data.Type = PARTICLE_TYPE::TYPE_PARTICLE_PIN;
	}

	// 施法的是其他玩家
	else if (FActor && FActor->IsA(APlayerCharacter::StaticClass()))
	{
		Data.Type = PARTICLE_TYPE::TYPE_PARTICLE_PIOP;
	}

	// 被施法的是客户端对象
	else if(SActor && SActor->IsA(AServerCharacter::StaticClass()))
	{
		Data.Type = PARTICLE_TYPE::TYPE_PARTICLE_NPC;
	}

	// 其他
	else
	{
		Data.Type = PARTICLE_TYPE::TYPE_PARTICLE_SCENE;
	}
	
	bool bVisible = false;

	// 添加粒子
	switch (Data.LODType)
	{
		case LOD_TYPE::NEAR_DIS:
			if (ParticleManager->ParticleDisplayConfig.NEARCount[Data.Type] >= ParticleManager->CurrentParticleDisplayConfig.NEARCount[Data.Type] + 1)
			{
				ParticleManager->CurrentParticleDisplayConfig.NEARCount[Data.Type] += 1;
				bVisible = true;
			}
			break;
		case LOD_TYPE::MIDDEL_DIS:
			if (ParticleManager->ParticleDisplayConfig.MIDDELCount[Data.Type] >= ParticleManager->CurrentParticleDisplayConfig.MIDDELCount[Data.Type] + 1)
			{
				ParticleManager->CurrentParticleDisplayConfig.MIDDELCount[Data.Type] += 1;
				bVisible = true;
			}
			break;
		case LOD_TYPE::FAR_DIS:
			if (ParticleManager->ParticleDisplayConfig.FARCount[Data.Type] >= ParticleManager->CurrentParticleDisplayConfig.FARCount[Data.Type] + 1)
			{
				ParticleManager->CurrentParticleDisplayConfig.FARCount[Data.Type] += 1;
				bVisible = true;
			}
			break;
		default:
			break;
	}
	
	if (ParticleManager->ParticleDisplayConfig.MaxCount < ParticleManager->CurrentParticleDisplayConfig.MaxCount + 1)
	{
		bVisible = false;
	}
	else
	{
		ParticleManager->CurrentParticleDisplayConfig.MaxCount += 1;
	}
	
	if (!bVisible)
	{
		switch (Data.LODType)
		{
			case LOD_TYPE::NEAR_DIS:
				ParticleManager->CurrentParticleDisplayConfig.NEARCount[Data.Type] -= 1;
				break;
			case LOD_TYPE::MIDDEL_DIS:
				ParticleManager->CurrentParticleDisplayConfig.MIDDELCount[Data.Type] -= 1;
				break;
			case LOD_TYPE::FAR_DIS:
				ParticleManager->CurrentParticleDisplayConfig.FARCount[Data.Type] -= 1;
				break;
		}
	}
		
	Data.bVisible = bVisible;

	//设置可见性
	if (!bVisible)
	{
		GetParticleSystemComponent()->SetVisibility(bVisible);
		if (STComponent)
		{
			STComponent->SetVisibility(bVisible);
		}
		if (SKComponent)
		{
			SKComponent->SetVisibility(bVisible);
		}
	}
}

void AEffectEmitter::SortEffect()
{
	if (UClientGameInstance::Instance->ParticleManager->IsNeedSort(UID))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(UClientBPFunctionLibrary::GetPlayerCharacter());
		if (IsValid(Player))
		{
			UClientGameInstance::Instance->ParticleManager->SortEffect(Player->CameraComponent->GetComponentLocation(),
				Player->CameraComponent->GetComponentRotation());
		}
	}
}

void AEffectEmitter::AddModule(class UParticleModule* ParticleModule, int32 EmitterIndex, int32 LODIndex)
{
	ModuleLODData Data;
	Data.EmitterIndex = EmitterIndex;
	Data.LODIndex = LODIndex;
	ModuleList.Add(ParticleModule, Data);
	UClientGameInstance::Instance->ParticleManager->AddModule(ParticleModule);
}

void AEffectEmitter::RemoveModule()
{
	UParticleLODLevel* CurLevel;
	for (auto Module : ModuleList)
	{
		if (UClientGameInstance::Instance->ParticleManager->RemoveModule(Module.Key))
		{
			CurLevel = GetParticleSystemComponent()->Template->Emitters[Module.Value.EmitterIndex]->LODLevels[Module.Value.LODIndex];
			if (CurLevel->Modules.Num())
			{
				CurLevel->Modules.RemoveAt(CurLevel->Modules.Num() - 1);
				CurLevel->SpawnModules.RemoveAt(CurLevel->SpawnModules.Num() - 1);
				CurLevel->UpdateModules.RemoveAt(CurLevel->UpdateModules.Num() - 1);
			}
		}
	}
}

int32 AEffectEmitter::GetEntityID(AActor* Actor)
{
	AServerCharacter* ServerCharacter = Cast<AServerCharacter>(Actor);
	if (!ServerCharacter)
	{
		//CS3_ERROR(TEXT("-->Null Pointer error:AEffectEmitter::GetEntityID : ServerCharacter!"));
		return 0;
	}

	return ServerCharacter->EntityId;
}

void AEffectEmitter::AddBulletMsgBegin(KBEngine::FVariantArray* args, AActor* Actor)
{
	AServerCharacter* ServerCharacter = Cast<AServerCharacter>(Actor);
	if (!ServerCharacter)
	{
		//CS3_ERROR(TEXT("-->Null Pointer error:AEffectEmitter::AddBulletMsgBegin : ServerCharacter!"));
		return;
	}

	args->Add(INT_TO_FSTRING(BulletBuffOwnerId));
	args->Add(INT_TO_FSTRING(BulletBuffIndex));
	if (BulletBuffProxyId == -1 && UClientBPFunctionLibrary::GetPlayerCharacter() == ServerCharacter)
	{
		args->Add(INT_TO_FSTRING(UClientBPFunctionLibrary::GetPlayerID()));
	}
	else if (BulletBuffProxyId == UClientBPFunctionLibrary::GetPlayerID())
	{
		args->Add(INT_TO_FSTRING(ServerCharacter->EntityId));
	}
	args->Add(ID);
	args->Add(INT_TO_FSTRING(BulletBuffEffectIndex));
	args->Add(INT_TO_FSTRING(BulletBuffEffectCustomIndex));
}

bool AEffectEmitter::IsPlaying()
{ 
	return bPlaying;
}

FString AEffectEmitter::GetEffectID()
{ 
	return ID;
}

float AEffectEmitter::GetInitAlpha()
{ 
	return EffectData->InitAlpha;
}

float AEffectEmitter::GetAlpha()
{ 
	return Alpha; 
}

bool AEffectEmitter::IsGraduallyHidden()
{
	return EffectData->bGraduallyHidden;
}

bool AEffectEmitter::IsUseConstAlpha()
{
	return EffectData->bUseConstAlpha;
}

int32 AEffectEmitter::GetSortIndex()
{
	return EffectData->SortIndex;
}

void AEffectEmitter::GetBaseParticleLocationAndRotation_Implementation(const FVector& Location, const FRotator& Rotation, int32 MoudelIndex, int32 BaseParticleIndex, bool bTrace, const FVector& ImpactPoint, UParticleSystem* Template)
{

}

void AEffectEmitter::ParticleUpdateLoopEnd_Implementation()
{

}

#pragma optimize("", on)
