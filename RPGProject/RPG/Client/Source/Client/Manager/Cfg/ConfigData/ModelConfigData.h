#pragma once

#include "../ClientTableRowBase.h"
#include "Engine/StreamableManager.h"
#include "ModelConfigData.generated.h"


USTRUCT(BlueprintType)
struct FMODEL_CONFIG_DATA : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_USTRUCT_BODY()
public:
	///模型部件路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UObject> ModelPartPath;

	/// 相对位置值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Transform")
		FVector RelativeLocation;

	/// 相对旋转值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Transform")
		FRotator RelativeRotation;

	/// 相对缩放值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Transform")
		FVector RelativeScale3D = FVector(1.0f);
};
