#pragma once

#include "../BaseLoader.h"
#include "LoginLoader.generated.h"


UCLASS(BlueprintType)
class  ULoginLoader : public UBaseLoader
{
	GENERATED_BODY()
protected:
	virtual void InitData()override;
};
