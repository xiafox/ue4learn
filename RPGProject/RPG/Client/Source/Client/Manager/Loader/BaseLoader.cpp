#include "BaseLoader.h"
#include "BaseDetect.h"
#include "DetectDevelop/SceneDetect.h"


void UBaseLoader::InitData()
{
	TotalProgress = 0.0f;
	DetectorIndex = -1;
}

void UBaseLoader::StartLoading()
{
	InitData();
	Loading();
}

void UBaseLoader::Loading()
{
	if (LoadProgressList.Num() > 0 && LoadDetector.Num() > 0)
	{
		if (LoadDetector.Num() == (DetectorIndex + 1))
		{
			FinishLoaded();
		}
		else
		{
			DetectorIndex++;
			LoadDetector[DetectorIndex]->OnDetectFinished.BindUObject(this, &UBaseLoader::OnAssetFinishDetect);
			if (LoadParamList.Num() > DetectorIndex)
			{
				LoadDetector[DetectorIndex]->StartDetect(LoadParamList[DetectorIndex]);
			}
			else
			{
				LoadDetector[DetectorIndex]->StartDetect();
			}
		}
	}
	else
	{
		FinishLoaded();
	}
}

void UBaseLoader::CancelLoading()
{
	if (LoadDetector.Num() > 0)
	{
		LoadDetector[DetectorIndex]->CancelDetect();
		LoadDetector[DetectorIndex]->OnDetectFinished.Unbind();
	}
	OnLoadFinished.Unbind();
}

void UBaseLoader::FinishLoaded()
{
	if (LoadDetector.Num() > 0)
	{
		LoadDetector[DetectorIndex]->OnDetectFinished.Unbind();
	}
	TotalProgress = 1.0f;
	///<广播加载完成
	OnLoadFinished.ExecuteIfBound();	
	OnLoadFinished.Unbind();
}

void UBaseLoader::OnAssetFinishDetect()
{
	if (LoadProgressList.Num() > 0 && LoadDetector.Num() > 0 && DetectorIndex > -1)
	{
		TotalProgress += LoadProgressList[DetectorIndex];
		LoadDetector[DetectorIndex]->OnDetectFinished.Unbind();
	}
	Loading();
}

float UBaseLoader::GetCurProgress()
{
	if (DetectorIndex > 0 && LoadProgressList.Num() > DetectorIndex && LoadDetector.Num() > DetectorIndex)
	{
		return TotalProgress + LoadDetector[DetectorIndex]->CProgress * LoadProgressList[DetectorIndex];
	}
	return TotalProgress;
}
