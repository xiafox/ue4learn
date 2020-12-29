#pragma once

#include "../BaseLoader.h"
#include "CreateRoleLoader.generated.h"


UCLASS(BlueprintType)
class  UCreateRoleLoader : public UBaseLoader
{
	GENERATED_BODY()
protected:
	virtual void InitData()override;
};