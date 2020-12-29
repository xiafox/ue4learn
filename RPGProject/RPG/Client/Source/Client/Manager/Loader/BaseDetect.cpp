#include "BaseDetect.h"
#include "ClientGameInstance.h"


void UBaseDetect::InitData(const FString& Params)
{
	CProgress = 0.0f;
	TimeoutTime = 0.0f;
}

void UBaseDetect::StartDetect(const FString& Params/* = TEXT("")*/)
{
	InitData(Params);
	Detect();
	UClientGameInstance::Instance->GetWorld()->GetTimerManager().SetTimer(DetectTimer, this, &UBaseDetect::Detect, 0.1f, true);
	if (TimeoutTime > 0.0f)
	{
		UClientGameInstance::Instance->GetWorld()->GetTimerManager().SetTimer(TimeoutTimer, this, &UBaseDetect::OnTimeoutDetect, TimeoutTime, false);
	}
}

void UBaseDetect::Detect()
{
	CProgress += 0.1f;
	if (CProgress >= 1.0f)
	{
		FinishDetect();
	}
}

void UBaseDetect::FinishDetect()
{
	CProgress = 1.0f;	

	if (DetectTimer.IsValid())
	{
		UClientGameInstance::Instance->GetWorld()->GetTimerManager().ClearTimer(DetectTimer);
	}
	if (TimeoutTimer.IsValid())
	{
		UClientGameInstance::Instance->GetWorld()->GetTimerManager().ClearTimer(TimeoutTimer);
	}
	///<通知检测完成
	OnDetectFinished.ExecuteIfBound();
}

void UBaseDetect::OnTimeoutDetect()
{
	FinishDetect();
}

void UBaseDetect::CancelDetect()
{

}

