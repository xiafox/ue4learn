#pragma once

#include "CoreMinimal.h"
#include "Animation/BlendSpaceBase.h"
#include "ActionDefine.generated.h"



// 对象状态
UENUM(Blueprintable)
enum class EObjectState : uint8
{
	// 无状态
	STATE_NONE = 0,

	// 自由状态
	STATE_FREE,

	// 战斗状态
	STATE_FIGHT,

	// 死亡状态
	STATE_DEAD,
};

// 行为模式
UENUM(BlueprintType)
enum class EActionModel : uint8
{
	// 无
	NONE = 0,

	// 待机
	IDLE,

	// 移动
	MOVE,

	// 下落
	FALLING,
};


// 技能施法阶段
UENUM(Blueprintable)
enum class ESkillActionState : uint8
{
	// 技能未释放
	Skill_NO = 0,

	// 技能攻击阶段
	SKILL_ATTACK,

	// 技能吟唱阶段
	SKILL_CHANT,

	// 技能受击阶段
	SKILL_BEHIT,
};

// 效果状态
UENUM(Blueprintable)
enum class EEffectState : uint8
{
	// 无
	EFFECT_NO = 0,

	// 眩晕
	EFFECT_DIZZY = 1,

	// 定身
	EFFECT_FIX = 2,

	// 睡眠
	EFFECT_SLEEP = 4,
};



USTRUCT(BlueprintType)
struct CLIENT_API FAnimationData
{
	GENERATED_USTRUCT_BODY()
public:
	// 状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EObjectState State;

	// 行为模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionModel ActionModel;

	// 技能状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEffectState EffectState;

	// 动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* AnimSeq;

	// 动画合成
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBlendSpaceBase* AnimBlend;

	// 是否循环播放
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool BloopAnim;

	// 播放速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AnimRate;

	// 开始播放位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartAnimPos;

};