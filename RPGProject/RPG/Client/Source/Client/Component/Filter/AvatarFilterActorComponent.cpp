#include "AvatarFilterActorComponent.h"
#include "../../Character/ServerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#pragma optimize("", off)
UAvatarFilterActorComponent::UAvatarFilterActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAvatarFilterActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UAvatarFilterActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateCharacterPositionAndDirection(DeltaTime);
	
}

void UAvatarFilterActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UAvatarFilterActorComponent::OnUpdateVolatileData(const FVector& position, const FVector& direction, int32 parentID)
{
	AServerCharacter* ServerCharacter = Cast<AServerCharacter>(GetOwner());
	if (ServerCharacter->IsDestroyed())
	{
		return;
	}
	StoredInputs.time_ = GetWorld()->GetTimeSeconds();
	StoredInputs.position_ = position;
	StoredInputs.direction_ = direction;
	StoredInputs.parentID_ = parentID;
	IsNeedToUpdate = true;
	FRotator CurrentRotation= ServerCharacter->GetActorRotation();
	CurrentRotation.Yaw = StoredInputs.direction_.Z;
	ServerCharacter->FaceRotation(CurrentRotation);
}

void UAvatarFilterActorComponent::SetPosition(const FVector& position, int32 parentID)
{
	
}

void UAvatarFilterActorComponent::SetDirection(const FVector& direction, int32 parentID)
{
	
}

void UAvatarFilterActorComponent::SetPosBySerPos(const int32 vehicleID, const FVector& position, const FVector& direction)
{

}


void UAvatarFilterActorComponent::UpdateCharacterPositionAndDirection(float DeltaTime)
{
	//如果不需要更新，直接返回
	if (IsNeedToUpdate)
		return;

	AServerCharacter* ServerCharacter = Cast<AServerCharacter>(GetOwner());
	if (ServerCharacter->IsDestroyed())
	{
		return;
	}
	///<停止移动
	float Dist = FVector::Dist(StoredInputs.position_, ServerCharacter->GetActorLocation());
	if (Dist < STOP_MOVE_DIST)
	{
		IsNeedToUpdate = false;
		StoredInputs.ResetData();
		return;
	}

	if (Dist > STOP_MOVE_DIST)
	{
		///<计算朝向
		FVector VectorDirection =StoredInputs.position_- ServerCharacter->GetActorLocation();
		VectorDirection.Normalize();
		float Displacement = (VectorDirection * ServerCharacter->GetVelocity().Size()).Size();//位移
		if (Dist >= Displacement)
		{
			ServerCharacter->AddMovementInput(VectorDirection);
		}
		else
		{
			float Scale = Dist / Displacement;
			ServerCharacter->AddMovementInput(VectorDirection, Scale);
		}
	}
}

void UAvatarFilterActorComponent::OnUpdateVolatileDataByParent(const FVector& position, const FVector& direction, int32 parentID)
{
	
}

void UAvatarFilterActorComponent::OnGotParentEntity(KBEngine::Entity* parentEnt)
{

}

void UAvatarFilterActorComponent::OnLoseParentEntity()
{

}
#pragma optimize("", on)