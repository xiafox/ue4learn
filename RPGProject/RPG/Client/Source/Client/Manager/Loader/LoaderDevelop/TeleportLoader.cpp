#include "TeleportLoader.h"
#include "../DetectDevelop/SceneDetect.h"

void UTeleportLoader::InitData()
{
	Super::InitData();
	if (LoadDetector.Num() < 1)
	{
		UBaseDetect* Detector = NewObject<USceneDetect>();
		LoadDetector.Add(Detector);
		LoadProgressList.Add(1.0f);
		LoadParamList.Add(TEXT("Teleport"));
	}
}

