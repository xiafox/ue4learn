// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/EntityInterfaceObject.h"
#include "ClientTypes/ModelDefine.h"
#include "ModelInterfaceObject.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UModelInterfaceObject : public UEntityInterfaceObject
{
	GENERATED_BODY()

public:
	TMap<MODEL_PART_TYPE, FString> GetModelPartDatas();

	/// 是否在变身状态下
	bool IsChangeBody();

	void OnRep_modelParts(const TMap<MODEL_PART_TYPE, FString>& NewModel, const TMap<MODEL_PART_TYPE, FString>& OldModel);

	/// 设置模型部件
	UFUNCTION(BlueprintCallable)
		void CLIENT_OnSetModelPart(const int32& ModelPart, const FString& ModelID);

	void OnRep_modelExtParts(const TMap<MODEL_PART_TYPE, FString>& NewModel, const TMap<MODEL_PART_TYPE, FString>& OldModel);

	/// 设置变身模型
	UFUNCTION(BlueprintCallable)
		void CLIENT_OnSetModelExtPart(const int32& ModelPart, const FString& ModelID);

	/// 设置模型大小
	UFUNCTION(BlueprintCallable)
		void OnRep_modelScale(const float& NewScale, const float& OldScale);

	virtual void GetMethodMap(TArray<FString>& Methods) override;
	virtual void GetPropertyMap(TArray<FString>& Propertys) override;


private:
	TMap<MODEL_PART_TYPE, FString> ModelParts;
	TMap<MODEL_PART_TYPE, FString> ModelExtParts;
	
};
