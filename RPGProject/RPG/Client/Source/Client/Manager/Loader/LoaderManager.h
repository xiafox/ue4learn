#pragma once
#include "../BaseManager.h"
#include "ClientTypes/GameFrameworkDefines.h"
#include "LoaderManager.generated.h"


UCLASS(Blueprintable)
class ULoaderManager : public UBaseManager
{
	GENERATED_BODY()

public:
	ULoaderManager();	///<构造函数
	~ULoaderManager();	///<析沟函数

	static ULoaderManager *GetInstance();

	void ChangeToNewLoader(ELoadingType LoadingType);
	bool IsLoading() { return CurrLoader != nullptr; };
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "LoaderManager")
		void OnBPProgressNotify(float Progress, bool bIsShow = true);
private:	
	UFUNCTION()
		void OnLoadFinished();
	void OnProgressNotify();
	void OnStartLoadingSpace(ELoadingType LoadingType);
	void CancelLoading();///<取消当前传送加载主要是加载中掉线处理
private:
	UPROPERTY()
		class UBaseLoader* CurrLoader;		///<当前活动的加载器
	ELoadingType CurrLoaderType = ELoadingType::None;
	FTimerHandle ProgressNotifyTimer;	///<进度通知定时器
};
