#pragma once
#include "../BaseLoader.h"
#include "TeleportLoader.generated.h"


UCLASS(BlueprintType)
class  UTeleportLoader : public UBaseLoader
{
	GENERATED_BODY()
protected:
	virtual void InitData()override;
};