#pragma once
#include "../ClientTableRowBase.h"
#include "ClientConfigData.generated.h"


///各配置表的类型
UENUM()
enum class CLIENT_CONFIG_TYPE_ENUM :uint8
{
	///空
	CLIENT_CONFIG_TYPE_NULL = 0	UMETA(DisplayName = "CLIENT_CONFIG_TYPE_NULL"),
	CLIENT_CONFIG_ENTITY_INTERFACE_DEFINE = 1 UMETA(DisplayName = "CLIENT_CONFIG_ENTITY_INTERFACE_DEFINE"),
	CLIENT_CONFIG_ENTITY_OF_ACTOR_CLASS_ASSET = 2 UMETA(DisplayName = "CLIENT_CONFIG_ENTITY_OF_ACTOR_CLASS_ASSET"),///<entity对应Actor的蓝图资源
	CLIENT_CONFIG_MODEL_PART_CONFIG = 3 UMETA(DisplayName = "CLIENT_CONFIG_MODEL_PART_CONFIG"),
	CLIENT_CONFIG_ANIMATION_CONFIG = 4 UMETA(DisplayName = "CLIENT_CONFIG_ANIMATION_CONFIG"),
	CLIENT_CONFIG_STRTOCLASS = 5 UMETA(DisplayName = "CLIENT_CONFIG_STRTOCLASS"),
	CLIENT_CONFIG_TYPE_PARTICLE = 6 UMETA(DisplayName = "CLIENT_CONFIG_TYPE_PARTICLE"),
	CLIENT_CONFIG_EFFECT_PRELOAD_DATA = 7 UMETA(DisplayName = "CLIENT_CONFIG_EFFECT_PRELOAD_DATA"),
};

USTRUCT(BlueprintType)
struct  FCLIENT_CONFIG_DATA : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_BODY()
public:
	///配置提示信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "CsvCongData", DisplayName = "HintDesc")
		FString HintDesc;
	///配置加载路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "CsvCongData", DisplayName = "ConfigPath")
		TSoftObjectPtr<UDataTable> ConfigPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "CsvCongData", DisplayName = "PreLoad")
		bool PreLoading = true;
};
