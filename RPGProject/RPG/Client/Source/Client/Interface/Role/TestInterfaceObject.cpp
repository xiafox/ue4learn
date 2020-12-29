// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInterfaceObject.h"

#pragma optimize("", off)

void UTestInterfaceObject::CLIENT_testInt(const int32& arg1, const int32& arg2, const int32& arg3, const FString& arg4)
{

}

void UTestInterfaceObject::CLIENT_testUInt(const int32& arg1, const int32& arg2, const int32& arg3, const FString& arg4)
{

}

void UTestInterfaceObject::CLIENT_testFloat(const float& arg1, const float& arg2, const float& arg3)
{

}

void UTestInterfaceObject::CLIENT_testStr(const FString& arg1, const FString& arg2)
{

}

void UTestInterfaceObject::CLIENT_testListInt(const TArray<int32>& arg1)
{

}

void UTestInterfaceObject::CLIENT_testListUInt(const TArray<int32>& arg1)
{

}

void UTestInterfaceObject::Client_testListStr(const TArray<FString>& arg1)
{

}

void UTestInterfaceObject::Client_testDict(const FROLEINFO_T& arg1)
{

}

void UTestInterfaceObject::SetModelPart(MODEL_PART_TYPE ModelPart, FString ModelID)
{
	KBEngine::FVariantArray arg;
	arg.Add(int32(ModelPart));
	arg.Add(ModelID);
	CellCall(TEXT("CELL_SetModelPart"), arg);
}

void UTestInterfaceObject::SetModel()
{
	KBEngine::FVariantArray arg;
	CellCall(TEXT("CELL_SetModel"), arg);
}

void UTestInterfaceObject::SetModelExt(int32 IsAdd)
{
	KBEngine::FVariantArray arg;
	arg.Add(IsAdd);
	CellCall(TEXT("CELL_SetModelExt"), arg);
}

void UTestInterfaceObject::GetMethodMap(TArray<FString>& Methods)
{
	Super::GetMethodMap(Methods);
	Methods.Add(TEXT("CLIENT_testInt"));
	Methods.Add(TEXT("CLIENT_testUInt"));
	Methods.Add(TEXT("CLIENT_testFloat"));
	Methods.Add(TEXT("CLIENT_testStr"));
	Methods.Add(TEXT("CLIENT_testListInt"));
	Methods.Add(TEXT("CLIENT_testListUInt"));
	Methods.Add(TEXT("Client_testListStr"));
	Methods.Add(TEXT("Client_testDict"));
}

void UTestInterfaceObject::GetPropertyMap(TArray<FString>& Propertys)
{
	Super::GetPropertyMap(Propertys);
	
}

#pragma optimize("", on)