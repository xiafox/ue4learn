// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatInterfaceObject.h"

#pragma optimize("",off)

void UCombatInterfaceObject::OnRep_HP(const int32& NewHP, const int32& OldHP)
{
}

void UCombatInterfaceObject::OnRep_MP(const int32& NewMP, const int32& OldMP)
{
}

void UCombatInterfaceObject::GetMethodMap(TArray<FString>& Methods)
{
}

void UCombatInterfaceObject::GetPropertyMap(TArray<FString>& Propertys)
{
	Propertys.Add(TEXT("HP"));
	Propertys.Add(TEXT("MP"));
}

#pragma optimize("",on)