// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/EntityInterfaceObject.h"
#include "StateInterfaceObject.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UStateInterfaceObject : public UEntityInterfaceObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void OnRep_state(const int32& NewState, const int32& OldState);

	UFUNCTION(BlueprintCallable)
		void OnRep_effectState(const int32& NewEffect, const int32& OldEffect);

	UFUNCTION(BlueprintCallable)
		void OnRep_actForbid(const int32& NewActForbid, const int32& OldActForbid);

	virtual void GetMethodMap(TArray<FString>& Methods) override;
	virtual void GetPropertyMap(TArray<FString>& Propertys) override;
};
