#pragma once
#include "CoreMinimal.h"
#include "../../Cfg/ClientTableRowBase.h"
#include "EffectEmitterData.generated.h"


// 粒子从属类型
UENUM(BlueprintType)
enum class PARTICLE_TYPE : uint8
{
	///<当前客户端玩家（包括宠物）携带的粒子光效类型
	TYPE_PARTICLE_PLAYER = 1			UMETA(DisplayName = "TYPE_PARTICLE_PLAYER"),
	///<当前客户端其他玩家（包括宠物）携带的粒子光效类型
	TYPE_PARTICLE_OTHERPLAYER = 2		UMETA(DisplayName = "TYPE_PARTICLE_OTHERPLAYER"),
	///<当前客户端玩家正在交互的Entity光效类型（比如和当前玩家正在战斗的Monster所产生的光效）
	TYPE_PARTICLE_PIN = 3				UMETA(DisplayName = "TYPE_PARTICLE_PIN"),
	///<当前客户端其他玩家正在交互的Entity光效类型
	TYPE_PARTICLE_PIOP = 4				UMETA(DisplayName = "TYPE_PARTICLE_PIOP"),
	///<除玩家外的Entity光效类型
	TYPE_PARTICLE_NPC = 5				UMETA(DisplayName = "TYPE_PARTICLE_NPC"),
	///<场景光效类型
	TYPE_PARTICLE_SCENE = 6				UMETA(DisplayName = "TYPE_PARTICLE_SCENE"),
};

// 粒子显示距离等级
UENUM(BlueprintType)
enum class LOD_TYPE : uint8
{
	///<近距离
	NEAR_DIS = 1		UMETA(DisplayName = "NEAR_DIS"),
	///<中等距离
	MIDDEL_DIS = 2		UMETA(DisplayName = "MIDDEL_DIS"),
	///<远距离
	FAR_DIS = 3			UMETA(DisplayName = "FAR_DIS"),
};

USTRUCT(BlueprintType)
struct FEFFECT_TRANSFORM_DATA
{
	GENERATED_USTRUCT_BODY()
public:
	///是否世界位置
	///是，代表“世界的”位置
	///否，代表“相对的”位置
	///注意：
	///①“绑定的、世界的”，代表：位置为目标位置加上配置表的位置，不跟随目标变化
	///②“绑定的、相对的”，代表：位置为目标位置加上配置表的位置，跟随目标变化
	///③“不绑定、世界的”，代表：位置为配置表的位置，不跟随目标变化
	///④“不绑定、相对的”，代表：位置为目标位置加上配置表的位置，不跟随目标变化
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bLocationOnWorld : 1;

	///是否世界旋转
	///是，代表“世界的”旋转
	///否，代表“相对的”旋转
	///注意：
	///①“绑定的、世界的”，代表，旋转为目标的旋转加上配置表的旋转，不跟随目标变化
	///②“绑定的、相对的”，代表，旋转为目标的旋转加上配置表的旋转，跟随目标变化
	///③“不绑定、世界的”，代表，旋转为配置表的旋转，不跟随目标变化
	///④“不绑定、相对的”，代表，旋转为目标的旋转加上配置表的旋转，不跟随目标变化
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bRotationOnWorld : 1;

	///是否世界缩放
	///是，代表“世界的”缩放
	///否，代表“相对的”缩放
	///注意：
	///①“绑定的、世界的”，代表，缩放为配置表的缩放，不跟随目标变化
	///②“绑定的、相对的”，代表，缩放为配置表的缩放乘以模型的缩放，跟随目标变化
	///③“不绑定、世界的”，代表，缩放为配置表的缩放，不跟随目标变化
	///④“不绑定、相对的”，代表，缩放为配置表的缩放，不跟随目标变化
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bScaleOnWorld : 1;

	///位置变换
	///注意：
	///光效创建后的位置修正
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform Transform;

	///光效第一个绑定点
	///指定绑定点/插槽（见具体模型的骨骼）
	///找不到、留空，代表“默认绑定胶囊体中心”，约为root上1米
	///注意：
	///移动光效的起点，简单光效的绑定点
	///光效的第二个绑定点
	///指定绑定点/插槽（见具体模型的骨骼）
	///找不到、留空，代表“默认绑定胶囊体中心”，约为root上1米
	///注意：
	///移动光效的终点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Socket;

	///是否绑定第一个绑定点
	///注意：
	///移动光效此属性无效(除Beam)
	///是否绑定第二个绑定点
	///注意：
	///一般情况下简单光效配置的绑定点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBindSocket;

	bool operator==(const FEFFECT_TRANSFORM_DATA& OtherData)
	{
		if (Transform.GetLocation() == OtherData.Transform.GetLocation() &&
			Transform.GetRotation() == OtherData.Transform.GetRotation() &&
			Transform.GetScale3D() == OtherData.Transform.GetScale3D() &&
			bLocationOnWorld == OtherData.bLocationOnWorld &&
			bRotationOnWorld == OtherData.bRotationOnWorld &&
			bScaleOnWorld == OtherData.bScaleOnWorld &&
			bBindSocket == OtherData.bBindSocket && 
			Socket == OtherData.Socket)
		{
			return true;
		}

		return false;
	}
};



USTRUCT(BlueprintType)
struct FEFFECT_DATA
{
	GENERATED_USTRUCT_BODY()
public:

	///是否交换光效目标
	///如“A向B发射一个火球”变成“B向A发射一个火球”
	///注意：
	///交换光效Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bSwap : 1;

	///粒子是否一直渲染
	///粒子在摄像机外也一直更新模块数据
	///注意：
	///只有在粒子在摄像机外生成，跟随绑定对象移动到摄像机内时不显示，勾选才有意义
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bAlwaysRender : 1;

	///是否对光效进行排序
	///透明物体的排序是根据单个单个网格体离相机的距离进行排序渲染的，没有精确到像素，可能造成渲染乱序
	///注意：
	///开启自定义透明度排序
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bSortEffect : 1;

	///排序优先级
	///优先级大的后渲染，渲染表现会在前面
	///注意：
	///只有当粒子中有开启bSortEffect，这个优先级才会生效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SortIndex = 0;

	///1倍缩放下粒子包围盒的大小
	///根据粒子包围盒和摄像机的位置，确定一个锥体，用于判断粒子是否在排序范围内
	///注意：
	///这个值越精确越好
	///UPROPERTY(EditAnywhere, BlueprintReadWrite)
	///	FVector EffectBounds;

	///延迟播放时间（秒）
	///一段时间后才开始表现
	///注意：
	///创建光效Actor，延迟激活表现
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DelayPlayTime = 0.0f;

	///持续时间（秒）-1代表一直存在，0代表立即销毁，请为每个光效配置生命周期
	///表现持续一段时间
	///注意：
	///激活粒子、模型、路径等表现
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LifeTime = 0.0f;

	///延迟销毁时间（秒，0代表“立刻销毁”）
	///	表现结束后等待一段时间再销毁，不同组件有不同的表现：			//这个地方需要完善
	///		粒子/连线：缓慢消散
	///		贴花/模型：停止运动
	///		飞行/路径：停止移动
	///注意：
	///冻结表现：延迟销毁光效Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DelayDestroyTime = 0.0f;

	///第一个目标点的位置信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEFFECT_TRANSFORM_DATA FirstTransform;

	///第二个目标点的位置信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEFFECT_TRANSFORM_DATA SecondTransform;

	///对于非绑定光效，是否绑定其释放者的可见性（光效随施法者显示或者隐藏）
	///注意：
	///只对非绑定光效起作用
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bBindingVisibility : 1;

	///使用固定透明度
	///注意：
	///与GraduallyHidden渐变效果产生冲突 两者只能勾选一个
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bUseConstAlpha : 1;

	///初始透明度
	///注意：
	///勾选NonGradientHidden此配置有效
	///初始化透明度效果 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseConstAlpha"))
		float InitAlpha = 1.0f;

	///光效在生命周期内淡入淡出
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bGraduallyHidden;

	///淡入时间
	///注意：
	///勾选GraduallyHidden此配置有效
	///-1淡入为0秒
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bGraduallyHidden"))
		float FadeInTime = -1.f;

	///淡出时间
	///注意：
	///勾选GraduallyHidden此配置有效
	///-1淡出为生命周期
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bGraduallyHidden"))
		float FadeOutTime = -1.f;

	///子光效列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> ChildrenEffect;

	///是否连带销毁子光效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyChildren;

	///死亡光效列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> DeathEffect;

	virtual ~FEFFECT_DATA() {}

	///只比对基础属性
	bool operator==(FEFFECT_DATA& OtherData)
	{
		if (FMath::Abs(DelayPlayTime - OtherData.DelayPlayTime) < 0.01f &&
			FMath::Abs(DelayDestroyTime - OtherData.DelayDestroyTime) < 0.01f &&
			SecondTransform == OtherData.SecondTransform &&
			FirstTransform == OtherData.FirstTransform &&
			FMath::Abs(LifeTime - OtherData.LifeTime) < 0.01f &&
			bSwap == OtherData.bSwap)
		{
			return true;
		}
		return false;
	}
};

USTRUCT(BlueprintType)
struct FOTHER_EFFECT_DATA
{
	GENERATED_USTRUCT_BODY()

public:
	///飞行光效：抵达光效
	///飞行光效抵达目标时触发光效，指定飞行时间、指定飞行速度都有效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyEffect")
		TArray<FString> TriggerEffect;

	///飞行光效：是否“指定时间抵达”
	///是，代表，指定时间抵达
	///否，代表，指定速度飞行
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyEffect")
		bool bFlyOfTime = false;

	///飞行光效：指定飞行时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyEffect", meta = (EditCondition = "bFlyOfTime"))
		float FlyTime = 0.0f;

	///飞行光效：指定飞行速度（厘米/秒）
	///注意：
	///移动的光效通用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyEffect", meta = (EditCondition = "!bFlyOfTime"))
		float FlySpeed = 0.f;

	///飞行光效：是否做第二阶段曲线运动
	///注意：
	///飞行光效专用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyEffect")
		bool bCanSpecialMove = false;

	///飞行光效：第二阶段曲线运动最近距离
	///注意：
	///飞行光效专用，值为0不做曲线运动
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyEffect", meta = (EditCondition = "bCanSpecialMove"))
		float TransformationDistance = 0.0f;

	///飞行光效：第二阶段曲线运动上升的高度
	///注意：
	///飞行光效专用，值为0不做曲线运动
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyEffect", meta = (EditCondition = "bCanSpecialMove"))
		float RisingDistance = 0.0f;

	///螺旋线：偏角增量Z
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpiralEffect")
		float Spiral_DeltaAngleZ = 0.0f;

	///螺旋线：仰角增量XY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpiralEffect")
		float Spiral_DeltaAngleXY = 0.0f;

	///获得宝物光效目标头顶飞起得距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialEffect")
		float Special_SourceHeight = 0.f;

	///获得宝物光效降落头顶得距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialEffect")
		float Special_TargetHeight = 0.f;

	///连线光效：光效是否随着目标弯曲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeamEffect")
		bool Beam_IsBendFromTarget;

	///连线光效：扰动强度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeamEffect")
		float Beam_ParticleRange = 1.0f;

	///连线光效：粒子强度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeamEffect")
		float Beam_ParticleSize = 1.0f;

	///连线光效：弯曲强度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeamEffect", meta = (EditCondition = "Beam_IsBendFromTarget"))
		float Beam_BendStrength = 0.0f;

	///拖尾光效：拖尾模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrailEffect")
		TEnumAsByte<enum ETrailWidthMode> Trail_WidthScaleMode = ETrailWidthMode::ETrailWidthMode_FromCentre;
};

UENUM(BlueprintType)
enum class RESPONSE_TYPE : uint8
{
	BEGIN_COLLISION		UMETA(DisplayName = "BEGIN_COLLISION"),
	END_COLLISION		UMETA(DisplayName = "END_COLLISION"),
};

UENUM(BlueprintType)
enum class PARAMS_TYPE : uint8
{
	BULLETBUFF		UMETA(DisplayName = "BULLETBUFF"),
	OtherEntityID	UMETA(DisplayName = "OtherEntityID"),
	SKILLID			UMETA(DisplayName = "SKILLID"),
};

USTRUCT(BlueprintType)
struct  FMETHOD_NAME
{
	GENERATED_USTRUCT_BODY()
public:
	///响应的方法
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MethodName;

	///方法参数列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<PARAMS_TYPE> ParamsList;

	FMETHOD_NAME()
	{
		MethodName = "onBulletCollisionBegin";
		ParamsList.Add(PARAMS_TYPE::BULLETBUFF);
	}
};

//碰撞响应数据
USTRUCT(BlueprintType)
struct  FEFFECT_COLLISION_RESPONSE
{
	GENERATED_USTRUCT_BODY()
public:

	///周期响应事件,光效碰撞进了网格体后未出来是否周期响应碰撞事件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLoopResponse = true;

	///碰撞事件响应间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bLoopResponse"))
		float ResponseCycle = 1.0f;

	///碰撞调用的技能ID，需要配合RESPONSE_TYPE::SKILLID使用
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ResponseSkillID = 1;

	///响应时机
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		RESPONSE_TYPE ResponseType;

	///碰撞的客户端类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> ActorType;

	///碰撞的服务器类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString EntityType;

	///响应的方法列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMETHOD_NAME> MethodList;

	FEFFECT_COLLISION_RESPONSE()
	{
		ResponseType = RESPONSE_TYPE::BEGIN_COLLISION;
		ActorType = AActor::StaticClass();
		MethodList.Add(FMETHOD_NAME());
	}
};

//声音效果
USTRUCT(BlueprintType)
struct  FAUDIO_EFFECT
{
	GENERATED_USTRUCT_BODY()
public:

	///音频资源 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<class USoundBase> AudioAsset;

	///是否循环播放 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLoop;

	///音频描叙 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "说明")
		FString Description;
};

///路径资源
USTRUCT(BlueprintType)
struct  FSPLINE_ASSET
{
	GENERATED_USTRUCT_BODY()
public:
	///路径：路径资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<class ABaseSplineAsset> SplineAsset;

	///路径：移动速度（厘米/秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed = 0.0f;

	///路径：循环次数
	///注意：
	///0代表一直循环永不停止
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LoopNumber = 0;

	///路径：是否“保持本地位移”
	///创建后不再跟随移动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bAbsoluteLocation : 1;

	///路径：是否“保持本地旋转”
	///创建后不再跟随旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bAbsoluteRotation : 1;

	///路径：是否“保持本地缩放”
	///创建后不再跟随缩放
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bAbsoluteScale : 1;
};

USTRUCT(BlueprintType)
struct  FMODEL_ASSET
{
	GENERATED_USTRUCT_BODY()
public:
	///静态模型
	///注意：
	///支持模型碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UStaticMesh> CollisionMesh;

	///静态模型形变信息
	///注意：
	///静态模型的形变设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform CollisionMeshTransform;

	///光效的静态网格物体的可见性设置
	///注意：
	///屏蔽模型的可见性规则
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCollisionMeshVisibility;

	///骨骼模型
	///若不配置动画资源，无表现
	///注意：
	///只支持表现，不支持碰撞功能
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<class USkeletalMesh> AnimMesh;

	///骨骼模型形变信息
	///注意：
	///骨骼模型的形变设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform AnimMeshTransform;

	///骨骼模型：动画资源列表
	///光效播放时，循环播放列表第一个资源
	///延迟销毁时，暂停在当时的表现
	///注意：
	///需要模型与动作的骨骼互相匹配，资源列表其他资源，按特定的脚本指定播放
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSoftObjectPtr<class UAnimationAsset>> AnimAsset;

	///骨骼模型：动画播放速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct  FEFFECT_EMITTER_DATA : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_USTRUCT_BODY()
public:
	///光效类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "EffectInstance")
		TSoftClassPtr<class AEffectEmitter> EffectInstance;

	///粒子资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "ParticleSystem")
		TSoftObjectPtr<UParticleSystem> ParticleSystemPtr;

	///光效数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "EffectData")
		FEFFECT_DATA EffectData;

	///其他光效数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "OtherEffectData")
		FOTHER_EFFECT_DATA OtherEffectData;

	///模型资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "ModelData")
		FMODEL_ASSET ModelData;

	///路径数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "SplineAsset")
		FSPLINE_ASSET SplineAsset;

	///音效数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "AudioData")
		FAUDIO_EFFECT AudioData;

	///碰撞响应
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "CollisionResponse")
		TArray<FEFFECT_COLLISION_RESPONSE> CollisionResponse;

	FEFFECT_EMITTER_DATA()
	{
		CollisionResponse.Add(FEFFECT_COLLISION_RESPONSE());
	}

	///备注说明
	///
	///注意：
	///	策划同人应当备注明确光效配置的语境，以方便后续维护跟进
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "备注")
		FString Remarks;

	bool operator==(FEFFECT_EMITTER_DATA& OtherData)
	{
		if (EffectInstance == OtherData.EffectInstance &&
			ParticleSystemPtr == OtherData.ParticleSystemPtr &&
			EffectData == OtherData.EffectData)
		{
			return true;
		}

		return false;
	}
};

USTRUCT()
struct  FStateParticles
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<int32> EffectUIDList;
};

USTRUCT(BlueprintType)
struct  FEFFECT_PRELOAD_DATA : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_USTRUCT_BODY()
public:
	///粒子预加载列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> PreLoadList;

	///预加载地图的名称为逻辑关卡的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "备注")
	FString Description;
};


