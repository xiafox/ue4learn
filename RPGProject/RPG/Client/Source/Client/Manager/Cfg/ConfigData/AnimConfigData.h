#pragma once

#include "../ClientTableRowBase.h"
#include "ClientTypes/ActionDefine.h"
#include "Animation/AnimationAsset.h"
#include "AnimConfigData.generated.h"

USTRUCT(BlueprintType)
struct  FANIMATION_CONFIG_DATA : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_USTRUCT_BODY()
public:
	// 状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EObjectState State;

	// 行为模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionModel ActionModel;

	// 效果状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEffectState EffectState;

	// 动作资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UAnimationAsset> AnimPath;

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