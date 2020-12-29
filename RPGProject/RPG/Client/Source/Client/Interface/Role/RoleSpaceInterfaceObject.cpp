// Fill out your copyright notice in the Description page of Project Settings.


#include "RoleSpaceInterfaceObject.h"

#pragma optimize("", off)

void URoleSpaceInterfaceObject::CLIENT_OnEnterSpaceNotify_Implementation(const FString& SpaceScriptID, const INT32 SpaceType)
{

}

void URoleSpaceInterfaceObject::CLIENT_OnLeaveSpaceNotify_Implementation()
{

}

void URoleSpaceInterfaceObject::CELL_GoToSpace(FString SpaceScriptID, FVector Position, FVector Direction)
{
	KBEngine::Entity* entity = GetEntity();
	if (entity == nullptr)
	{
		return;
	}
	KBEngine::FVariantArray args;
	args.Add(SpaceScriptID);
	args.Add(Position);
	args.Add(Direction);
	CellCall(TEXT("CELL_GoToSpace"), args);
}

void URoleSpaceInterfaceObject::GetMethodMap(TArray<FString>& Methods)
{
	Super::GetMethodMap(Methods);
	Methods.Add(TEXT("CLIENT_OnEnterSpaceNotify"));
	Methods.Add(TEXT("CLIENT_OnLeaveSpaceNotify"));
}

void URoleSpaceInterfaceObject::GetPropertyMap(TArray<FString>& Propertys)
{
	Super::GetPropertyMap(Propertys);

}
#pragma optimize("", on)