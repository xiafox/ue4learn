// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/Emitter.h"
// 数据类型
#include "KBEDefine.h"
#include "../EmitterData/EffectEmitterData.h"
#include "EffectEmitter.generated.h"


DECLARE_STATS_GROUP(TEXT("AEffectEmitter"), STATGROUP_AEffectEmitter, STATCAT_Advanced);


/*
* 文件名称：EffectEmitter.h
* 功能说明：
* 创建光效：
	统一接口调用粒子管理器的PlayParticle方法，在播放前会预先加载资源，因为是资源加载是异步处理，所以在加载完毕播放前会判断播放的对象是否有效。
	服务器通知客户端销毁Actor也是异步处理，所以在播放的时候会判断对象是否有destory标记
*
* 光效的BeginPlay：
    准备光效的所有资源，包括光效的两个目标，设置网格资源
	根据条件开启光效Tick
*
* BeginOverlap：
	绑定了静态网格(STComponent)的碰撞，这里说明一下，光效的碰撞针对静态网格，动态网格(SKComponent)只做表现。
	检测碰撞，向服务器发送子弹Buff信息，在playing为true时有效
*
* InitEmitterTransform：
	初始化光效的位置，创建路径资源
*
* SetEffectVisible：
	设置光效的可见性，绑定光效根据可见性规则显示，非绑定光效根据bBindingVisibility这个变量来控制
*
* Play：
	播放光效，设置延迟播放
*
* StartPlay：
	真正开始播放，并设置生命周期，开启碰撞，设置粒子为活动，播放子光效，音效和动画，并将状态playing设置为true
*
* Stop：
	粒子播放完毕，等待被销毁，关闭碰撞，粒子设置为未激活状态，播放死亡光效，销毁需要被销毁的子光效，并将状态playing设置为false
*
* Tick：
	更新粒子的数据，透明度，位置，定期调用服务器方法等，只在playing为true时有效
*
* EndPlay：
	数据的清理，资源的销毁等
*
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmitterOnStop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmitterOnArrived);

///模块数据
struct ModuleLODData
{
	int32 EmitterIndex;				//发射器索引
	int32 LODIndex;					//LOD索引
};

/**
 * 粒子光效类基类
 */
UCLASS(abstract)
class  AEffectEmitter : public AEmitter
{
	
	friend class UParticleManager;	///<光效管理类友元

	GENERATED_BODY()

public:
	///构造函数
	AEffectEmitter();


/*-----------------粒子生命周期--------------
*	
*	重写AEmitter类部分方法
*	BeginPlay：
*	DeferSpawn流程时，执行PrepareData
*	Tick：
*	记录粒子运行时间
*	粒子排序
*	持续响应子弹Buff
*	自定义路径移动
*	EndPlay：
*	回收不是被Stop的粒子的数据，执行ClearData
*/
	
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#if WITH_EDITOR
	virtual void Destroyed() override;
#endif



/*-----------------粒子播放流程--------------
*
*	粒子播放的所有流程
*	准备阶段：
*	InitBeforeBeginPlay：准备粒子基本数据
*	开始阶段：
*	PrepareData：粒子开始总入口，使用对象池中的粒子时，准备数据后直接调用PrepareData即可播放光效
*	运行阶段：执行所有业务逻辑
*	Play:播放粒子
*	Stop:结束粒子
*	结束阶段：
*	ClearData:清空粒子数据
*	回收阶段：
*	ClearDataAndAddToBufferPool：加入缓冲区
*/

protected:

	///BeginPlay之前调用
	void InitBeforeBeginPlay(FString InEffectID, int32 UID, AActor* FirstTarget, AActor* SecondTarget, float EffectSendAngle, const FEFFECT_EMITTER_DATA* EffectEmitterData, void* OnEffectCreate);

	///准备粒子数据
	void PrepareData();

	///添加数据到管理器
	void AddDataToManager();

	///增加ParticleModeulColorEX模块
	void AddModule(class UParticleModule* ParticleModule, int32 EmitterIndex, int32 LODIndex);

	///移除ParticleModeulColorEX模块
	void RemoveModule();

	///移除管理器数据
	void RemoveDataFormManager();

	///清除粒子数据
	void ClearData();

	///清除数据，加入粒子缓冲区
	virtual void ClearDataAndAddToBufferPool();

/*-----------------粒子逻辑模块--------------
*
*	粒子所有功能
*/

protected:

	///设置光效和网格体的可见性，并返回绑定的对象
	AActor* SetEffectVisible();

	///更新粒子管理器对应的光效数据，同时更加状态设置行为，只在BeginPlay里面，初始化位置函数后使用
	void UpdateVisibilityData(bool bCreate);

	///添加透明度模块
	virtual void AddAlphaModule(UParticleSystemComponent* ParticleComponent = nullptr);

	///重写光效隐藏效果，在隐藏状态下设置粒子为激活无效，所以每次刷新可见性时会重新激活粒子
	virtual void SetActorHiddenInGame(bool bNewHidden);

	///初始化配置
	virtual void InitFromConfig();

	///初始化路径
	void InitSplineFromConfig();

	///初始化位置信息
	virtual void InitEmitterTransform();

	///初始化路径的位置
	void InitSplineTransform();

	///光效碰撞开始
	UFUNCTION()
	virtual void OnEmitterCollisionBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	///光效碰撞结束
	UFUNCTION()
	virtual void OnEmitterCollisionEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	///不要手动去设置粒子的碰撞，他们有自己的规则
	void SetMeshComponentCollision(bool State);

public:
	/**
	* @fn设置子弹Buff数据
	*
	* @param int32 ProxyID 客户端验证entityID
	* @param int32 OwnerID Buff拥有者entityID
	*
	*/
	void SetBuffBulletData(const int32 BuffIndex, const int32 ProxyID, const int32 OwnerID, const int32 EffectIndex, const int32 EffectCustomIndex);

protected:

	///播放光效，用于内部调用播放子光效，子光效会随主光效一起销毁
	void PlayChildParticle();

	///播放死亡光效，用于内部调用播放死亡光效，死亡光效不会随主光效一起销毁
	void PlayDeathParticle();

	///播放开始的动画
	void PlayAnimation();

	///循环响应碰撞事件
	void CycleResponseCollisionEvent(float DeltaTime);

	///光效透明度更新
	virtual void UpdateTransparency(float DeltaSeconds);

	///更新粒子的排序
	void SortEffect();

	///光效开始播放，不是真正的播放，可能有延迟播放设置
	void Play();

	///播放效果事件回调.
	virtual void OnPlay();

	///光效真正的播放
	virtual void StartPlay();

	//结束播放，结束光效请使用光效管理器的方法，内部使用只限于this指针或SpawnBase类
	void Stop();

	///如果有延时销毁则会调用此方法，处理光效Stop后要做的额外的事情
	virtual void DelayDestroySetting();

	///销毁
	void DestroyEffect();

	///光效抵达
	UFUNCTION()
	virtual void OnEmitterEffectArrived();



/*-----------------调用服务器方法模块--------------
*
*	服务器参数获取
*/

protected:

	///光效抵达时调用的服务器方法
	void RPC_onBulletCollisionArrive();

	///获得entityID
	int32 GetEntityID(AActor* Actor);

	///子弹Buff
	void AddBulletMsgBegin(KBEngine::FVariantArray* args, AActor* Actor);



/*-----------------粒子发射器--------------
*
*	ParticleModuleTransformEX包含：
*	GetBaseParticleLocationAndRotation
*	ParticleUpdateLoopEnd
*/

public:

	///获取每个发射粒子的位置，角度，发射器编号，粒子编号，是否进行射线检测以及检测的位置，需要配合GetParticleTransform模块使用
	UFUNCTION(BlueprintNativeEvent, Category = "Emitter")
	void GetBaseParticleLocationAndRotation(const FVector& Location, const FRotator& Rotation, int32 MoudelIndex, int32 BaseParticleIndex, bool bTrace, const FVector& ImpactPoint, UParticleSystem* Template);

	///粒子发射器一帧更新完毕，需要配合GetParticleTransform模块使用，并把模块放在最后一个发射器上
	UFUNCTION(BlueprintNativeEvent, Category = "Emitter")
	void ParticleUpdateLoopEnd();



/*-----------------粒子属性--------------
*
*/

public:

	///光效结束回调用事件(只是停止，不一定是销毁)
	FEmitterOnStop OnEffectStop;

	///光效抵达事件
	UPROPERTY(BlueprintAssignable)
	FEmitterOnArrived OnEffectArrived;

protected:

	///透明度初始值，用于光效淡入淡出
	float Alpha = 0.0f;

	///已经播放的时间
	float CurrentTime;

	///子光效UID
	TArray<int32> ChildrenEffectUID;

	///自定义路径
	UPROPERTY(BlueprintReadOnly)
	class ABaseSplineAsset* Spline = nullptr;

	///碰撞列表，当前和光效接触的Actor列表
	UPROPERTY()
	TMap<AActor*, float> CollisionList;

	///额外的粒子模块，用于光效的渐变和设置透明度，已AddToRoot
	TMap<UParticleModule*, ModuleLODData> ModuleList;

	///空的根节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	///声音组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAudioComponent* AudioComponent;

	///骨架网格组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SKComponent;

	///静态网格组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* STComponent;

	///目标位置
	UPROPERTY(BlueprintReadOnly)
	FVector TargetPos = FVector::ZeroVector;

	///绑定的开始位置，会一直刷新，Beam光效会用到
	UPROPERTY(BlueprintReadOnly)
	FVector SourcePos = FVector::ZeroVector;

#if WITH_EDITOR
	///区分编辑器光效和游戏世界光效的标志
	bool bIsInEdit = false;

	FName ActorName;

	///延迟创建标记
	bool bDefer = true;
#endif //WITH_EDITOR


/*-----------------粒子管理器传过来的属性--------------
*
*/

protected:

	///目标1。请注意，此属性只在获取管理器数据时初始化，其他地方请不要做修改
	UPROPERTY(BlueprintReadOnly)
	AActor* FirstTarget;

	///目标2。请注意，此属性只在获取管理器数据时初始化，其他地方请不要做修改
	UPROPERTY(BlueprintReadOnly)
	AActor* SecondTarget;

	///光效ID
	UPROPERTY(BlueprintReadOnly)
	FString ID;

	///光效UID
	UPROPERTY(BlueprintReadOnly)
	int32 UID = -1;

	///子弹偏移角度，只是给路径用
	UPROPERTY(BlueprintReadOnly)
	float EffectSendAngle; 

	///粒子创建回调事件
	void* OnEffectCreate = nullptr;

/*-----------------配置表属性--------------
*
*/
	///播放粒子传过来的粒子数据表
	const FEFFECT_EMITTER_DATA* EffectEmitterData;

	///简单粒子数据表
	const FEFFECT_DATA* EffectData;

	///目标1的位置信息
	const FEFFECT_TRANSFORM_DATA* FirstTransform;

	///目标2的位置信息
	const FEFFECT_TRANSFORM_DATA* SecondTransform;

	///在样条曲线上的速度
	float MoveSpeed = 0.f;

	///持续时间
	UPROPERTY(BlueprintReadOnly)
	float LifeTime;	

	//淡入时间
	UPROPERTY(BlueprintReadOnly)
	float FadeInTime;

	//淡出时间
	UPROPERTY(BlueprintReadOnly)
	float FadeOutTime;

	///动画资源
	UPROPERTY(BlueprintReadOnly)
	TArray<UAnimationAsset*> AnimAsset;

	///播放状态
	uint32 bPlaying : 1;

/*-----------------配置表属性--------------
*
*/

private:

	///子弹Buff相关
	int32 BulletBuffIndex;
	int32 BulletBuffProxyId;
	int32 BulletBuffOwnerId;
	int32 BulletBuffEffectIndex;
	int32 BulletBuffEffectCustomIndex;


/*-----------------GetAttributes--------------
*
*/

public:

	UFUNCTION(BlueprintPure)
	bool IsPlaying();

	UFUNCTION(BlueprintPure)
	FString GetEffectID();

	UFUNCTION(BlueprintPure)
	float GetInitAlpha();

	UFUNCTION(BlueprintPure)
	float GetAlpha();

	UFUNCTION(BlueprintPure)
	bool IsGraduallyHidden();

	UFUNCTION(BlueprintPure)
	bool IsUseConstAlpha();

	UFUNCTION(BlueprintPure)
	int32 GetSortIndex();

};
