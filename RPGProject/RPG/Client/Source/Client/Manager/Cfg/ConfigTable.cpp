#include "ConfigTable.h"


bool UConfigTable::CheckRow(const FCLIENT_TABLE_ROW_BASE* Row, const FName& RowName) const
{
	if (Row == nullptr)
	{
		return false;
	}

	//这个状态只在这有修改
	CheckState& NowCheckState = AllRowCheckState.FindOrAdd(RowName);
	switch (NowCheckState)
	{
	case CheckState::NotCheck:
		if (Row->Check())
		{
			NowCheckState = CheckState::Ok;
			return true;
		}
		else
		{
			NowCheckState = CheckState::Error;
			return false;
		}
	case CheckState::Ok: return true;
	case CheckState::Error: return false;
	default: return false;
	}
}

UConfigTable* UConfigTable::CreateCfgTable(UDataTable* Table)
{
	UConfigTable* ConfigTable = NewObject<UConfigTable>();
	ConfigTable->Table = Table;

	TArray<FName> RowNames = ConfigTable->GetRowNames();
	ConfigTable->AllRowCheckState.Reserve(RowNames.Num());
	for (auto& RowName : RowNames)
	{
		ConfigTable->AllRowCheckState.Add(RowName, CheckState::NotCheck);
	};
	return ConfigTable;
}

const UDataTable* UConfigTable::GetBaseTable() const
{
	return Table;
}

TArray<FName> UConfigTable::GetRowNames() const
{
	return Table->GetRowNames();
}