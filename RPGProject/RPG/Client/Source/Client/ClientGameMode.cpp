#include "ClientGameMode.h"
#include "ClientGameInstance.h"
#include "Interface/Role/TestInterfaceObject.h"
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma optimize( "", off )
AClientGameMode::AClientGameMode()
{
	DefaultPawnClass = nullptr;
	GameSessionClass = nullptr;
	GameStateClass = nullptr;
	PlayerStateClass = nullptr;
	SpectatorClass = nullptr;
}

void AClientGameMode::GM_Login(FString Account, FString PassWord)
{
	TArray<uint8> Datas;
	UClientGameInstance::Instance->KBEComponents->Login(Account, PassWord, Datas);
}

void AClientGameMode::GM_ShowModelConfig()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("左武器 17 : 'SM_Long_003'")), true, true, FLinearColor(0.0, 0.66, 1.0), 120.0);
	UKismetSystemLibrary::PrintString(this, FString(TEXT("身体 4 : 'SK_M_F002'")), true, true, FLinearColor(0.0, 0.66, 1.0), 120.0);
	UKismetSystemLibrary::PrintString(this, FString(TEXT("头发 5 : 'SK_M_H101'")), true, true, FLinearColor(0.0, 0.66, 1.0), 120.0);
	UKismetSystemLibrary::PrintString(this, FString(TEXT("头饰 6 : 'SK_M_T101'")), true, true, FLinearColor(0.0, 0.66, 1.0), 120.0);
	UKismetSystemLibrary::PrintString(this, FString(TEXT("上衣 8 : 'SK_M_C101'")), true, true, FLinearColor(0.0, 0.66, 1.0), 120.0);
	UKismetSystemLibrary::PrintString(this, FString(TEXT("身体 4 : 'test'")), true, true, FLinearColor(0.0, 0.66, 1.0), 120.0);	
}

void AClientGameMode::GM_SetModelPart(MODEL_PART_TYPE ModelPart, FString ModelID)
{
	UTestInterfaceObject* obj = Cast<UTestInterfaceObject>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("TestInterfaceObject")));
	if (obj)
	{
		obj->SetModelPart(ModelPart, ModelID);
	}
}

void AClientGameMode::GM_SetModel()
{
	UTestInterfaceObject* obj = Cast<UTestInterfaceObject>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("TestInterfaceObject")));
	if (obj)
	{
		obj->SetModel();
	}
}

void AClientGameMode::GM_SetModelExt(int32 IsAdd)
{
	UTestInterfaceObject* obj = Cast<UTestInterfaceObject>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("TestInterfaceObject")));
	if (obj)
	{
		obj->SetModelExt(IsAdd);
	}
}

void AClientGameMode::GM_SetIntAttr(FString AttrName, int32 & AttrVal)
{
	UTestInterfaceObject* obj = Cast<UTestInterfaceObject>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("TestInterfaceObject")));
	if (obj)
	{
		KBEngine::FVariantArray arg;
		arg.Add(AttrName);
		arg.Add(AttrVal);
		obj->CellCall(TEXT("CELL_SetIntAttr"), arg);
	}
}

void AClientGameMode::GM_SetStrAttr(FString AttrName, FString & AttrVal)
{
	UTestInterfaceObject* obj = Cast<UTestInterfaceObject>(UClientBPFunctionLibrary::GetInterfaceByName(TEXT("TestInterfaceObject")));
	if (obj)
	{
		KBEngine::FVariantArray arg;
		arg.Add(AttrName);
		arg.Add(AttrVal);
		obj->CellCall(TEXT("CELL_SetStrAttr"), arg);
	}
}

#pragma optimize( "", on )