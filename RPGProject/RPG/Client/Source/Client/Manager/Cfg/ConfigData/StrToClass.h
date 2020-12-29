#pragma once

#include "../ClientTableRowBase.h"
#include "Engine/StreamableManager.h"
#include "StrToClass.generated.h"


USTRUCT(BlueprintType)
struct FSTR_TO_CLASS_DATA : public FCLIENT_TABLE_ROW_BASE
{
	GENERATED_USTRUCT_BODY()
public:
	//编号对应蓝图类路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<UObject> ClassPath;

	//后台异步加载(永不释放)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool BackstageAsyncLoad = true;

	UClass* GetTargetClass()
	{
		UClass* targetClass = ClassPath.Get();
		if (targetClass == nullptr )
		{
			FStreamableManager StreamManager;
			return StreamManager.LoadSynchronous(ClassPath);
		}
		return targetClass;
	}

	bool GetIsBackstageAsyncLoad() override
	{
		return BackstageAsyncLoad;
	};
};
