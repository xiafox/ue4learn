#include "CfgManager.h"
#include "Client.h"
#include "ConfigData/ClientConfigData.h"
#include "Engine/StreamableManager.h"

UCfgManager::UCfgManager()
{

}

UCfgManager::~UCfgManager()
{
}

UCfgManager* UCfgManager::GetInstance()
{
	client_checkNoReentry();

	UCfgManager* Instance = NewObject<UCfgManager>();
	if (Instance && Instance->ClientConfigTable == nullptr)
	{
		FString path = TEXT("/Game/Blueprints/Configs/DT_ClientConfigData.DT_ClientConfigData");
#if WITH_EDITOR
		Instance->LoadMainCfg(path, false);
#else
		Instance->LoadMainCfg(path);
#endif
	}
	return Instance;
}

void UCfgManager::LoadMainCfg(FString path, bool preload)
{
	if (!path.Compare(TEXT("")))return;

	UDataTable* baseTable = LoadObject<UDataTable>(nullptr, *path);
	checkf(baseTable != nullptr, TEXT("º”‘ÿ÷˜≈‰÷√±Ì ß∞‹£∫\t%s"), *path);
	if (baseTable)
	{
		UConfigTable* ConfigTable = UConfigTable::CreateCfgTable(baseTable);
		ClientConfigTable = ConfigTable;
	}
	if (preload)
	{
		TArray<FName> RowNames = baseTable->GetRowNames();
		for (FName& rowName : RowNames)
		{
			if (!rowName.ToString().IsEmpty())
			{
				const FCLIENT_CONFIG_DATA* ConfigData = ClientConfigTable->GetRow<FCLIENT_CONFIG_DATA>(rowName.ToString());
				if (ConfigData->PreLoading)
				{
					FStreamableManager StreamManager;
					UObject* TableObj = StreamManager.LoadSynchronous(ConfigData->ConfigPath.ToSoftObjectPath());
					if (TableObj)
					{
						UConfigTable* childTable = UConfigTable::CreateCfgTable((UDataTable*)TableObj);
						int tableID = FCString::Atoi(*rowName.ToString());
						TableMap.Add((CLIENT_CONFIG_TYPE_ENUM)tableID, childTable);
					}
					else
					{
						///<º”‘ÿ ß∞‹
					}
				}
			}
		}
	}
}

const UConfigTable* UCfgManager::GetTableByType(CLIENT_CONFIG_TYPE_ENUM TableType)
{	
	UConfigTable* ConfigTable = nullptr;
	if (TableMap.Contains(TableType))
	{
		ConfigTable = TableMap[TableType];
	}
	else
	{
		const FCLIENT_CONFIG_DATA* ConfigData = ClientConfigTable->GetRow<FCLIENT_CONFIG_DATA>(FString::FromInt((int)TableType));
		if (ConfigData->ConfigPath.ToSoftObjectPath().IsValid())
		{
			FStreamableManager StreamManager;
			UObject* TableObj = StreamManager.LoadSynchronous(ConfigData->ConfigPath.ToSoftObjectPath());
			checkf(TableObj != nullptr, TEXT("º”‘ÿtype≈‰÷√±Ì ß∞‹£∫\t%s"), *ConfigData->ConfigPath.ToSoftObjectPath().ToString());
			ConfigTable = UConfigTable::CreateCfgTable((UDataTable*)TableObj);
			TableMap.Add(TableType, ConfigTable);
		}
	}
	return ConfigTable;
}

const UConfigTable* UCfgManager::GetTableByPath(TSoftObjectPtr<UDataTable> ConfigPath)
{
	UConfigTable* ConfigTable = nullptr;
	if (TablePathMap.Contains(ConfigPath))
	{
		ConfigTable = TablePathMap[ConfigPath];
	}
	else
	{
		if (ConfigPath.ToSoftObjectPath().IsValid())
		{
			FStreamableManager StreamManager;
			UObject* TableObj = StreamManager.LoadSynchronous(ConfigPath.ToSoftObjectPath());
			checkf(TableObj != nullptr, TEXT("º”‘ÿpath≈‰÷√±Ì ß∞‹£∫\t%s"), *ConfigPath.ToSoftObjectPath().ToString());
			ConfigTable = UConfigTable::CreateCfgTable((UDataTable*)TableObj);
			TablePathMap.Add(ConfigPath, ConfigTable);
		}
	}
	return ConfigTable;
}