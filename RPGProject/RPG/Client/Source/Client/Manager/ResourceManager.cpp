// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceManager.h"
#include "Misc/Paths.h"
#include "Character/ServerCharacter.h"
#include "../ClientTypes/GameConst.h"

UResourceManager::UResourceManager()
{
	ResourceLoadPriority.Add(RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_PLAYER, { 39, 29, 19 });
	ResourceLoadPriority.Add(RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_OTHER_ENTITY, { 37, 27, 17 });

	ResourceLoadPriority.Add(RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_MEDIA, { 1 });
	ResourceLoadPriority.Add(RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_SPACE_EFFECT, { 3 });
	ResourceLoadPriority.Add(RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_EFFECT, { 2 });
	ResourceLoadPriority.Add(RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_STORY, { 1 });
	ResourceLoadPriority.Add(RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_DEFAULT, { 0 });
}

UResourceManager* UResourceManager::GetInstance()
{
	client_checkNoReentry();
	return NewObject<UResourceManager>();
}

TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const TArray<FStringAssetReference>& TargetsToStream, FStreamableDelegate DelegateToCall, RESOURCE_LOAD_PRIORITY_TYPE type, FVector Position/* = FVector::ZeroVector*/, bool bManageActiveHandle/* = false*/)
{
	TAsyncLoadPriority Priority = GetAsyncLoadPriority(type, Position);
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetsToStream, DelegateToCall, Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}
TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const FStringAssetReference& TargetToStream, FStreamableDelegate DelegateToCall, RESOURCE_LOAD_PRIORITY_TYPE type, FVector Position/* = FVector::ZeroVector*/, bool bManageActiveHandle/* = false*/)
{
	TAsyncLoadPriority Priority = GetAsyncLoadPriority(type, Position);
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetToStream, DelegateToCall, Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}

TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const TArray<FStringAssetReference>& TargetsToStream, TFunction<void()>&& Callback, RESOURCE_LOAD_PRIORITY_TYPE type, FVector Position/* = FVector::ZeroVector*/, bool bManageActiveHandle/* = false*/)
{
	TAsyncLoadPriority Priority = GetAsyncLoadPriority(type, Position);
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetsToStream, std::move(Callback), Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}
TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const FStringAssetReference& TargetToStream, TFunction<void()>&& Callback, RESOURCE_LOAD_PRIORITY_TYPE type, FVector Position/* = FVector::ZeroVector*/, bool bManageActiveHandle/* = false*/)
{
	TAsyncLoadPriority Priority = GetAsyncLoadPriority(type, Position);
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetToStream, std::move(Callback), Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}

TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const TArray<FStringAssetReference>& TargetsToStream, FStreamableDelegate DelegateToCall, TAsyncLoadPriority Priority, bool bManageActiveHandle/* = false*/)
{
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetsToStream, DelegateToCall, Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}
TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const FStringAssetReference& TargetToStream, FStreamableDelegate DelegateToCall, TAsyncLoadPriority Priority, bool bManageActiveHandle/* = false*/)
{
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetToStream, DelegateToCall, Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}

TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const TArray<FStringAssetReference>& TargetsToStream, TFunction<void()>&& Callback, TAsyncLoadPriority Priority, bool bManageActiveHandle/* = false*/)
{
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetsToStream, std::move(Callback), Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}
TSharedPtr<FStreamableHandle> UResourceManager::RequestAsyncLoad(const void * UserObject, const FStringAssetReference& TargetToStream, TFunction<void()>&& Callback, TAsyncLoadPriority Priority, bool bManageActiveHandle/* = false*/)
{
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetToStream, std::move(Callback), Priority, bManageActiveHandle);
	AddResourceHandles(UserObject, StreamableHandle);
	return StreamableHandle;
}

void UResourceManager::RequestAsyncLoad(const UObject* UserObject, const TArray<TSoftObjectPtr<UObject>>& TargetsToStream, FResourceLoaderDelegate DelegateToCall, RESOURCE_LOAD_PRIORITY_TYPE type /*= RESOURCE_LOAD_PRIORITY_TYPE::RESOURCE_LOAD_PRIORITY_TYPE_DEFAULT*/, FVector Position /*= FVector::ZeroVector*/, bool bManageActiveHandle /*= false*/)
{
	TArray<FStringAssetReference> Targets;
	for (auto It = TargetsToStream.CreateConstIterator();It;It++)
	{
		Targets.AddUnique(TargetsToStream[It.GetIndex()].ToSoftObjectPath());
	}
	RequestAsyncLoad(UserObject, Targets, FStreamableDelegate::CreateLambda([DelegateToCall, TargetsToStream]() {
		TArray<UObject *> LoadedResourceArray;
		for (auto It = TargetsToStream.CreateConstIterator(); It; It++)
		{
			UObject* LoadedResource = TargetsToStream[It.GetIndex()].Get();
			if (IsValid(LoadedResource))
			{
				LoadedResourceArray.AddUnique(LoadedResource);
			}
		}
		DelegateToCall.ExecuteIfBound(LoadedResourceArray);
	}), type, Position, bManageActiveHandle);
}

void UResourceManager::BackstageAsyncLoad_ObjectPath(const FSoftObjectPath& TargetToStream)
{
	TSharedPtr<FStreamableHandle> StreamableHandle = StreamManager.RequestAsyncLoad(TargetToStream, FStreamableDelegate(), FStreamableManager::DefaultAsyncLoadPriority, true);
	AddResourceHandles(nullptr, StreamableHandle, true);
}

void UResourceManager::BackstageAsyncLoad_ClassPtr(const TSoftClassPtr<UObject>& TargetToStream)
{
	BackstageAsyncLoad_ObjectPath(TargetToStream.ToSoftObjectPath());
}

void UResourceManager::BackstageAsyncLoad_ObjectPtr(const TSoftObjectPtr<UObject>& TargetToStream)
{
	BackstageAsyncLoad_ObjectPath(TargetToStream.ToSoftObjectPath());
}

UObject* UResourceManager::SynchronousLoad(const FStringAssetReference& TargetToStream, bool bManageActiveHandle/* = false*/)
{
	if (bManageActiveHandle)
	{
		TSharedPtr<FStreamableHandle> StreamableHandle;
		UObject* ResultObj = StreamManager.LoadSynchronous(TargetToStream, bManageActiveHandle, &StreamableHandle);
		ManagedActiveStreamableHandles.AddUnique(StreamableHandle);
		return ResultObj;
	}
	else
	{
		return StreamManager.LoadSynchronous(TargetToStream, bManageActiveHandle);
	}
}

UObject* UResourceManager::SynchronousLoad(const TSoftObjectPtr<UObject>& TargetToStream, bool bManageActiveHandle/* = false*/)
{
	if (bManageActiveHandle)
	{
		TSharedPtr<FStreamableHandle> StreamableHandle;
		UObject* ResultObj = StreamManager.LoadSynchronous(TargetToStream, bManageActiveHandle, &StreamableHandle);
		ManagedActiveStreamableHandles.AddUnique(StreamableHandle);
		return ResultObj;
	}
	else
	{
		return StreamManager.LoadSynchronous(TargetToStream, bManageActiveHandle);
	}
}

UClass* UResourceManager::SynchronousLoadClass(const TSoftClassPtr<UObject>& TargetToStream, bool bManageActiveHandle /*= false*/)
{
	if (bManageActiveHandle)
	{
		TSharedPtr<FStreamableHandle> StreamableHandle;
		UClass* ResultObj = StreamManager.LoadSynchronous(TargetToStream, bManageActiveHandle, &StreamableHandle);
		ManagedActiveStreamableHandles.AddUnique(StreamableHandle);
		return ResultObj;
	}
	else
	{
		return StreamManager.LoadSynchronous(TargetToStream, bManageActiveHandle);
	}
}

void UResourceManager::FindLoadFaileAssetPathToLog(TArray<FStringAssetReference> LoadPartArray)
{
	for (int32 i = 0; i < LoadPartArray.Num(); i++)
	{
		FStringAssetReference asset1 = LoadPartArray[i];
		UObject* itemObj = asset1.TryLoad();
		if (!itemObj)
		{
			//CS3_ERROR(TEXT("-->Null Pointer error:UResourceManager::FindLoadFaileAssetPathToLog : itemObj!"));
		}
	}
}

void UResourceManager::CheckMultipleAssetFileIsExist(TArray<FStringAssetReference> LoadPartArray, int32 EntityID, FString ModelID)
{
	for (int32 i = 0; i < LoadPartArray.Num(); i++)
	{
		FStringAssetReference TargetToStream = LoadPartArray[i];
		if (!CheckAssetFileIsExist(TargetToStream))
		{
			//CS3_ERROR(TEXT("RequestAsyncLoad File is not exist!!!! Asset[path=%s] EntityID=%d ModelID=%s assetName=%s"), *TargetToStream.ToString(), EntityID, *ModelID, *TargetToStream.GetAssetName());
		}
	}
}

bool UResourceManager::CheckAssetFileIsExist(const FStringAssetReference& TargetToStream)
{
	FString FilePath = TargetToStream.ToString();
	FString	FileName;
	FilePath.Split(TEXT("."), &FileName, NULL);
	FileName = FileName.Append(TEXT(".uasset"));
	FString realFilePath;
	FPackageName::TryConvertLongPackageNameToFilename(FileName, realFilePath, TEXT(""));
	if (FPaths::FileExists(realFilePath))
	{
		return true;
	}
	else
	{
		return false;
	}
}

TAsyncLoadPriority UResourceManager::GetAsyncLoadPriority(RESOURCE_LOAD_PRIORITY_TYPE type, FVector Position)
{
	TAsyncLoadPriority Priority = 0;
	AServerCharacter* ServerCharacter = UClientBPFunctionLibrary::GetPlayerCharacter();
	if (ServerCharacter != nullptr)
	{
		if (Position != FVector::ZeroVector)
		{
			if (Position.Equals(ServerCharacter->GetActorLocation()))		//玩家自己给予最高加载优先级，使得加载进度条的时候玩家能更快速加载
			{
				Priority = 100;
			}
			else if (FVector::Dist(ServerCharacter->GetActorLocation(), Position) <= RESOURCE_LOAD_DISTANCE[0])
			{
				Priority = ResourceLoadPriority[type][0];
			}
			else if (FVector::Dist(ServerCharacter->GetActorLocation(), Position) <= RESOURCE_LOAD_DISTANCE[1])
			{
				Priority = ResourceLoadPriority[type][1];
			}
			else
			{
				Priority = ResourceLoadPriority[type][2];
			}
		}
		else
		{
			Priority = ResourceLoadPriority[type][0];
		}

	}
	return Priority;
}

void UResourceManager::ReleaseManageActiveHandles()
{
	for (auto Handle: ManagedActiveStreamableHandles)
	{
		if (Handle.IsValid())
		{
			Handle->CancelHandle();
			Handle.Reset();
		}
	}
}

void UResourceManager::ReleaseHandlesByObject(const void * UserObject)
{
	if (!IsValid(this))
	{
		//保证shutdown的正确性
		//CS3_DEBUG(TEXT("UResourceManager is NULL"));
		return;
	}
	if (UserObject)
	{
		TSet<TWeakPtr<FStreamableHandle>>* StreamableHandles = AllStreamableHandles.Find(UserObject);
		if (StreamableHandles)
		{
			TSet<TWeakPtr<FStreamableHandle>> LocalStreamableHandles = *StreamableHandles;
			for (TWeakPtr<FStreamableHandle> WeakHandle : LocalStreamableHandles)
			{
				TSharedPtr<FStreamableHandle> Handle = WeakHandle.Pin();
				if (Handle.IsValid() && !ManagedActiveStreamableHandles.Contains(Handle))
				{
					Handle->CancelHandle();
					Handle.Reset();
				}
			}
		}
	}
}

void UResourceManager::ReleaseAllHandles()
{
	for (TPair<const void *, TSet<TWeakPtr<FStreamableHandle>>> HandleItem:AllStreamableHandles)
	{
		TSet<TWeakPtr<FStreamableHandle>> StreamHandles = HandleItem.Value;
		if (StreamHandles.Num() > 0)
		{
			for (TWeakPtr<FStreamableHandle> WeakHandle : StreamHandles)
			{
				TSharedPtr<FStreamableHandle> Handle = WeakHandle.Pin();
				if (Handle.IsValid() && !ManagedActiveStreamableHandles.Contains(Handle))
				{
					Handle->CancelHandle();
					Handle.Reset();
				}
			}
		}
	}
	AllStreamableHandles.Empty();
	ManagedActiveStreamableHandles.Empty();
}

void UResourceManager::AddResourceHandles(const void * UserObject, TSharedPtr<FStreamableHandle> StreamableHandle, bool bManageActiveHandle/* = false */)
{
	if (bManageActiveHandle)
	{
		ManagedActiveStreamableHandles.AddUnique(StreamableHandle);
	}
	if (UserObject)
	{
		AllStreamableHandles.FindOrAdd(UserObject).Add(StreamableHandle);
	}
}

