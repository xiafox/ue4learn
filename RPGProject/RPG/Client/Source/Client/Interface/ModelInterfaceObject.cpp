// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelInterfaceObject.h"
#include "Character/ServerCharacter.h"

#pragma optimize( "", off )


TMap<MODEL_PART_TYPE, FString> UModelInterfaceObject::GetModelPartDatas()
{
	if (ModelExtParts.Num() >= 1)
	{
		return ModelExtParts;
	}
	else
	{
		return ModelParts;
	}
}

bool UModelInterfaceObject::IsChangeBody()
{
	return ModelExtParts.Num() >= 1;
}

void UModelInterfaceObject::OnRep_modelParts(const TMap<MODEL_PART_TYPE, FString>& NewModel, const TMap<MODEL_PART_TYPE, FString>& OldModel)
{
	ModelParts = NewModel;
	AServerCharacter* owner = Cast<AServerCharacter>(GetActor());
	if (owner)
	{
		owner->CreateAppearance(GetModelPartDatas());
	}
	
}

void UModelInterfaceObject::CLIENT_OnSetModelPart(const int32& ModelPart, const FString& ModelID)
{
	ModelParts.Add(MODEL_PART_TYPE(ModelPart), ModelID);
	if (IsChangeBody())return;
	
	AServerCharacter* owner = Cast<AServerCharacter>(GetActor());
	if (owner)
	{
		owner->ChangeModelPart(MODEL_PART_TYPE(ModelPart), ModelID);
	}
}

void UModelInterfaceObject::OnRep_modelExtParts(const TMap<MODEL_PART_TYPE, FString>& NewModel, const TMap<MODEL_PART_TYPE, FString>& OldModel)
{
	ModelExtParts = NewModel;

	AServerCharacter* owner = Cast<AServerCharacter>(GetActor());
	if (owner)
	{
		owner->CreateAppearance(GetModelPartDatas());
	}
}

void UModelInterfaceObject::CLIENT_OnSetModelExtPart(const int32& ModelPart, const FString& ModelID)
{
	ModelExtParts.Add(MODEL_PART_TYPE(ModelPart), ModelID);

	AServerCharacter* owner = Cast<AServerCharacter>(GetActor());
	if (owner)
	{
		owner->ChangeModelPart(MODEL_PART_TYPE(ModelPart), ModelID);
	}
}

void UModelInterfaceObject::OnRep_modelScale(const float& NewScale, const float& OldScale)
{
	AServerCharacter* owner = Cast<AServerCharacter>(GetActor());
	if (owner)
	{
		owner->SetModelScale(NewScale);
	}
}

void UModelInterfaceObject::GetMethodMap(TArray<FString>& Methods)
{
	Methods.Add(TEXT("CLIENT_OnSetModelPart"));
	Methods.Add(TEXT("CLIENT_OnSetModelExtPart"));
}

void UModelInterfaceObject::GetPropertyMap(TArray<FString>& Propertys)
{
	Propertys.Add(TEXT("modelScale"));
}

#pragma optimize( "", on )