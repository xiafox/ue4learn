#pragma once

#include "../BaseLoader.h"
#include "BackToSelectRoleLoader.generated.h"


UCLASS(BlueprintType)
class  UBackToSelectRoleLoader : public UBaseLoader
{
	GENERATED_BODY()
protected:
	virtual void InitData()override;
};



