#pragma once

#include "GameFramework/Actor.h"
#include "BaseSplineAsset.generated.h"

/*
* 文件名称：BaseSplineAsset.h
* 功能说明：路径资源基类
* 文件作者：zhangfudong
* 目前维护：zhangfudong
* 创建时间：2017-9-01
*/

//所有技能路径资源继承此类

class USplineComponent;

UCLASS(BlueprintType, Blueprintable)
class ABaseSplineAsset : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSplineAsset();

protected:
	// Called when the game starts or when spawned
	

public:	

	///让粒子在路径上移动
	UFUNCTION(BlueprintCallable, Category = "SplinePath")
		void MoveOnSpline(UParticleSystemComponent* Target, bool bHaveOffset, float DeltaTime);

 	UFUNCTION(BlueprintCallable, Category = "SplinePath")
 		void MeshMoveOnSpline(USkeletalMeshComponent* Target, bool bHaveOffset, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "SplinePath")
		void ActorMoveOnSpline(AActor* Target, float DeltaTime);

	UFUNCTION(Blueprintpure, Category = "SplinePath")
		FORCEINLINE float GetDistance()
		{
			return CurrentDistance;
		}

	UFUNCTION(Blueprintpure, Category = "SplinePath")
		FORCEINLINE bool GetState()
		{
			return bMoving;
		}

private:
	///设置位置
	void SetTargetTransform(UParticleSystemComponent* Target, bool bHaveOffset);

	///设置骨架网格物体位置
	void SetTargetTransform(USkeletalMeshComponent* Target, bool bHaveOffset);

	///设置Actor位置
	void SetTargetTransform(AActor* Target);

	///设置移动轨迹的位置并判断是否可移动
	bool SetDistance(float DeltaTime);

public:
	///当前路径
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USplineComponent *SplineComponent;

	///粒子当前的运动圈数
	UPROPERTY(BlueprintReadOnly)
		int32 LocalNumber = 0;

	///粒子的运动圈数
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
		int32 LoopNumber = 0;

	///粒子运动速度
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
		float MoveSpeed;

protected:
	///粒子当前在路径上的距离
	UPROPERTY(BlueprintReadOnly)
		float CurrentDistance = 0.0f;

	///粒子上一帧在路径上的距离
	UPROPERTY(BlueprintReadOnly)
		float LastDistance = 0.0f;

	///路径上移动添加的偏移量
	FRotator Offset = FRotator(0, -90, 0);

	///标识是否在移动的状态
	bool bMoving = true;
};
