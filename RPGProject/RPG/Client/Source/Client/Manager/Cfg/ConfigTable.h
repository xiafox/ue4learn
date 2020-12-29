#pragma once
#include "ClientObject.h"
#include "Engine/DataTable.h"
#include "ClientTableRowBase.h"
#include "ConfigTable.generated.h"


UCLASS(BlueprintType)
class UConfigTable :public UClientObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UDataTable* Table;

	enum CheckState
	{
		NotCheck,
		Ok,
		Error,
	};

	//用来记录行是否已经检查过了(防止每次都检查一遍)
	mutable TMap<FName, CheckState> AllRowCheckState;

	//检查行数据
	bool CheckRow(const FCLIENT_TABLE_ROW_BASE* Row, const FName& RowName) const;

public:
	static UConfigTable* CreateCfgTable(UDataTable* Table);

	const UDataTable* GetBaseTable() const;

	TArray<FName> GetRowNames() const;

	template < class T = FCLIENT_TABLE_ROW_BASE>
	const T* GetRow(FString RowName) const
	{
		FName CurRowName = FName(*RowName);
		T* RowTable = (T*)Table->FindRowUnchecked(CurRowName);
		if (CheckRow(RowTable, CurRowName))
		{
			RowTable->RowName = CurRowName;
			return RowTable;
		}
		else
		{
			return nullptr;
		}
	}

	template < class T = FCLIENT_TABLE_ROW_BASE>
	const T* GetRow(FName RowName) const
	{
		T* RowTable = (T*)Table->FindRowUnchecked(RowName);
		if (CheckRow(RowTable, RowName))
		{
			RowTable->RowName = RowName;
			return RowTable;
		}
		else
		{
			return nullptr;
		}
	}

	template <class T>
	void GetAllRows(OUT TArray<T*>& OutRowArray) const
	{
		Table->GetAllRows(TEXT(""), OutRowArray);
	}
};
