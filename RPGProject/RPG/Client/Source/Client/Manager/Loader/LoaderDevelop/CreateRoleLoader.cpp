#include "CreateRoleLoader.h"
#include "../DetectDevelop/SceneDetect.h"

void UCreateRoleLoader::InitData()
{
	Super::InitData();
	if (LoadDetector.Num() < 1)
	{
		UBaseDetect* Detector = NewObject<USceneDetect>();
		LoadDetector.Add(Detector);
		LoadProgressList.Add(1.0f);
		LoadParamList.Add(TEXT("CreateRole"));
	}
}

