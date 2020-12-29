// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleManager.h"
// UE4类型
#include "Particles/ParticleModule.h"
#include "GameDelegates.h"
#include "EmitterData/EffectEmitterData.h"
// CS3类型



///<一帧创建的最大粒子数量
#define MaxCreateCount 100
///<简单粒子最大缓存数量
#define MaxSimpleCacheCount 500	
///<飞行粒子最大缓存数量
#define MaxFlyCacheCount 50

#pragma optimize("", off)

static int32 GEnableParticleSystem = 1;
static FAutoConsoleVariableRef ParticleSystemSwitch(
	TEXT("GEnableParticleSystem"),
	GEnableParticleSystem,
	TEXT("if true enable particle system.\n"),
	ECVF_Default
);

UParticleManager* UParticleManager::GetInstance()
{
	client_checkNoReentry();
	//预加载数据

	UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_TYPE_PARTICLE);
	//UParticleManager* Instacne = (UParticleManager*)StaticConstructObject_Internal(UClientBPFunctionLibrary::GetUClassByStr(TEXT("BP_ParticleManager")));
	UParticleManager* Instance = NewObject<UParticleManager>();
	Instance->Init();
	return Instance;
}

void UParticleManager::Init()
{
	BP_SimpleEffect = UClientBPFunctionLibrary::GetUClassByStr(TEXT("BP_SimpleEffect"));
	BP_FlyEffect = UClientBPFunctionLibrary::GetUClassByStr(TEXT("BP_FlyEffect"));
	// 绑定地图结束和切换地图事件
	FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &UParticleManager::OnEndPlayMap);
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UParticleManager::OnPreLoadMap);
}

void UParticleManager::Tick(float DeltaTime)
{
	// 有默认对象的存在，判断下是不是游戏实例中的管理器对象
	if (this != UClientGameInstance::Instance->ParticleManager)
	{
		return;
	}
	// 清空上一帧创建计数
	CurrentTickCreateEffect = 0;

	// 延迟创建
	DeferSpawnEmitter(DeltaTime);
}

void UParticleManager::DeferSpawnEmitter(float DeltaTime)
{
	// 默认返回
	if (CahceType == ParticleCacheType::Default)
	{
		return;
	}
	// 帧数不够返回
	int32 Count = 0;
	if (DeltaTime < 0.02f)
	{
		Count = 2;
	}
	else if (DeltaTime < 0.025f)
	{
		Count = 1;
	}
	else
	{
		return;
	}

	FTransform Transform = FTransform();
	AEffectEmitter* Emitter = nullptr;
	UClass* CurrentClass = nullptr;

	// 缓存光效对象
	if (CahceType == ParticleCacheType::SimpleEffect)
	{
		CurrentClass = BP_SimpleEffect;
	}
	else
	{
		CurrentClass = BP_FlyEffect;
	}

	for (size_t i = 0; i < Count; i++)
	{
		Emitter = Cast<AEffectEmitter>(UClientBPFunctionLibrary::BeginDeferredActorSpawnFromClass(CurrentClass, Transform));
		if (Emitter)
		{
			ParticleDeferSpawnPool.Add(Emitter);
			Emitter->AddToRoot();
			Emitter = nullptr;
		}
	}

	int32 BufferPoolSize = ParticleBufferPool.Num();
	int32 DeferPoolSize = ParticleDeferSpawnPool.Num();

	// 缓存已满就不缓存了
	if (CahceType == ParticleCacheType::SimpleEffect && DeferPoolSize + BufferPoolSize >= MaxSimpleCacheCount)
	{
		CahceType = ParticleCacheType::Default;
	}
	else if (CahceType == ParticleCacheType::FlyEffect && DeferPoolSize >= MaxFlyCacheCount)
	{
		CahceType = ParticleCacheType::Default;
	}
}

void UParticleManager::OnEndPlayMap()
{
	// 每次切换地图时清理数据
	for (auto Emitter : ParticleDeferSpawnPool)
	{
		Emitter->RemoveFromRoot();
	}
	ParticleDeferSpawnPool.Empty();
	ParticleBufferPool.Empty();

	for (auto Asset : EmitterPreLoadAssetList)
	{
		Asset->RemoveFromRoot();
	}
	EmitterPreLoadAssetList.Empty();
}

void UParticleManager::OnPreLoadMap(const FString& Level)
{
	OnEndPlayMap();
	// 重置状态，又要开始预加载了
	CahceType = ParticleCacheType::SimpleEffect;
	// 预加载粒子资源
	UpdatePreLoadAsset(Level);
}

void UParticleManager::UpdatePreLoadAsset(const FString& Level)
{
	// 获取逻辑关卡的名字
	FString LevelName = UClientBPFunctionLibrary::SplitString(Level, false, "/").Pop();
	if (LevelName.IsEmpty())
	{
		return;
	}

	// 读取本地图预加载资源
	if (UClientGameInstance::Instance && UClientGameInstance::Instance->CfgManager)
	{
		auto EffectTable = UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_EFFECT_PRELOAD_DATA);
		const FEFFECT_PRELOAD_DATA* PreloadData = EffectTable->GetRow<FEFFECT_PRELOAD_DATA>(LevelName);
		if (!PreloadData)
		{
			return;
		}

		// 预加载资源
		for (auto EffectID: PreloadData->PreLoadList)
		{
			const FEFFECT_EMITTER_DATA* EffectEmitterData = GetEffectData(EffectID);
			if (!EffectEmitterData)
			{
				continue;
			}

			TArray<TSoftObjectPtr<UObject>> AssetArr;
			AssetArr.Add(EffectEmitterData->ParticleSystemPtr);
			AssetArr.Add(EffectEmitterData->ModelData.AnimMesh);
			for (auto Asset : EffectEmitterData->ModelData.AnimAsset)
			{
				AssetArr.Add(Asset);
			}
			AssetArr.Add(EffectEmitterData->ModelData.CollisionMesh);
			AssetArr.Add(EffectEmitterData->AudioData.AudioAsset);

			//添加要加载的资源
			TArray<FStringAssetReference> AssetReferenceArr;
			for (TSoftObjectPtr<UObject> Ass : AssetArr)
			{
				if (Ass.ToSoftObjectPath().IsValid())
				{
					AssetReferenceArr.Add(Ass.ToSoftObjectPath());
				}
			}

			//加载资源
			UClientGameInstance::Instance->ResourceManager->RequestAsyncLoad(this, AssetReferenceArr,
				FStreamableDelegate::CreateLambda([this, EffectEmitterData]()
			{
				// 添加到根集
				if (EffectEmitterData->ParticleSystemPtr.Get())
				{
					EffectEmitterData->ParticleSystemPtr.Get()->AddToRoot();
					this->EmitterPreLoadAssetList.Add(EffectEmitterData->ParticleSystemPtr.Get());
				}

				if (EffectEmitterData->ModelData.AnimMesh.Get())
				{
					EffectEmitterData->ModelData.AnimMesh.Get()->AddToRoot();
					this->EmitterPreLoadAssetList.Add(EffectEmitterData->ModelData.AnimMesh.Get());
				}

				if (EffectEmitterData->AudioData.AudioAsset.Get())
				{
					EffectEmitterData->AudioData.AudioAsset.Get()->AddToRoot();
					this->EmitterPreLoadAssetList.Add(EffectEmitterData->AudioData.AudioAsset.Get());
				}

				if (EffectEmitterData->ModelData.CollisionMesh.Get())
				{
					EffectEmitterData->ModelData.CollisionMesh.Get()->AddToRoot();
					this->EmitterPreLoadAssetList.Add(EffectEmitterData->ModelData.CollisionMesh.Get());
				}
				
				for (auto Asset : EffectEmitterData->ModelData.AnimAsset)
				{
					if (Asset.Get())
					{
						Asset.Get()->AddToRoot();
						this->EmitterPreLoadAssetList.Add(Asset.Get());
					}
				}
			}), RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_EFFECT);
		}
	}
}

int32 UParticleManager::PlayParticle(const FString& EffectID, const float EffectSendAngle, AActor* Target1, AActor* Target2, FEffectCreate OnComplete/* = nullptr*/)
{
	if (!GEnableParticleSystem)
	{
		//CS3_DEBUG(TEXT("The Particle System is down"));
		return EffectUID++;
	}

	const FEFFECT_EMITTER_DATA* EffectEmitterData = GetEffectData(EffectID);

	if (EffectEmitterData)
	{
		//获取光效数据成功，在光效管理器中创建此粒子的唯一标记
		UEffectMap.Add(EffectUID, FInternal_EffectData(EffectID));
		//CS3_DEBUG(TEXT("Particle ID named %d have been added"), EffectUID);

		//获取要加载的资源路径
		TArray<TSoftObjectPtr<UObject>> AssetArr;
		AssetArr.Add(EffectEmitterData->ParticleSystemPtr);
		AssetArr.Add(EffectEmitterData->ModelData.AnimMesh);
		for (auto Asset : EffectEmitterData->ModelData.AnimAsset)
		{
			AssetArr.Add(Asset);
		}
		AssetArr.Add(EffectEmitterData->ModelData.CollisionMesh);
		AssetArr.Add(EffectEmitterData->AudioData.AudioAsset);

		//添加要加载的资源
		TArray<FStringAssetReference> AssetReferenceArr;
		for (TSoftObjectPtr<UObject> Ass : AssetArr)
		{
			if (Ass.ToSoftObjectPath().IsValid())
			{
				AssetReferenceArr.Add(Ass.ToSoftObjectPath());
			}
		}

		AssetReferenceArr.Add(EffectEmitterData->SplineAsset.SplineAsset.ToSoftObjectPath());
		AssetReferenceArr.Add(EffectEmitterData->EffectInstance.ToSoftObjectPath());
		//加载资源
		TWeakObjectPtr<AActor> DelayTarget1Ptr(Target1);
		TWeakObjectPtr<AActor> DelayTarget2Ptr(Target2);
		UClientGameInstance::Instance->ResourceManager->RequestAsyncLoad(this, AssetReferenceArr,
			FStreamableDelegate::CreateLambda([this, EffectSendAngle, EffectEmitterData, DelayTarget1Ptr, DelayTarget2Ptr, OnComplete, EffectID, UID = EffectUID]()
		{
			this->SpawnParticle(DelayTarget1Ptr, DelayTarget2Ptr, OnComplete, EffectID, UID, EffectSendAngle, EffectEmitterData);

		}), RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_EFFECT);

		return EffectUID++;
	}
	else
	{
		//CS3_DEBUG(TEXT("EffectID : the effect %s none config data in EffectConfig"), *EffectID);
	}
	return EffectUID++;
}

void UParticleManager::SpawnParticle(TWeakObjectPtr<AActor> DelayTarget1Ptr, TWeakObjectPtr<AActor> DelayTarget2Ptr, FEffectCreate OnComplete,
	FString EffectID, int32 UID, float EffectSendAngle, const FEFFECT_EMITTER_DATA* EffectEmitterData)
{
	if (CurrentTickCreateEffect >= MaxCreateCount)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this, EffectSendAngle, EffectEmitterData, DelayTarget1Ptr, DelayTarget2Ptr, OnComplete, EffectID, UID]
		{
			this->SpawnParticle(DelayTarget1Ptr, DelayTarget2Ptr, OnComplete, EffectID, UID, EffectSendAngle, EffectEmitterData);
		}));

		//CS3_DEBUG(TEXT("The number of spwan-particle more than MaxCreateCount, Create it next frame"));
		return;
	}
	
	// 判断下对象是否有效或者等待被销毁
	if (!DelayTarget1Ptr.IsValid() || !DelayTarget2Ptr.IsValid())
	{
		//CS3_DEBUG(TEXT("The particle release target is invalid, EffectID:"), *EffectID);
		UEffectMap.Remove(UID);
		return;
	}

	// 判断光效是否已经被销毁
	if (!UEffectMap[UID].bIsValid)
	{
		//CS3_DEBUG(TEXT("The particle has been destroyed before it is created, EffectID:"), *EffectID);
		UEffectMap.Remove(UID);
		return;
	}

	// 每一帧创建粒子计数
	CurrentTickCreateEffect++;

	AEffectEmitter* Effect = nullptr;
	
	// 延迟创建标志
	bool bDefer = true;

	// 当一帧创建的粒子大于1，考虑使用缓冲对象，先使用预缓冲池对象，再使用缓冲池对象
	if (CurrentTickCreateEffect > 1 || ParticleBufferPool.Num() > 0)
	{
		// 当取缓存标志为Simple的时候，预缓冲池肯定是有对象的
		if (EffectEmitterData->EffectInstance.Get() == BP_SimpleEffect)
		{
			if (GetType == ParticleCacheType::SimpleEffect)
			{
				if (ParticleDeferSpawnPool.Num() > 0)
				{
					Effect = ParticleDeferSpawnPool.Pop();
					Effect->RemoveFromRoot();
				}
				else if(ParticleBufferPool.Num() > 0)
				{
					bDefer = false;
					Effect = ParticleBufferPool.Pop();
				}
				// 设置缓存状态和取缓存状态
				if (ParticleDeferSpawnPool.Num() + ParticleBufferPool.Num() < MaxSimpleCacheCount)
				{
					CahceType = ParticleCacheType::SimpleEffect;
				}
				else if (ParticleDeferSpawnPool.Num() == 0)
				{
					CahceType = GetType = ParticleCacheType::FlyEffect;
				}
			}
			else
			{
				if (ParticleBufferPool.Num() > 0)
				{
					bDefer = false;
					Effect = ParticleBufferPool.Pop();
				}
			}
		}
		else if(GetType == ParticleCacheType::FlyEffect && EffectEmitterData->EffectInstance.Get() == BP_FlyEffect)
		{
			if (ParticleDeferSpawnPool.Num() > 0)
			{
				Effect = ParticleDeferSpawnPool.Pop();
				Effect->RemoveFromRoot();
				// 重置缓存状态
				CahceType = ParticleCacheType::FlyEffect;
			}
		}
	}

	FTransform Transform = FTransform();

	// DeferredSpawn
	if (!Effect)
	{
		if (!EffectEmitterData->EffectInstance.IsValid())
		{
			//CS3_ERROR(TEXT("EffectInstance is none, EffectID:"), *EffectID);
			return;
		}
		Effect = Cast<AEffectEmitter>(UClientBPFunctionLibrary::BeginDeferredActorSpawnFromClass(EffectEmitterData->EffectInstance.Get(), Transform));
	}
	
	AActor* Target1Ptr = DelayTarget1Ptr.Get();
	AActor* Target2Ptr = DelayTarget2Ptr.Get();

	// 延迟创建初始化
	if (bDefer)
	{
		// 在BeginPlay之前初始化一些必要的数据
		Effect->InitBeforeBeginPlay(EffectID, UID, Target1Ptr, Target2Ptr, EffectSendAngle, EffectEmitterData, (void*)&OnComplete);
		UGameplayStatics::FinishSpawningActor(Effect, Transform);
	}
	// 缓存创建，调用正常流程方法
	else
	{
		Effect->GetParticleSystemComponent()->SetComponentTickEnabled(true);
		Effect->InitBeforeBeginPlay(EffectID, UID, Target1Ptr, Target2Ptr, EffectSendAngle, EffectEmitterData, (void*)&OnComplete);
		Effect->PrepareData();
	}
}

AEffectEmitter* UParticleManager::PlayParticleOfEdit(const FName& EffectID, AActor* Target)
{
	if (!Target)
	{
		return nullptr;
	}

	const FEFFECT_EMITTER_DATA* EffectEmitterData = GetEffectData(FNAME_TO_FSTRING(EffectID));

	if (EffectEmitterData)
	{
		//获取要加载的资源路径
		TArray<TSoftObjectPtr<UObject>> AssetArr;
		AssetArr.Add(EffectEmitterData->ParticleSystemPtr);
		if (EffectEmitterData->ModelData.AnimAsset.Num())
		{
			AssetArr.Add(EffectEmitterData->ModelData.AnimAsset[0]);
		}
		AssetArr.Add(EffectEmitterData->ModelData.AnimMesh);
		AssetArr.Add(EffectEmitterData->ModelData.CollisionMesh);
		
		TArray<FStringAssetReference> AssetReferenceArr;

		//添加要加载的资源
		for (TSoftObjectPtr<UObject> Ass : AssetArr)
		{
			if (Ass.ToSoftObjectPath().IsValid() && nullptr == Ass.Get())
			{
				AssetReferenceArr.Add(Ass.ToSoftObjectPath());
			}
		}
		AssetReferenceArr.Add(EffectEmitterData->EffectInstance.ToSoftObjectPath());

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.bAllowDuringConstructionScript = true;
		AEffectEmitter* Effect = Cast<AEffectEmitter>(Target->GetWorld()->SpawnActor(EffectEmitterData->EffectInstance.Get(), nullptr, nullptr, SpawnParameters));

		if (AssetReferenceArr.Num())
		{
			//加载资源
			FStreamableManager StreamManager;
			for (auto Asset : AssetReferenceArr)
			{
				StreamManager.LoadSynchronous(Asset);
			}
		}

		//加载完毕后设置到组件
		if (Effect)
		{
			//设置及初始化
			Effect->InitBeforeBeginPlay(FNAME_TO_FSTRING(EffectID), 0, Target, Target, 0, EffectEmitterData, nullptr);
			Effect->InitFromConfig();
			Effect->InitEmitterTransform();
#if WITH_EDITOR
			Effect->bIsInEdit = true;
#endif // WITH_EDITOR
			Effect->Play();
			return Effect;
		}
	}
	else
	{
		//CS3_DEBUG(TEXT("EffectID : the effect is none config data in EffectConfig"));
	}

	return nullptr;
}

const FEFFECT_EMITTER_DATA* UParticleManager::GetEffectData(const FString& EffectID)
{
	// 获取配置数据
	const FEFFECT_EMITTER_DATA* EffectData = nullptr;
	if (UClientGameInstance::Instance)
	{
		auto EffectTable = UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_TYPE_PARTICLE);
		EffectData = EffectTable->GetRow<FEFFECT_EMITTER_DATA>(EffectID);
	}
#if WITH_EDITOR
	else
	{
		// 编辑器粒子
		FStreamableManager StreamManager;
		auto EffectTable = (UDataTable*)StreamManager.LoadSynchronous(FSoftObjectPath(*FString("DataTable'/Game/Miscs/ContentConfig/Effect/DT_EffectCfg.DT_EffectCfg'")));

		if (EffectTable)
		{
			FName ID = FSTRING_TO_FNAME(EffectID);
			for (auto Row : EffectTable->GetRowMap())
			{
				if (ID == Row.Key)
				{
					EffectData = (const FEFFECT_EMITTER_DATA*)Row.Value;
				}
			}
		}
	}
#endif //  WITH_EDITOR

	return EffectData;

}

void UParticleManager::Stop(AEffectEmitter * Effect)
{
	Effect->Stop();
}

void UParticleManager::StopEffectByID(FString EffectID)
{
	for (TMap<int32, FInternal_EffectData>::TIterator TBegin = UEffectMap.CreateIterator(); TBegin; ++TBegin)
	{
		if (TBegin.Value().EffectID == EffectID)
		{
			if (IsValid(TBegin.Value().Owner))
			{
				TBegin.Value().Owner->Stop();
			}
		}
	}
}

void UParticleManager::Hide(AEffectEmitter* Effect)
{
	Effect->SetActorHiddenInGame(true);
}

void UParticleManager::HideByID(FString EffectID)
{
	for (TMap<int32, FInternal_EffectData>::TIterator TBegin = UEffectMap.CreateIterator(); TBegin; ++TBegin)
	{
		if (TBegin.Value().EffectID == EffectID)
		{
			if (IsValid(TBegin.Value().Owner))
			{
				TBegin.Value().Owner->SetActorHiddenInGame(true);
			}
		}
	}
}

void UParticleManager::HideByUID(int32 UID)
{
	FInternal_EffectData* EffectData = UEffectMap.Find(UID);

	if (EffectData != nullptr)
	{
		if (IsValid(EffectData->Owner))
		{
			EffectData->Owner->SetActorHiddenInGame(true);
		}
	}
}


void UParticleManager::ShowByUID(int32 UID)
{
	FInternal_EffectData* EffectData = UEffectMap.Find(UID);

	if (EffectData != nullptr)
	{
		if (IsValid(EffectData->Owner))
		{
			EffectData->Owner->SetActorHiddenInGame(false);
		}
	}
}

void UParticleManager::SetVisiableByActor(AActor* Parent, bool visible)
{
	for (TMap<int32, FInternal_EffectData>::TIterator TBegin = UEffectMap.CreateIterator(); TBegin; ++TBegin)
	{
		if (TBegin.Value().Parent == Parent && IsValid(TBegin.Value().Parent))
		{
			if (IsValid(TBegin.Value().Owner) && TBegin.Value().Owner->EffectData->bBindingVisibility)
			{
				TBegin.Value().Owner->SetActorHiddenInGame(!visible);
			}
		}
	}
}

void UParticleManager::StopEffectByUID(int32 UID)
{
	//我们只管停播，移除列表的事情交由光效自己处理
	FInternal_EffectData* EffectData = UEffectMap.Find(UID);

	if (EffectData != nullptr)
	{
		if (IsValid(EffectData->Owner))
		{
			EffectData->Owner->Stop();
		}
		else
		{
			EffectData->bIsValid = false;
		}
	}
}

FString UParticleManager::FindEffectIDByUID(int32 UID)
{
	FInternal_EffectData* EffectData = UEffectMap.Find(UID);

	if (UID > 0)
	{
		ensureMsgf(EffectData != nullptr, TEXT("The particle named %d was no found"), UID);
	}

	if (EffectData != nullptr)
	{
		return EffectData->EffectID;
	}
	return "";
}

AActor* UParticleManager::FindEmitterByUID(int32 UID)
{
	FInternal_EffectData* EffectData = UEffectMap.Find(UID);

	if (EffectData != nullptr)
	{
		if (IsValid(EffectData->Owner))
		{
			return EffectData->Owner;
		}
	}

	return nullptr;
}



void UParticleManager::AddToSortList(int32 UID)
{
	if (!UEffectMap.Contains(UID))
	{
		return;
	}

	SortList.Add(UID);
	SortList.Sort([this](const int32 UID1,const int32 UID2)
	{
		return UEffectMap[UID1].Owner->GetSortIndex() > UEffectMap[UID2].Owner->GetSortIndex();
	});
}

void UParticleManager::RemoveToSortList(int32 UID)
{
	SortList.Remove(UID);
	if (!SortList.Num())
	{
		ResetSort();
	}
}

bool UParticleManager::IsNeedSort(int32 UID)
{
	if (SortList.Num())
	{
		return SortList[0] == UID;
	}

	return false;
}

void UParticleManager::SortEffect(const FVector& CameraLocation, const FRotator CameraRotation)
{
	if (!SortList.Num())
	{
		return;
	}

	//摄像机前面的粒子
	TArray<AEffectEmitter*> FNormalEffect;
	//摄像机后面的粒子
	TArray<AEffectEmitter*> BNormalEffect;
	//排序的粒子
	TArray<AEffectEmitter*> SortEffect;
	//位置
	FVector Location;
	UParticleSystemComponent* ParticleSystemComponent;

	for (auto it = UEffectMap.CreateIterator(); it; ++it)
	{
		// 可能粒子还在加载
		if (!it.Value().Owner)
		{
			continue;
		}

		ParticleSystemComponent = it.Value().Owner->GetParticleSystemComponent();
		Location = FTransform(CameraRotation.Quaternion(), CameraLocation, FVector(1.f)).InverseTransformPosition(ParticleSystemComponent->GetComponentLocation());

		if (it.Value().Owner->GetSortIndex() > 0)
		{
			// 判断盒子在相机前
			if (Location.X > 0)
			{
				SortEffect.Add(it.Value().Owner);
			}
			// 相机在粒子的包围盒内
			else
			{
				Location = ParticleSystemComponent->GetComponentTransform().InverseTransformPosition(CameraLocation);
				if (FMath::Abs(Location.X) < ParticleSystemComponent->Bounds.BoxExtent.X &&
					FMath::Abs(Location.Y) < ParticleSystemComponent->Bounds.BoxExtent.Y &&
					FMath::Abs(Location.Z) < ParticleSystemComponent->Bounds.BoxExtent.Z)
				{
					SortEffect.Add(it.Value().Owner);
				}
			}
		}
		else
		{
			if (Location.X > 0)
			{
				FNormalEffect.Add(it.Value().Owner);
			}
			else
			{
				BNormalEffect.Add(it.Value().Owner);
			}
		}
	}
	// 按自定义的属性排序
	SortEffect.Sort([](AEffectEmitter& Effect1, AEffectEmitter& Effect2)
	{
		return Effect1.GetSortIndex() > Effect2.GetSortIndex();
	});
	// 按距离排序
	FNormalEffect.Sort([CameraLocation](AEffectEmitter& Effect1, AEffectEmitter& Effect2)
	{
		return (Effect1.GetParticleSystemComponent()->GetComponentLocation() - CameraLocation).Size() >
			(Effect2.GetParticleSystemComponent()->GetComponentLocation() - CameraLocation).Size();
	});
	// 按距离排序
	BNormalEffect.Sort([CameraLocation](AEffectEmitter& Effect1, AEffectEmitter& Effect2)
	{
		return (Effect1.GetParticleSystemComponent()->GetComponentLocation() - CameraLocation).Size() <
			(Effect2.GetParticleSystemComponent()->GetComponentLocation() - CameraLocation).Size();
	});

	// 摄像机后面的排序
	int32 Total = BNormalEffect.Num() + FNormalEffect.Num() + SortEffect.Num();
	for (int32 i = 0; i < BNormalEffect.Num(); i++)
	{
		BNormalEffect[i]->GetParticleSystemComponent()->SetTranslucentSortPriority(Total);
	}

	float Distance = (UEffectMap[SortList[0]].Owner->GetParticleSystemComponent()->GetComponentLocation() - CameraLocation).Size();

	// 摄像机前面的和需要范围排序的排序
	int32 Count = 0;
	for (int32 i = 0; i < FNormalEffect.Num(); i++)
	{
		if ((FNormalEffect[i]->GetParticleSystemComponent()->GetComponentLocation() - CameraLocation ).Size()< Distance)
		{
			FNormalEffect[i]->GetParticleSystemComponent()->SetTranslucentSortPriority(Total);
		}
		// 中断排序，因为距离已经到要遮蔽的粒子了
		else
		{
			Count = i;
			break;
		}
	}

	// 需要排序的粒子开始排序
	for (int32 i = 0; i < SortEffect.Num(); i++)
	{
		SortEffect[i]->GetParticleSystemComponent()->SetTranslucentSortPriority(FNormalEffect.Num() + SortEffect.Num() - Count - i);
	}

	// 需要排序的粒子开始排序
	for (int32 i = Count; i < FNormalEffect.Num(); i++)
	{
		FNormalEffect[i]->GetParticleSystemComponent()->SetTranslucentSortPriority(0);
	}


	///获取粒子视锥体的大小

	/*********************************************/
	//		 /-------------------/
	//		/					/|	粒子包围盒
	//     /				   / |
	//    /					  /  |
	//	 |--------------------|	 |
	//	 |		粒子		  |  /
	//	 |					  | /
	//	 |--------------------|/
	//   
	//									·相机点
	//
	//
	/*********************************************/
}

void UParticleManager::ResetSort()
{
	for (auto it = UEffectMap.CreateIterator(); it; ++it)
	{
		if (it.Value().Owner)
		{
			it.Value().Owner->GetParticleSystemComponent()->SetTranslucentSortPriority(0);
		}
	}
}

void UParticleManager::AddModule(UParticleModule* ParticleModule)
{
	if (ModuleMap.Contains(ParticleModule))
	{
		ModuleMap[ParticleModule] += 1;
	}
	else
	{
		ModuleMap.Add(ParticleModule, 1);
		ParticleModule->AddToRoot();
	}
}

bool UParticleManager::RemoveModule(UParticleModule* ParticleModule)
{
	if(ModuleMap.Contains(ParticleModule))
	{
		ModuleMap[ParticleModule] -= 1;
		if (ModuleMap[ParticleModule] == 0)
		{
			ModuleMap.Remove(ParticleModule);
			ParticleModule->RemoveFromRoot();
			return true;
		}
	}

	return false;
}

AActor* UParticleManager::GetWorldSetting()
{
	if (GetWorld())
	{
		GetWorld()->GetWorldSettings();
	}

	return nullptr;
}
#pragma optimize("", on)
