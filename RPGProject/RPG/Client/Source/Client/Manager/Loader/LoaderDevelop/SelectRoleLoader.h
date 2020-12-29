#pragma once
#include "../BaseLoader.h"
#include "SelectRoleLoader.generated.h"


UCLASS(BlueprintType)
class  USelectRoleLoader : public UBaseLoader
{
	GENERATED_BODY()
protected:
	virtual void InitData()override;
};