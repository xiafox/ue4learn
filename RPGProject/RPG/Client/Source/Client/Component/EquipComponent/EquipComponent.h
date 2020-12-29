// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClientTypes/ModelDefine.h"
#include "Components/MeshComponent.h"
#include "EquipComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLIENT_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();

	///  创建模型
	void CreateModel(const TMap<MODEL_PART_TYPE, FString>& ModelMap);

	/// 改变模型部件
	void ChangeModelPart(const MODEL_PART_TYPE& ModelPart, const FString& ModelID);

	void OnSetMeshFinish(const MODEL_PART_TYPE& ModelPart);

private:
	/// 移除没有设置的模型
	void RemoveOtherMesh(const TArray<MODEL_PART_TYPE>& ModelPartTypes);

	/// 创建mesh组件
	void CreateActorMeshComponents(const TArray<MODEL_PART_TYPE>& ModelPartTypes);

	FORCEINLINE UStaticMeshComponent* CreateStaticMeshComponent(USkeletalMeshComponent* ParentMesh, const MODEL_PART_TYPE& ModelType, const FName& Name);
	FORCEINLINE USkeletalMeshComponent* CreateSKeletalMeshComponent(USkeletalMeshComponent* ParentMesh, const MODEL_PART_TYPE& ModelType, const FName& Name);

	void GetModelDataFromCfg(const TMap<MODEL_PART_TYPE, FString>& ModelMap, TMap<MODEL_PART_TYPE, const struct FMODEL_CONFIG_DATA*>& ModelData);

	/// 刷新所有的mesh插槽点。因为骨骼没添加上，添加到对应的插槽点会失败
	void RefreshMeshSockName();		

	

private:
	TMap<MODEL_PART_TYPE, UMeshComponent*> OwnerMeshComponent;
};

