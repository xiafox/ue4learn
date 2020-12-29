#pragma once

#include "CoreMinimal.h"
#include "ClientTypes/ModelDefine.h"
#include "MapInfo.generated.h"


 /**
 * 字典转化 服务器类型为：DICT_INT_INT
 */
USTRUCT(BlueprintType)
struct FINT_INT_MAPS
{
	GENERATED_BODY()
public:
	FINT_INT_MAPS(){}
	FINT_INT_MAPS(const FVariant& variant)
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
					int32 key = dataMap[TEXT("key")].GetValue<int32>();
					int32 value = dataMap[TEXT("value")].GetValue<int32>();
					datas.Add(key, value);
				}
			}
		}
	}

	FVariant ToVariant()
	{
		FVariant outVariant;
		KBEngine::FVariantMap outMap;
		KBEngine::FVariantMap VarMap;
		KBEngine::FVariantArray VarArr;

		for (auto it = datas.CreateConstIterator(); it; ++it)
		{

			VarMap.Add(TEXT("key"), FVariant(it.Key()));
			VarMap.Add(TEXT("value"), FVariant(it.Value()));
			VarArr.Add(VarMap);
		}

		outMap.Add(TEXT("dictData"), VarArr);
		outVariant = FVariant(outMap);
		return outVariant;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, int32> datas;
};

/**
* 字典转化 服务器类型为：DICT_STR_INT
*/
USTRUCT(BlueprintType)
struct FSTR_INT_MAPS
{
	GENERATED_BODY()
public:
	FSTR_INT_MAPS() {}
	FSTR_INT_MAPS(const FVariant& variant)
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
					FString key = dataMap[TEXT("key")].GetValue<FString>();
					int32 value = dataMap[TEXT("value")].GetValue<int32>();
					datas.Add(key, value);
				}
			}
		}
	}

	FVariant ToVariant()
	{
		FVariant outVariant;
		KBEngine::FVariantMap outMap;
		KBEngine::FVariantMap VarMap;
		KBEngine::FVariantArray VarArr;

		for (auto it = datas.CreateConstIterator(); it; ++it)
		{

			VarMap.Add(TEXT("key"), FVariant(it.Key()));
			VarMap.Add(TEXT("value"), FVariant(it.Value()));
			VarArr.Add(VarMap);
		}

		outMap.Add(TEXT("dictData"), VarArr);
		outVariant = FVariant(outMap);
		return outVariant;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> datas;
};

/**
* 字典转化 服务器类型为：DICT_STR_STR
*/
USTRUCT(BlueprintType)
struct FSTR_STR_MAPS
{
	GENERATED_BODY()
public:
	FSTR_STR_MAPS() {}
	FSTR_STR_MAPS(const FVariant& variant)
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
					FString key = dataMap[TEXT("key")].GetValue<FString>();
					FString value = dataMap[TEXT("value")].GetValue<FString>();
					datas.Add(key, value);
				}
			}
		}
	}

	FVariant ToVariant()
	{
		FVariant outVariant;
		KBEngine::FVariantMap outMap;
		KBEngine::FVariantMap VarMap;
		KBEngine::FVariantArray VarArr;

		for (auto it = datas.CreateConstIterator(); it; ++it)
		{

			VarMap.Add(TEXT("key"), FVariant(it.Key()));
			VarMap.Add(TEXT("value"), FVariant(it.Value()));
			VarArr.Add(VarMap);
		}

		outMap.Add(TEXT("dictData"), VarArr);
		outVariant = FVariant(outMap);
		return outVariant;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> datas;
};

/**
* 字典转化 服务器类型为：DICT_INT_STR
*/
USTRUCT(BlueprintType)
struct FINT_STR_MAPS
{
	GENERATED_BODY()
public:
	FINT_STR_MAPS() {}
	FINT_STR_MAPS(const FVariant& variant)
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
					int32 key = dataMap[TEXT("key")].GetValue<int32>();
					FString value = dataMap[TEXT("value")].GetValue<FString>();
					datas.Add(key, value);
				}
			}
		}
	}

	FVariant ToVariant()
	{
		FVariant outVariant;
		KBEngine::FVariantMap outMap;
		KBEngine::FVariantMap VarMap;
		KBEngine::FVariantArray VarArr;

		for (auto it = datas.CreateConstIterator(); it; ++it)
		{

			VarMap.Add(TEXT("key"), FVariant(it.Key()));
			VarMap.Add(TEXT("value"), FVariant(it.Value()));
			VarArr.Add(VarMap);
		}

		outMap.Add(TEXT("dictData"), VarArr);
		outVariant = FVariant(outMap);
		return outVariant;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, FString> datas;
};