#include "RoleFilterActorComponent.h"
#include "../../../Character/ServerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#pragma optimize("", off)
URoleFilterActorComponent::URoleFilterActorComponent()
{
	IsReceiveNewPositionOrDirection = false;
	IsNeedToUpdate = false;
	TotalTime = 0.0f;
	LastRotation = FRotator::ZeroRotator;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URoleFilterActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void URoleFilterActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateCharacterPositionAndDirection(DeltaTime);	
}

void URoleFilterActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URoleFilterActorComponent::OnUpdateVolatileData(const FVector& position, const FVector& direction, int32 parentID)
{	
	StoredInputs.time_ = GetWorld()->GetTimeSeconds();
	StoredInputs.position_ = position;
	StoredInputs.direction_ = direction;
	StoredInputs.parentID_ = parentID;
	IsReceiveNewPositionOrDirection = true;
	IsNeedToUpdate = true;
}

void URoleFilterActorComponent::SetPosition(const FVector& position, int32 parentID)
{
	
}

void URoleFilterActorComponent::SetDirection(const FVector& direction, int32 parentID)
{
	
}

void URoleFilterActorComponent::UpdateCharacterPositionAndDirection(float DeltaTime)
{
	//如果不需要更新，直接返回
	if (!IsNeedToUpdate)
		return;

	AServerCharacter* ServerCharacter = Cast<AServerCharacter>(GetOwner());
	if (ServerCharacter->IsDestroyed())
		return;

	///停止更新
	float CurrDistance = FVector::Dist(StoredInputs.position_, ServerCharacter->GetActorLocation());
	if (CurrDistance < STOP_MOVE_DIST && FMath::IsNearlyEqual(FRotator::ClampAxis(StoredInputs.direction_.Z), FRotator::ClampAxis(ServerCharacter->GetActorRotation().Yaw), STOP_ROTATOR_ANGLE))
	{
		IsNeedToUpdate = false;
		return;
	}

	//移动（或转动）到目标位置（或角度）所需的时间
	if (IsReceiveNewPositionOrDirection)
	{	
		if (CurrDistance < STOP_MOVE_DIST)
		{
			TotalTime = 0.1f;
		}
		else
		{
			TotalTime = CurrDistance / (ServerCharacter->GetCharacterMovement()->GetMaxSpeed());
		}
		LastRotation = ServerCharacter->GetActorRotation();
		IsReceiveNewPositionOrDirection = false;
	}

	//位移处理
	if (CurrDistance > STOP_MOVE_DIST)
	{
		///<计算2D朝向
		FVector VectorDirection = StoredInputs.position_ - ServerCharacter->GetActorLocation();
		VectorDirection.Normalize();
		float MoveDistance = (VectorDirection * ServerCharacter->GetVelocity().Size()).Size();//位移
		if (CurrDistance >= MoveDistance)
		{
			ServerCharacter->AddMovementInput(VectorDirection);
		}
		else
		{
			float Scale = CurrDistance / MoveDistance;
			ServerCharacter->AddMovementInput(VectorDirection, Scale);
		}
	}

	//角度处理
	FRotator CharacterRotation = FRotator::ZeroRotator;
	CharacterRotation = ServerCharacter->GetActorRotation();

	if( (DeltaTime / TotalTime) >1.0f)
	{
		CharacterRotation.Yaw = StoredInputs.direction_.Z;
		ServerCharacter->SetActorRotation(CharacterRotation);
		return;
	}

	if (FMath::IsNearlyEqual(ServerCharacter->GetActorRotation().Yaw, StoredInputs.direction_.Z, STOP_ROTATOR_ANGLE))
		return;

	CharacterRotation.Yaw = FMath::FInterpTo(sameSignDegree(StoredInputs.direction_.Z, CharacterRotation.Yaw), StoredInputs.direction_.Z, 1.0f, FMath::Min(DeltaTime / TotalTime, 0.2f));
	ServerCharacter->SetActorRotation(CharacterRotation);
}

void URoleFilterActorComponent::OnUpdateVolatileDataByParent(const FVector& position, const FVector& direction, int32 parentID)
{
	
}

/**
* 把两个不同符号的角度換成相同符号的角度
*/
float URoleFilterActorComponent::sameSignDegree(float degree, float closer)
{
	if (closer > degree + 180.0)
		return closer - 2.0 * 180.0;
	else if (closer < degree - 180.0)
		return closer + 2.0 * 180.0;
	else
		return closer;
}

FVector URoleFilterActorComponent::sameSignDegree(const FVector& degree, const FVector& closer)
{
	FVector v;
	v.X = sameSignDegree(degree.X, closer.X);
	v.Y = sameSignDegree(degree.Y, closer.Y);
	v.Z = sameSignDegree(degree.Z, closer.Z);
	return v;
}
#pragma optimize("", on)