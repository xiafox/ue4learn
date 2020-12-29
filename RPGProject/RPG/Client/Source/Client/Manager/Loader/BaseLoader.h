#pragma once
#include "ClientObject.h"
#include "BaseLoader.generated.h"



DECLARE_DELEGATE(FOnLoadFinished);
class UBaseDetect;
/**
*加载基类
*/
UCLASS(BlueprintType)
class UBaseLoader : public UClientObject
{
	GENERATED_BODY()

public:
	virtual void StartLoading();		///<开始加载
	virtual void CancelLoading();				///<取消当前加载
	virtual float GetCurProgress();	   ///<获取当前进度
protected:
	virtual void InitData();	
	virtual void Loading();				///<检测加载进度	
	virtual void FinishLoaded();		///<完成检测
	virtual void OnAssetFinishDetect(); ///<单个资源加载完成	
public:	
	FOnLoadFinished OnLoadFinished;				///<加载回调
protected:
	UPROPERTY()
		TArray<UBaseDetect*> LoadDetector;
	float TotalProgress = 0.0f;						///<加载总进度
	int32  DetectorIndex = -1;						///<
	TArray<float> LoadProgressList;					///<加载进度列表
	TArray<FString>LoadParamList;
};