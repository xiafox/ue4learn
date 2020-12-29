#pragma once
#include "../BaseLoader.h"
#include "WorldLoader.generated.h"


UCLASS(BlueprintType)
class  UWorldLoader : public UBaseLoader
{
	GENERATED_BODY()
protected:	
	virtual void InitData()override;
};