#include "PosSyncComponent.h"
#include "../../../Character/ServerCharacter.h"

UPosSyncComponent::UPosSyncComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPosSyncComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AServerCharacter* MyCharacter = Cast<AServerCharacter>(GetOwner());

	if (nullptr != MyCharacter&& MyCharacter->GetSelfEntity()!=nullptr)
	{
		FVector ActorLocation = MyCharacter->GetActorLocation();

		FVector ActorDirection = MyCharacter->GetSelfEntity()->Direction();
		ActorDirection.Z = GetOwner()->GetActorRotation().Euler().Z;
		//每tick都把坐标同步到底层，以便底层能同步最新的位置信息到其它人身上	
		float DisHeight = MyCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		MyCharacter->GetSelfEntity()->UpdateVolatileDataToServer(ActorLocation - FVector(0.0, 0.0, DisHeight), ActorDirection);
	}
}
