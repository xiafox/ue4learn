#pragma once
#include "Engine/DataTable.h"
#include "ClientTableRowBase.generated.h"


enum CheckResult
{
	//没有重载
	Unrealized,
	//检查正确
	Ok,
	//检查错误
	Error,
};

struct RowCheckResultInfo
{
	FName RowName;
	CheckResult Result;
	FString CheckLog;
};

///<所有配置表结构体的基类，此类公开了RowName
USTRUCT()
struct FCLIENT_TABLE_ROW_BASE : public FTableRowBase
{
	GENERATED_BODY()
public:
	///配置表默认行名（建议将ID设置为默认值，不需要表头字符）
	FName RowName;

	//检查数据是否正常
	virtual bool Check() const
	{
		return true;
	};
private:
	inline void SetRowName(const FName& NewRowName)
	{
		RowName = NewRowName;
	};

public:
	//不会加载其他资源得浅检查
	virtual void CheckShallow(RowCheckResultInfo& ResultInfo)
	{
		ResultInfo.Result = CheckResult::Unrealized;
	};

	//会加载其他资源得深检查(深检查中要先调用浅检查)
	virtual void CheckDeep(RowCheckResultInfo& ResultInfo)
	{
		CheckShallow(ResultInfo);
	};

	virtual bool GetIsBackstageAsyncLoad()
	{
		return true;
	};
};