#pragma once

#include "CoreMinimal.h"
#include "KBEDefine.h"
#include "ClientTypes/ModelDefine.h"
#include "ModelInfo.generated.h"


/**
 *服务器entity与客户端entity交互方法对应参数的定义
 */

 /**
 * 字典转化
 */
USTRUCT(BlueprintType)
struct FMODEL_PART_MAPS
{
	GENERATED_BODY()
public:
	FMODEL_PART_MAPS(){}
	FMODEL_PART_MAPS(const FVariant& variant)
	{
		datas.Empty();
		KBEngine::FVariantMap variantMap = variant.GetValue<KBEngine::FVariantMap>();
		if (variantMap.Contains(TEXT("dictData")))
		{
			KBEngine::FVariantArray variantArray = variantMap[TEXT("dictData")].GetValue<KBEngine::FVariantArray>();
			for (auto data : variantArray)
			{
				KBEngine::FVariantMap dataMap = data.GetValue<KBEngine::FVariantMap>();
				if (dataMap.Contains(TEXT("key")) && dataMap.Contains(TEXT("value")))
				{
					int32 modelPart = dataMap[TEXT("key")].GetValue<int32>();
					FString modelID = dataMap[TEXT("value")].GetValue<FString>();
					datas.Add(MODEL_PART_TYPE(modelPart), modelID);
				}
			}
		}
	}

	void FromVariantMap(const KBEngine::FVariantMap& variantMap)
	{
		datas.Empty();
		if (variantMap.Contains(TEXT("dictData")))
		{
			KBEngine::FVariantArray variantArray = variantMap[TEXT("dictData")].GetValue<KBEngine::FVariantArray>();
			for (auto data : variantArray)
			{
				KBEngine::FVariantMap dataMap = data.GetValue<KBEngine::FVariantMap>();
				if (dataMap.Contains(TEXT("key")) && dataMap.Contains(TEXT("value")))
				{
					int32 modelPart = dataMap[TEXT("key")].GetValue<int32>();
					FString modelID = dataMap[TEXT("value")].GetValue<FString>();
					datas.Add(MODEL_PART_TYPE(modelPart), modelID);
				}
			}
		}
		
	}

	FVariant ToVariant()
	{

	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<MODEL_PART_TYPE, FString> datas;
};
