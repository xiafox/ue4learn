// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipComponent.h"
#include "Character/GameCharacter.h"
#include "ClientTypes/GameConst.h"
#include "Manager/Cfg/ConfigData/ModelConfigData.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEquipComponent::CreateModel(const TMap<MODEL_PART_TYPE, FString>& ModelMap)
{
	AGameCharacter* owner = Cast<AGameCharacter>(GetOwner());
	if (owner == nullptr)return;
	
	TArray<MODEL_PART_TYPE> ModelParts;
	ModelMap.GetKeys(ModelParts);
	RemoveOtherMesh(ModelParts);
	CreateActorMeshComponents(ModelParts);

	TMap<MODEL_PART_TYPE, const FMODEL_CONFIG_DATA*> ModelData;
	GetModelDataFromCfg(ModelMap, ModelData);

	for (auto It = ModelData.CreateConstIterator(); It; ++It)
	{
		TSoftObjectPtr<UObject> objPath = It.Value()->ModelPartPath;
		if (objPath.ToSoftObjectPath().IsValid())
		{
			FStreamableManager StreamManager;
 			UObject* obj_ = StreamManager.LoadSynchronous(objPath.ToSoftObjectPath());
			checkf(obj_ != nullptr, TEXT("Load Mesh(%s) fail!!"), *ModelMap[It.Key()]);

// 			IEquipPartInterface* interface_ = Cast<IEquipPartInterface>(OwnerMeshComponent[It.Key()]);
// 			interface_->SetMesh(obj_);

			if (obj_->GetClass() == USkeletalMesh::StaticClass())
			{
				USkeletalMeshComponent* MeshCom_ = Cast<USkeletalMeshComponent>(OwnerMeshComponent[It.Key()]);
				MeshCom_->SetSkeletalMesh(Cast<USkeletalMesh>(obj_));
			}
			else
			{
				UStaticMeshComponent* MeshCom_ = Cast<UStaticMeshComponent>(OwnerMeshComponent[It.Key()]);
				MeshCom_->SetStaticMesh(Cast<UStaticMesh>(obj_));
			}

			FVector Location_ = It.Value()->RelativeLocation;
			FRotator Rotation_ = It.Value()->RelativeRotation;
			FVector Scale_ = It.Value()->RelativeScale3D;
			if (!Location_.IsZero())
			{
				OwnerMeshComponent[It.Key()]->SetRelativeLocation(Location_);
			}
			if (!Rotation_.IsZero())
			{
				OwnerMeshComponent[It.Key()]->SetRelativeRotation(Rotation_);
			}
			OwnerMeshComponent[It.Key()]->SetRelativeScale3D(Scale_);

			OnSetMeshFinish(It.Key());
// 			
// 			interface_->OnSetMeshFinish(It.Key());
		}
	}
	RefreshMeshSockName();

	owner->CreateApperanceOver();
}

void UEquipComponent::ChangeModelPart(const MODEL_PART_TYPE& ModelPart, const FString& ModelID)
{
	if (!OwnerMeshComponent.Contains(ModelPart))
	{
		TArray<MODEL_PART_TYPE> ModelParts;
		ModelParts.Add(ModelPart);
		CreateActorMeshComponents(ModelParts);
	}
	if (ModelID.IsEmpty())
	{
		if (OwnerMeshComponent[ModelPart]->GetClass() == USkeletalMeshComponent::StaticClass())
		{
			USkeletalMeshComponent* MeshCom_ = Cast<USkeletalMeshComponent>(OwnerMeshComponent[ModelPart]);
			MeshCom_->SetSkeletalMesh(nullptr);
		}
		else
		{
			UStaticMeshComponent* MeshCom_ = Cast<UStaticMeshComponent>(OwnerMeshComponent[ModelPart]);
			MeshCom_->SetStaticMesh(nullptr);
		}
// 		IEquipPartInterface* eInterface = Cast<IEquipPartInterface>(OwnerMeshComponent[ModelPart]);
// 		if (eInterface)
// 		{
// 			eInterface->SetMesh(nullptr);
// 			return;
// 		}
	}
	TMap<MODEL_PART_TYPE, FString> ModelMap;
	ModelMap.Add(ModelPart, ModelID);
	TMap<MODEL_PART_TYPE, const FMODEL_CONFIG_DATA*> ModelData;
	GetModelDataFromCfg(ModelMap, ModelData);

	if (ModelData.Contains(ModelPart))
	{
		TSoftObjectPtr<UObject> objPath = ModelData[ModelPart]->ModelPartPath;
		if (objPath.ToSoftObjectPath().IsValid())
		{
			FStreamableManager StreamManager;
			UObject* obj_ = StreamManager.LoadSynchronous(objPath.ToSoftObjectPath());
			checkf(obj_ != nullptr, TEXT("Load Mesh(%s) fail!!"), *ModelID);

			if (obj_->GetClass() == USkeletalMesh::StaticClass())
			{
				USkeletalMeshComponent* MeshCom_ = Cast<USkeletalMeshComponent>(OwnerMeshComponent[ModelPart]);
				MeshCom_->SetSkeletalMesh(Cast<USkeletalMesh>(obj_));
			}
			else
			{
				UStaticMeshComponent* MeshCom_ = Cast<UStaticMeshComponent>(OwnerMeshComponent[ModelPart]);
				MeshCom_->SetStaticMesh(Cast<UStaticMesh>(obj_));
			}

// 			IEquipPartInterface* interface_ = Cast<IEquipPartInterface>(OwnerMeshComponent[ModelPart]);
// 			interface_->SetMesh(obj_);
// 			interface_->OnSetMeshFinish(ModelPart);
		}
	}
		
}

void UEquipComponent::OnSetMeshFinish(const MODEL_PART_TYPE& ModelPart)
{

}

void UEquipComponent::RemoveOtherMesh(const TArray<MODEL_PART_TYPE>& ModelPartTypes)
{
	for (auto It = OwnerMeshComponent.CreateConstIterator(); It; ++It)
	{
		if (!ModelPartTypes.Contains(It.Key()))
		{
			if (OwnerMeshComponent[It.Key()]->GetClass() == USkeletalMeshComponent::StaticClass())
			{
				USkeletalMeshComponent* MeshCom_ = Cast<USkeletalMeshComponent>(OwnerMeshComponent[It.Key()]);
				MeshCom_->SetSkeletalMesh(nullptr);
			}
			else
			{
				UStaticMeshComponent* MeshCom_ = Cast<UStaticMeshComponent>(OwnerMeshComponent[It.Key()]);
				MeshCom_->SetStaticMesh(nullptr);
			}
// 			IEquipPartInterface* interface_ = Cast<IEquipPartInterface>(It.Value());
// 			if (interface_)
// 			{
// 				interface_->SetMesh(nullptr);
// 			}
		}
	}
}

void UEquipComponent::CreateActorMeshComponents(const TArray<MODEL_PART_TYPE>& ModelPartTypes)
{
	AGameCharacter* character = Cast<AGameCharacter>(GetOwner());
	if (character)
	{
		USkeletalMeshComponent* mesh = character->GetMesh();
		for (auto modelPart : ModelPartTypes)
		{
			if (OwnerMeshComponent.Contains(modelPart))
				continue;
			switch (modelPart)
			{
			case MODEL_PART_TYPE::MODEL_PART_BODY:
				OwnerMeshComponent.Add(modelPart, mesh);
				break;
			case MODEL_PART_TYPE::MODEL_PART_HEAD:
				character->SKMesh_Heads = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Heads"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_HAIR:
				character->SKMesh_Hair = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Hair"));
				OwnerMeshComponent.Add(modelPart, character->SKMesh_Hair);
				break;
			case MODEL_PART_TYPE::MODEL_PART_ADORN:
				character->SKMesh_Adorn = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Adorn"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_HAT:
				character->SKMesh_Hat = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Hat"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_COAT:
				character->SKMesh_Coats = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Coat"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_WRIST:
				character->SKMesh_Wrist = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Wrist"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_HAND:
				character->SKMesh_Gloves = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Gloves"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_WAIST:
				character->SKMesh_Belt = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SKMesh_Belt"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_NECKLACE:
				character->SKMesh_Necklace = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SKMesh_Necklace"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_LRING:
				character->SKMesh_L_RING = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SKMesh_L_RING"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_RRING:
				character->SKMesh_R_RING = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SKMesh_R_RING"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_PANTS:
				character->SKMesh_Pants = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Pants"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_SHOES:
				character->SKMesh_Shoes = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Shoes"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_CLOAK:
				character->SKMesh_Cloak = CreateSKeletalMeshComponent(mesh, modelPart, TEXT("SK_Cloak"));
				break;

			case MODEL_PART_TYPE::MODEL_PART_LWEAPON:
				character->STMesh_LHweapon = CreateStaticMeshComponent(mesh, modelPart, TEXT("LHweapon_Static"));
				break;
			case MODEL_PART_TYPE::MODEL_PART_RWEAPON:
				character->STMesh_RHweapon = CreateStaticMeshComponent(mesh, modelPart, TEXT("RHweapon_Static"));
				break;
			case MODEL_PART_TYPE::MODEL_TYPE_STATIC:
				character->STMesh_DefaultStatic = CreateStaticMeshComponent(mesh, modelPart, TEXT("Default_Static"));
				break;
			}
		}
	}
}

UStaticMeshComponent* UEquipComponent::CreateStaticMeshComponent(USkeletalMeshComponent* ParentMesh, const MODEL_PART_TYPE& ModelType, const FName& Name)
{
	UStaticMeshComponent* stMeshCom = NewObject<UStaticMeshComponent>(GetOwner(), Name);
	stMeshCom->RegisterComponent();
	stMeshCom->AttachToComponent(ParentMesh, FAttachmentTransformRules::KeepRelativeTransform);
	OwnerMeshComponent.Add(ModelType, stMeshCom);
	return stMeshCom;
}

USkeletalMeshComponent* UEquipComponent::CreateSKeletalMeshComponent(USkeletalMeshComponent* ParentMesh, const MODEL_PART_TYPE& ModelType, const FName& Name)
{
	USkeletalMeshComponent* skMeshCom = NewObject<USkeletalMeshComponent>(GetOwner(), Name);
	skMeshCom->RegisterComponent();
	skMeshCom->AttachToComponent(ParentMesh, FAttachmentTransformRules::KeepRelativeTransform);
	OwnerMeshComponent.Add(ModelType, skMeshCom);
	return skMeshCom;
}


void UEquipComponent::GetModelDataFromCfg(const TMap<MODEL_PART_TYPE, FString>& ModelMap, TMap<MODEL_PART_TYPE,  const FMODEL_CONFIG_DATA*>& ModelData)
{
	if (UClientGameInstance::Instance && UClientGameInstance::Instance->CfgManager)
	{
		const UConfigTable* table = UClientGameInstance::Instance->CfgManager->GetTableByType(CLIENT_CONFIG_TYPE_ENUM::CLIENT_CONFIG_MODEL_PART_CONFIG);
		if (table)
		{
			for (auto It = ModelMap.CreateConstIterator(); It; ++It)
			{
				const FMODEL_CONFIG_DATA* data = table->GetRow<FMODEL_CONFIG_DATA>(It.Value());
				if (data)
				{
					ModelData.Add(It.Key(), data);
				}
			}
		}
	}
}

void UEquipComponent::RefreshMeshSockName()
{
	if (OwnerMeshComponent.Contains(MODEL_PART_TYPE::MODEL_PART_BODY))
	{
		UMeshComponent* body = OwnerMeshComponent[MODEL_PART_TYPE::MODEL_PART_BODY];
		for (auto It = OwnerMeshComponent.CreateConstIterator(); It; ++It)
		{
			if (ModelPartType_To_SockName.Contains(It.Key()))
			{
				It.Value()->AttachToComponent(body, FAttachmentTransformRules::KeepRelativeTransform, ModelPartType_To_SockName[It.Key()]);
			}
		}
	}
}

