// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ClientObject.h"
#include "ClientTableRowBase.h"
#include "ConfigTable.h"
#include "ConfigData/ClientConfigData.h"
#include "CfgManager.generated.h"



/**
 * 配置表管理器
 */

UCLASS(BlueprintType)
class UCfgManager : public UClientObject
{
	GENERATED_BODY()

public:
	UCfgManager();
	~UCfgManager();

	static UCfgManager* GetInstance();///<获取CfgManager实例，返回static UCfgManager*  CfgManager实例对象

	/**
	 *获取配置列表数据
	 *
	 *@param TableType 列表类型
	 *
	 *@return  const UConfigTable
	 */
	const UConfigTable* GetTableByType(CLIENT_CONFIG_TYPE_ENUM TableType);

	/*
	*获取配置列表数据
	*
	*@param ConfigPath 配置表路径
	*
	*@return  const UConfigTable* 配置列表数据
	*/
	const UConfigTable* GetTableByPath(TSoftObjectPtr<UDataTable> ConfigPath);

private:
	void LoadMainCfg(FString path, bool preload = true);///<加载配置表
private:
	UPROPERTY()
		UConfigTable* ClientConfigTable;///配置表主数据表
	UPROPERTY()
		TMap<CLIENT_CONFIG_TYPE_ENUM, UConfigTable*> TableMap;///配置表
	UPROPERTY()
		TMap<TSoftObjectPtr<UDataTable>, UConfigTable*> TablePathMap;///配置表路径表
};
