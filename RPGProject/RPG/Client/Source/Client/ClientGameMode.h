// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClientTypes/ModelDefine.h"
#include "ClientGameMode.generated.h"


UCLASS(minimalapi)
class AClientGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AClientGameMode();

#pragma region GM指令
	UFUNCTION(Exec)
		void GM_Login(FString Account, FString PassWord);

	////////////////////////////////模型测试//////////////////////////////////////////
	UFUNCTION(Exec)
		void GM_ShowModelConfig();

	UFUNCTION(Exec)
		void GM_SetModelPart(MODEL_PART_TYPE ModelPart, FString ModelID);

	UFUNCTION(Exec)
		void GM_SetModel();

	UFUNCTION(Exec)
		void GM_SetModelExt(int32 IsAdd);

	////////////////////////////////模型测试//////////////////////////////////////////
	UFUNCTION(Exec)
		void GM_SetIntAttr(FString AttrName, int32& AttrVal);
	UFUNCTION(Exec)
		void GM_SetStrAttr(FString AttrName, FString& AttrVal);

	

#pragma endregion
};



