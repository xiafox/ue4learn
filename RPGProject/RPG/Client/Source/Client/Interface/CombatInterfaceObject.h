// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/EntityInterfaceObject.h"
#include "CombatInterfaceObject.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UCombatInterfaceObject : public UEntityInterfaceObject
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
		void OnRep_HP(const int32& NewHP, const int32& OldHP);

	UFUNCTION(BlueprintCallable)
		void OnRep_MP(const int32& NewMP, const int32& OldMP);

	virtual void GetMethodMap(TArray<FString>& Methods) override;
	virtual void GetPropertyMap(TArray<FString>& Propertys) override;
};
