#pragma once

#include "ClientObject.h"
#include "BaseDetect.generated.h"

DECLARE_DELEGATE(FOnDetectFinished);


/**
*加载基类
*/
UCLASS(BlueprintType)
class  UBaseDetect : public UClientObject
{
	GENERATED_BODY()
public:
	virtual void InitData(const FString& Params);
	virtual void StartDetect(const FString& Params = TEXT(""));
	virtual void Detect();	///<检测加载进度
	virtual void FinishDetect();	///<完成加载
	virtual void OnTimeoutDetect();///<超时检测
	virtual void CancelDetect();  ///<取消加载
public:
	FOnDetectFinished OnDetectFinished;
	FTimerHandle TimeoutTimer;					///<超时Timer
	FTimerHandle DetectTimer;					///<检测Timer
	float CProgress = 0.0f;
	float TimeoutTime = 0.0f;					///<超时时间
};



