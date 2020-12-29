// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
// 数据类型
#include "../BaseManager.h"
#include "EmitterData/EffectEmitterData.h"
#include "ParticleManager.generated.h"



///光效回调
DECLARE_DELEGATE_OneParam(FEffectCreate, int32);
DECLARE_DYNAMIC_DELEGATE_OneParam(FEffectCreateComplete, int32, UID);

// 粒子管理核心数据结构
USTRUCT()
struct  FInternal_EffectData
{
	GENERATED_USTRUCT_BODY()
public:

	FInternal_EffectData() {}

	FInternal_EffectData(FString EffectID)
	{
		bIsValid = true;
		this->EffectID = EffectID;
		this->EffectInstance = nullptr;
		this->Parent = nullptr;
		this->Owner = nullptr;
	}

	FInternal_EffectData(FString EffectID, UClass* EffectInstance, AActor* Parent, class AEffectEmitter* Owner)
	{
		bIsValid = true;
		this->EffectID = EffectID;
		this->EffectInstance = EffectInstance;
		this->Parent = Parent;
		this->Owner = Owner;
	}

	FString EffectID;								//光效ID
	UClass* EffectInstance;							//光效类型
	AActor* Parent;									//光效绑定的对象
	class AEffectEmitter* Owner;					//光效指针
	LOD_TYPE LODType;								//光效初始化时距离玩家的距离
	PARTICLE_TYPE Type;								//光效的类型，区别于类类型
	bool bVisible;									//光效是否可见，基于可见性配置
	bool bIsValid;									//光效有效性标识
};

// 粒子可见性表，根据游戏的画质设置，设置显示粒子的数量
USTRUCT(BlueprintType)
struct FPARTICLE_DIAPLAY_CONFIG : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_USTRUCT_BODY()
public:
	///最大数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxCount;

	///距离0 - 10米粒子的限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<PARTICLE_TYPE, int32> NEARCount;

	///距离10 - 40米粒子的限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<PARTICLE_TYPE, int32> MIDDELCount;

	///距离40 - ?米粒子的限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<PARTICLE_TYPE, int32> FARCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "画质说明")
		FString Description;

	FPARTICLE_DIAPLAY_CONFIG()
	{
		MaxCount = 0;
		//粒子距离表
		NEARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PLAYER, 0);
		NEARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_OTHERPLAYER, 0);
		NEARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PIN, 0);
		NEARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PIOP, 0);
		NEARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_NPC, 0);
		NEARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_SCENE, 0);
																																			
		MIDDELCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PLAYER, 0);
		MIDDELCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_OTHERPLAYER, 0);
		MIDDELCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PIN, 0);
		MIDDELCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PIOP, 0);
		MIDDELCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_NPC, 0);
		MIDDELCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_SCENE, 0);
																																			
		FARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PLAYER, 0);
		FARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_OTHERPLAYER, 0);
		FARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PIN, 0);
		FARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_PIOP, 0);
		FARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_NPC, 0);
		FARCount.Add(PARTICLE_TYPE::TYPE_PARTICLE_SCENE, 0);
	}
};

UCLASS(BlueprintType)
class  UParticleManager : public UBaseManager, public FTickableGameObject
{
	GENERATED_BODY()

public:

	static UParticleManager* GetInstance();

	// 初始化
	void Init();

	/**
	* BlueprintCallable，播放光效 : 蓝图可调用
	*
	* @param  EffectID		光效ID
	* @param  Target1		目标对象1
	* @param  Target2		目标对象2
	* @param  OnComplete	回调函数
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable, Category = "Particle")
	int32 PlayParticle(const FString& EffectID, const float EffectSendAngle, AActor* Target1, AActor* Target2, FEffectCreateComplete OnComplete)
	{
		return PlayParticle(EffectID, EffectSendAngle, Target1, Target2, FEffectCreate::CreateLambda([OnComplete](int32 UID)
		{
			OnComplete.ExecuteIfBound(UID);
		}));
	}

	/**
	* 播放光效
	* @param  EffectID		光效ID
	* @param  Target1		目标对象1
	* @param  Target2		目标对象2
	* @param  OnComplete	回调函数
	*
	*@return 无
	*/
	int32 PlayParticle(const FString& EffectID, const float EffectSendAngle, AActor* Target1, AActor* Target2, FEffectCreate OnComplete = nullptr);

	class AEffectEmitter* PlayParticleOfEdit(const FName& EffectID, AActor* Target);

	const FEFFECT_EMITTER_DATA* GetEffectData(const FString& EffectID);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	void Stop(class AEffectEmitter* Effect);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	void StopEffectByID(FString EffectID);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	void StopEffectByUID(int32 UID);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	void Hide(class AEffectEmitter* Effect);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	void HideByID(FString EffectID);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	void HideByUID(int32 UID);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	void ShowByUID(int32 UID);

	UFUNCTION(BlueprintCallable, Category = "Particle")
	FString FindEffectIDByUID(int32 UID);

	///通过UID查找光效，这边为什么不返回一个光效而是返回一个Actor，很多地方其实只用到了位置属性，
	///没有用到光效本身的属性，避免多余的头文件包含，直接返回一个Actor，需要使用类属性时直接进行类型转换即可。
	UFUNCTION(BlueprintCallable, Category = "Particle")
	AActor* FindEmitterByUID(int32 UID);
	
	UFUNCTION(BlueprintCallable, Category = "Particle")
	FORCEINLINE bool CheckEffectValidByUID(int32 UID)
	{
		return UEffectMap.Contains(UID);
	}

	///如果传入的为空，则显示或隐藏所有非绑定的光效
	UFUNCTION(BlueprintCallable, Category = "Particle")
	void SetVisiableByActor(AActor* owner,bool visible);

	///添加光效到排序列表
	void AddToSortList(int32 UID);

	///从排序列表中移除光效
	void RemoveToSortList(int32 UID);

	///是否是排序的目标
	bool IsNeedSort(int32 UID);

	///排序目标
	void SortEffect(const FVector& CameraLocation, const FRotator CameraRotation);

	///重置排序属性
	void ResetSort();

	///增加模块
	void AddModule(class UParticleModule* ParticleModule);

	///移除模块
	bool RemoveModule(class UParticleModule* ParticleModule);

	// ~Begin FTickableGameObject Interface
	///每一帧重置创建粒子的数量
	virtual void Tick(float DeltaTime) override;

	///获取线程ID
	FORCEINLINE TStatId GetStatId() const { return GetStatID(); }
	// ~End FTickableGameObject Interface

	// 获取世界位置，可以用来绑定位置光效
	UFUNCTION(BlueprintCallable, Category = "Particle")
	AActor* GetWorldSetting();

protected:

	///创建粒子
	void SpawnParticle(TWeakObjectPtr<AActor> DelayTarget1Ptr, TWeakObjectPtr<AActor> DelayTarget2Ptr, FEffectCreate OnComplete,
		FString EffectID, int32 UID, float EffectSendAngle, const FEFFECT_EMITTER_DATA* EffectEmitterData);

public:

	//所有粒子的Map
	UPROPERTY()
	TMap<int32, FInternal_EffectData> UEffectMap;

	//新建的模块对应的Map
	TMap<class UParticleModule*, int32> ModuleMap;

	//粒子UID计数
	int32 EffectUID = 1;

	//当前设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentSetting = -1;

	//粒子显示的规则表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPARTICLE_DIAPLAY_CONFIG ParticleDisplayConfig;

	//当前的粒子个数
	FPARTICLE_DIAPLAY_CONFIG CurrentParticleDisplayConfig;

private:
	//当前需要开启排序的粒子列表，这个表是有序表，从大到小排列
	TArray<int32> SortList;

/*-----------------预加载处理--------------
*
*	在读地图的时候，预加载一部分粒子数据
*/
private:
	// 预加载资源，已AddToRoot
	TArray<UObject*> EmitterPreLoadAssetList;

	// 切换地图时，更新预加载资源，只在OnPreLoadMap函数中调用
	void UpdatePreLoadAsset(const FString& Level);

/*-----------------帧数优化处理--------------
*	
*	粒子有预创建池和缓冲池，预创建池和缓冲池总数量应该小于500
*	当帧数大于40帧时，预创建池会进行工作	创建一个粒子的消耗约为0.4ms
*	40帧：每一帧创建1个对象		25ms
*	50帧：每一帧创建2个对象		20ms
*	
*	缓冲池保存被弃用的无碰撞的SimpleEffect
*
*	当缓冲池占满500时，预创建池会开始缓存FlyEffect，数量为50
*/
private:
	// 每一帧创建的粒子数量计数
	int32 CurrentTickCreateEffect = 0;

	// 粒子缓冲池
	UPROPERTY()
	TArray<class AEffectEmitter*> ParticleBufferPool;

	// 粒子预创建池
	UPROPERTY()
	TArray<class AEffectEmitter*> ParticleDeferSpawnPool;

	// 缓存标志
	enum class ParticleCacheType : uint8
	{
		Default = 0,
		SimpleEffect = 1,
		FlyEffect = 2,
	};
	// 缓存标志
	ParticleCacheType CahceType = ParticleCacheType::SimpleEffect;

	// 获取缓存标志
	ParticleCacheType GetType = ParticleCacheType::SimpleEffect;

	// 简单光效和飞行光效的蓝图类
	UPROPERTY()
	UClass* BP_SimpleEffect = nullptr;
	UPROPERTY()
	UClass* BP_FlyEffect = nullptr;

	// 预创建粒子
	void DeferSpawnEmitter(float DeltaTime);

	// 地图结束时预加载粒子移除root
	void OnEndPlayMap();

	// 地图切换时预加载粒子移除root
	void OnPreLoadMap(const FString& Level);

public:
	// 添加粒子进入缓冲池
	FORCEINLINE void AddToParticleBufferPool(class AEffectEmitter* Emitter)
	{
		ParticleBufferPool.Add(Emitter);
	}

	FORCEINLINE bool IsCacheClass(UClass* Class)
	{
		return (Class == BP_SimpleEffect) && ParticleBufferPool.Num() < 1000;
	}

#if WITH_EDITOR	
	// 防止编辑器手动删除粒子
	FORCEINLINE void RemoveFromParticleDeferSpawnPool(class AEffectEmitter* Emitter)
	{
		ParticleDeferSpawnPool.Remove(Emitter);
	}

	FORCEINLINE bool CheckOnBufferPool(class AEffectEmitter* Emitter)
	{
		return ParticleBufferPool.Contains(Emitter);
	}
#endif
};
















