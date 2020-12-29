#pragma once

#include "CoreMinimal.h"
#include "Client.h"
#include "RoleInfo.generated.h"

/**
 *服务器entity与客户端entity交互方法对应参数的定义
 */

 /**
 *玩家信息转换类
 */
USTRUCT(BlueprintType)
struct FROLEINFO
{
	GENERATED_BODY()
public:
	FROLEINFO(){}
	FROLEINFO(const FVariant& variant)
	{
		//参数形式{"dbid":...,"playerName":...}
		KBEngine::FVariantMap variantMap = variant.GetValue<KBEngine::FVariantMap>();//将变量variant从FVariant转成了FVariantMap即TMap<FString, FVariant> 
		DBid = UINT64_TO_FSTRING(variantMap[TEXT("dbid")].GetValue<uint64>());
		PlayerName = variantMap[TEXT("playerName")].GetValue<FString>();
		Camp = variantMap[TEXT("camp")].GetValue<int32>();
		Gender = variantMap[TEXT("gender")].GetValue<int32>();
		Profession = variantMap[TEXT("profession")].GetValue<int32>();
	}

	FVariant ToVariant()
	{
		FVariant outVariant = FVariant();
		KBEngine::FVariantMap variantMap = KBEngine::FVariantMap();
		variantMap.Add(TEXT("dbid"), DBid);
		variantMap.Add(TEXT("playerName"), PlayerName);
		variantMap.Add(TEXT("camp"), Camp);
		variantMap.Add(TEXT("gender"), Gender);
		variantMap.Add(TEXT("profession"), Profession);
		outVariant = FVariant(variantMap);
		return outVariant;
	}

	bool operator == (const FROLEINFO &other)
	{
		return this->DBid == other.DBid;
	}

public:
	///<DBID数据库唯一标识符
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DBid;
	///<玩家名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerName;
	///<玩家阵营
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Camp;
	///<玩家性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Gender;
	///<玩家职业
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Profession;
};


/**
*玩家信息转换Tmap类
*/
USTRUCT(BlueprintType)
struct FROLEINFO_T
{
	GENERATED_BODY()
public:
	FROLEINFO_T() {}
	FROLEINFO_T(const FVariant& variant)
	{
		//参数形式{"infos":[{"dbid":...,"playerName":...},...]}
		KBEngine::FVariantMap variantMap = variant.GetValue<KBEngine::FVariantMap>();//将变量variant从FVariant转成了FVariantMap即TMap<FString, FVariant> 
		KBEngine::FVariantArray variantArray_infos = variantMap[TEXT("infos")].GetValue<KBEngine::FVariantArray>();//将变量variantMap从FVariant转成FVariantArray
		for (FVariant& element : variantArray_infos)
		{
			FROLEINFO tempRoleInfo = FROLEINFO(element);
			infos.Add(tempRoleInfo);
		}
	}

	FVariant ToVariant()
	{
		FVariant outVariant = FVariant();
		KBEngine::FVariantMap variantMap = KBEngine::FVariantMap();
		KBEngine::FVariantArray variantArray_infos = KBEngine::FVariantArray();
		for (auto& tempRoleInfo : infos)
		{
			FVariant element = tempRoleInfo.ToVariant();
			variantArray_infos.Add(element);
		}
		variantMap.Add(TEXT("infos"), variantArray_infos);

		outVariant = FVariant(variantMap);
		return outVariant;
	}

public:
	TArray<FROLEINFO> infos;///<玩家转换信息数组
};