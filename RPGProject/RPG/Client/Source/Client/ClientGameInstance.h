// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KBEMain.h"
#include "ClientGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UClientGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	virtual void Init() override;
	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;
	virtual void Shutdown() override;


public:
	static UClientGameInstance* Instance;
	static FKBEMain* KBEComponents;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ClientGameInstance")
		class UGameStatusMgr* GameStatusMgr;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ClientGameInstance")
		class ULoginManager* LoginManager;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ClientGameInstance")
		class ULevelManager* LevelManager;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ClientGameInstance")
		class ULoaderManager* LoaderManager;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ClientGameInstance")
		class UCfgManager* CfgManager;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ClientGameInstance")
		class UParticleManager* ParticleManager;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ClientGameInstance")
		class UResourceManager* ResourceManager;
	
};
