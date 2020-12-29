// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyEffectEmitter.h"
#include "GameFramework/Character.h"
#include "Character/ServerCharacter.h"
// 数据类型

// CS3类型

#pragma optimize("", off)
AFlyEffectEmitter::AFlyEffectEmitter()
{
	
}

void AFlyEffectEmitter::BeginPlay()
{
	// 飞行类光效，统统开启Tick
	PrimaryActorTick.bCanEverTick = true;

	// 绑定飞行光效抵达事件
	OnEffectArrived.AddDynamic(this, &AFlyEffectEmitter::OnFlyFinished);

	Super::BeginPlay();
}

void AFlyEffectEmitter::InitFromConfig()
{
	Super::InitFromConfig();
	OtherEffectData			= &(EffectEmitterData->OtherEffectData);

	// 第二阶段运动相关
	bCanSpecialMove			= OtherEffectData->bCanSpecialMove;
	TransformationDistance	= OtherEffectData->TransformationDistance;
	RisingDistance			= OtherEffectData->RisingDistance;

	// 速度
	FlySpeed				= OtherEffectData->FlySpeed;
	bFlyOfTime				= OtherEffectData->bFlyOfTime;
}

void AFlyEffectEmitter::InitEmitterTransform()
{
	ACharacter* Character = Cast<ACharacter>(FirstTarget);

	//设置位置
	FTransform Transform = Character && FirstTransform->Socket != "" ? Character->GetMesh()->GetSocketTransform(FName(*FirstTransform->Socket)) : FirstTarget->GetActorTransform();
	//根据bLocationOnWorld属性设置位置
	FVector Location;
	if (FirstTransform->bLocationOnWorld)
	{
		Location = FirstTransform->Transform.GetLocation();
	}
	else
	{
		Location = Transform.TransformPosition(FirstTransform->Transform.GetLocation());
	}
	//根据bRotationOnWorld属性设置位置
	FRotator Rotation;
	if (FirstTransform->bRotationOnWorld)
	{
		Rotation = FirstTransform->Transform.GetRotation().Rotator();
	}
	else
	{
		Rotation = Transform.TransformRotation(FirstTransform->Transform.GetRotation()).Rotator();
	}
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	//不绑定的光效为配置表缩放
	SetActorScale3D(FirstTransform->Transform.GetScale3D());

	//设置起始点
	SourcePos = GetActorLocation();
	//设置目标点
	GetFlyTargetPos(true);

	//设置朝向
	if (FirstTarget != SecondTarget)
	{
		FRotator LookRotation = FRotationMatrix::MakeFromX(TargetPos - FirstTarget->GetActorLocation()).Rotator();
		SetActorRotation(LookRotation);
	}

	//是否可以做第二阶段运动
	if ((TargetPos - GetActorLocation()).Size() < TransformationDistance && 
		TransformationDistance > 0.0f && RisingDistance > 0.0f)
	{
		bCanSpecialMove = false;
	}

	//设置移动平台相关属性
	if (CheckAndSetEmitterBase() && Cast<AServerCharacter>(ParentActor)->GetSelfEntity())
	{
		AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
	}
	
	////表现组件中添加此光效
	//if (Cast<AGameCharacter>(FirstTarget) && EffectData->bBindingVisibility)
	//{
	//	Cast<AGameCharacter>(FirstTarget)->GetPerformanceComponent()->AddEffect(UID);
	//}

	//有路径资源的光效在蓝图中初始化位置
	if (Spline)
	{
		InitSplineTransform();
	}
}

FVector AFlyEffectEmitter::GetFlyTargetPos(bool bInit /* = false */)
{

	// 设置目标飞行重点，判断SecondTransform的所有属性设置
	if ((SecondTransform->bLocationOnWorld || !SecondTransform->bBindSocket) && !bInit)
	{
		return TargetPos;
	}

	if (IsValid(SecondTarget))
	{
		ACharacter* Character = Cast<ACharacter>(SecondTarget);
		//刷新目标点，自己的位置加上相对偏移
		FTransform Transform;
		Transform = Character && SecondTransform->Socket != "" ? Character->GetMesh()->GetSocketTransform(FName(*SecondTransform->Socket)) : SecondTarget->GetActorTransform();
		TargetPos = Transform.TransformPosition(SecondTransform->Transform.GetLocation());
	}

	if (!SecondTransform->bBindSocket && SecondTransform->bLocationOnWorld)
	{
		return TargetPos = SecondTransform->Transform.GetLocation();
	}

	return TargetPos;
}

FVector AFlyEffectEmitter::GetFlySourcePos(bool bInit /* = false */)
{
	// 设置目标飞行重点，判断SecondTransform的所有属性设置
	if ((FirstTransform->bLocationOnWorld) && !bInit)
	{
		return SourcePos;
	}

	if (IsValid(FirstTarget))
	{
		ACharacter* Character = Cast<ACharacter>(FirstTarget);
		//刷新目标点，自己的位置加上相对偏移
		FTransform Transform;
		Transform = Character && FirstTransform->Socket != "" ? Character->GetMesh()->GetSocketTransform(FName(*FirstTransform->Socket)) : FirstTarget->GetActorTransform();
		SourcePos = Transform.TransformPosition(FirstTransform->Transform.GetLocation());
	}
	
	if (FirstTransform->bLocationOnWorld)
	{
		return SourcePos = FirstTransform->Transform.GetLocation();
	}

	return SourcePos;
}

void AFlyEffectEmitter::PlayTriggerParticle()
{
	for (auto TriggerID : OtherEffectData->TriggerEffect)
	{
		UClientGameInstance::Instance->ParticleManager->PlayParticle(TriggerID, 0.0f, FirstTarget, SecondTarget);
	}
}

void AFlyEffectEmitter::OnFlyFinished()
{
	PlayTriggerParticle();
}

void AFlyEffectEmitter::Tick(float DeltaTime)
{
	if (bPlaying)
	{
		//刷新目标点
		GetFlyTargetPos();

		//刷新移动平台，只有初始化的时候在移动平台上才检测
		if (ParentActor && !CheckAndSetEmitterBase())
		{
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		//根据飞行时间更新速度
		if (bFlyOfTime)
		{
			FlySpeed = (GetActorLocation() - FVector(0, 0, TotalRisingOffset) - TargetPos).Size() / (OtherEffectData->FlyTime - CurrentTime);
		}

		switch (FlyType)
		{
		case FLY_TYPE::NORMAL:
			NormalMove(DeltaTime);
			break;
		case FLY_TYPE::SECOND:
			SecondMove(DeltaTime);
			break;
		default:
			break;
		}
	}
	
	AEffectEmitter::Tick(DeltaTime);
}

void AFlyEffectEmitter::NormalMove(float DeltaTime, FVector RisingOffset /*= FVector(0,0,0)*/)
{

	//位置和方向
	FVector Location;
	FRotator Rotation;

	float MoveLength = FlySpeed * DeltaTime;
	float CurrentLength = 60;//这边设置为60是应该1倍的胶囊体直径为68，60保证可以接触到

	// 在丢帧的情况下也可以保证碰撞正常触发
	while (MoveLength - CurrentLength > 0)
	{
		// 世界朝向
		Rotation = FRotationMatrix::MakeFromX(TargetPos - GetActorLocation()).Rotator();
		
		if (ParentActor)
		{
			// 相对于移动平台的相对朝向
			FQuat Quaternion = ParentActor->GetActorTransform().InverseTransformRotation(Rotation.Quaternion());
			// Delta，相对位置移动，需要乘以缩放
			Location = Quaternion.GetAxisX() * CurrentLength * GetActorRelativeScale3D();
			// 实际的相对位置更新
			Location += RootComponent->RelativeLocation;
			// 重置朝向，加-90度偏移
			Rotation = (Quaternion * FRotator(0, -90, 0).Quaternion()).Rotator();
			
			SetActorRelativeTransform(FTransform(Rotation, Location, GetActorRelativeScale3D()));
		}
		else
		{
			// 位置
			Location = (TargetPos - GetActorLocation() + FVector(0, 0, TotalRisingOffset)).GetSafeNormal() * CurrentLength + GetActorLocation();

			// 重置朝向
			Rotation = (Rotation.Quaternion() * FRotator(0, -90, 0).Quaternion()).Rotator();

			SetActorLocationAndRotation(Location, Rotation);
		}

		MoveLength -= CurrentLength;

		// 检测是否已经到达,触发回调事件
		if ((GetActorLocation() - TargetPos - FVector(0, 0, TotalRisingOffset)).Size() < FlySpeed * DeltaTime)
		{
			SetActorLocation(TargetPos);
			OnEffectArrived.Broadcast();
			Stop();
			return;
		}
	}

	// 世界朝向
	Rotation = FRotationMatrix::MakeFromX(TargetPos - GetActorLocation()).Rotator();

	if (ParentActor)
	{
		// 相对于移动平台的相对朝向
		FQuat Quaternion = ParentActor->GetActorTransform().InverseTransformRotation(Rotation.Quaternion());
		// Delta，相对位置移动，需要乘以缩放
		Location = (Quaternion.GetAxisX() * MoveLength + Quaternion.GetAxisZ() * RisingOffset.Z) * GetActorRelativeScale3D();
		// 实际的相对位置更新
		Location += RootComponent->RelativeLocation;
		// 重置朝向，加-90度偏移
		Rotation = (Quaternion * FRotator(0, -90, 0).Quaternion()).Rotator();
		// 上升阶段重置朝向
		if (FlyType == FLY_TYPE::SECOND)
		{
			GetSecondMoveRotation(Rotation);
		}

		SetActorRelativeTransform(FTransform(Rotation, Location, GetActorRelativeScale3D()));
	}
	else
	{
		// 位置
		Location = (TargetPos - GetActorLocation() + FVector(0, 0, TotalRisingOffset)).GetSafeNormal() * MoveLength + RisingOffset + GetActorLocation();

		// 重置朝向
		Rotation = (Rotation.Quaternion() * FRotator(0, -90, 0).Quaternion()).Rotator();

		if (FlyType == FLY_TYPE::SECOND)
		{
			GetSecondMoveRotation(Rotation);
		}

		SetActorLocationAndRotation(Location, Rotation);
	}

	// 检测是否已经到达,触发回调事件
	if ((GetActorLocation() - TargetPos - FVector(0, 0, TotalRisingOffset)).Size() < FlySpeed * DeltaTime)
	{
		SetActorLocation(TargetPos);
		OnEffectArrived.Broadcast();
		Stop();
		return;
	}

	// 累计上升高度
	TotalRisingOffset += RisingOffset.Z;

	// 判断是否要进入下一阶段
	EnterNextMove();
}

void AFlyEffectEmitter::SecondMove(float DeltaTime)
{
	LastRisingPercent = RisingPercent;
	// 计算当前的进度
	if (bFlyOfTime)
	{
		RisingPercent = (CompletedTime - (OtherEffectData->FlyTime - CurrentTime)) / CompletedTime;
	}
	else
	{
		float CurrentPercent = 1 - (GetActorLocation() - FVector(0, 0, FMath::Sin(RisingPercent * PI) * RisingDistance) - TargetPos).Size() / TransformationDistance;

		if (CurrentPercent> RisingPercent)
		{
			RisingPercent = CurrentPercent;
		}
	}

	// 上升的距离
	float RisingLength = (FMath::Sin(RisingPercent * PI) - FMath::Sin(LastRisingPercent * PI)) * RisingDistance;
	
	NormalMove(DeltaTime, FVector(0, 0, RisingLength));
}

void AFlyEffectEmitter::GetSecondMoveRotation(FRotator& OutRotation)
{
	// 斜率
	float k = FMath::Cos(RisingPercent * PI) * (RisingDistance / (GetActorLocation() - FVector(0, 0, TotalRisingOffset) - TargetPos).Size());
	float Angle = -atan(k) * 180 / PI;
	OutRotation.Roll = Angle;
}

void AFlyEffectEmitter::EnterNextMove()
{
	// 判断是否要进入曲线阶段
	if (bCanSpecialMove && (GetActorLocation() - TargetPos).Size() < TransformationDistance)
	{
		FlyType = FLY_TYPE::SECOND;
		bCanSpecialMove = false;

		// 播放第二阶段动画
		if (AnimAsset.Num() > 1 && SKComponent)
		{
			SKComponent->PlayAnimation(AnimAsset[1], true);
		}

		if (bFlyOfTime)
		{
			CompletedTime = OtherEffectData->FlyTime - CurrentTime;
		}
	}
}

bool AFlyEffectEmitter::CheckAndSetEmitterBase()
{
	// 当施法者和目标都在相同的移动平台上时，绑定光效到移动平台上
	AServerCharacter* SFTarget = Cast<AServerCharacter>(FirstTarget);
	AServerCharacter* SSTarget = Cast<AServerCharacter>(SecondTarget);
	// 初始化的时候，判断两个目标的Parent，否则只判断目标
	if (!ParentActor)
	{
		if (SFTarget && SSTarget && SFTarget->GetSelfEntity() && SSTarget->GetSelfEntity() && SFTarget->GetSelfEntity()->ParentID() != 0
			&& SFTarget->GetSelfEntity()->ParentID() == SSTarget->GetSelfEntity()->ParentID())
		{
			AActor* MovingPlatform = SFTarget->GetSelfEntity()->Parent()->Actor();
			if (MovingPlatform)
			{
				ParentActor = MovingPlatform;
				return true;
			}
		}
	}
	else
	{
		if (SSTarget && SSTarget->GetSelfEntity() && SSTarget->GetSelfEntity()->ParentID())
		{
			return true;
		}
	}
	// 失败了清空
	ParentActor = nullptr;
	return false;
}

#pragma optimize("", on)
