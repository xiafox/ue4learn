// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/EntityInterfaceObject.h"
#include "TestInterfaceObject.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UTestInterfaceObject : public UEntityInterfaceObject
{
	GENERATED_BODY()

	typedef UEntityInterfaceObject Super;

public:
	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 INT8
	*@param arg2 INT16
	*@param arg3 INT32
	*@param arg4 INT64
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void CLIENT_testInt(const int32& arg1, const int32& arg2, const int32& arg3, const FString& arg4);

	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 UINT8
	*@param arg2 UINT16
	*@param arg3 UINT32
	*@param arg4 UINT64
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void CLIENT_testUInt(const int32& arg1, const int32& arg2, const int32& arg3, const FString& arg4);

	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 FLOAT
	*@param arg2 FLOAT
	*@param arg3 FLOAT
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void CLIENT_testFloat(const float& arg1, const float& arg2, const float& arg3);

	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 STRING
	*@param arg2 UNICODE
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void CLIENT_testStr(const FString& arg1, const FString& arg2);

	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 ARRAY_INT32
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void CLIENT_testListInt(const TArray<int32>& arg1);

	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 ARRAY_INT16
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void CLIENT_testListUInt(const TArray<int32>& arg1);

	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 ARRAY_UNICODE
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void Client_testListStr(const TArray<FString>& arg1);

	/**
	*Define method服务器定义以下类型
	*
	*@param arg1 ARRAY_UNICODE
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable)
		void Client_testDict(const FROLEINFO_T& arg1);

	void SetModelPart(MODEL_PART_TYPE ModelPart, FString ModelID);

	void SetModel();

	void SetModelExt(int32 IsAdd);



	virtual void GetMethodMap(TArray<FString>& Methods) override;
	virtual void GetPropertyMap(TArray<FString>& Propertys) override;


	
};
