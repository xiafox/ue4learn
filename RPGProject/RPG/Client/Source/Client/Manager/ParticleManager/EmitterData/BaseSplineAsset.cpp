

#include "BaseSplineAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SplineComponent.h"
#include "Manager/ParticleManager//Emitter/EffectEmitter.h"

ABaseSplineAsset::ABaseSplineAsset()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	AddOwnedComponent(SplineComponent);
}

void ABaseSplineAsset::MoveOnSpline(UParticleSystemComponent* Target, bool bHaveOffset, float DeltaTime)
{
	if (bMoving)
	{
		if (SetDistance(DeltaTime))
		{
			SetTargetTransform(Target, bHaveOffset);
		}
		else
		{
			AEffectEmitter* Emitter = Cast<AEffectEmitter>(Target->GetOwner());
			if (Emitter)
			{
				//光效对象抵达目的地
				Emitter->OnEffectArrived.Broadcast();
				//CS3_DEBUG(TEXT("the actor end motion,particle id:%s"), *Emitter->GetEffectID());
			}
		}
	}
}

 void ABaseSplineAsset::MeshMoveOnSpline(USkeletalMeshComponent* Target, bool bHaveOffset, float DeltaTime)
 {
 	if (bMoving)
 	{
 		if (SetDistance(DeltaTime))
 		{
 			SetTargetTransform(Target, bHaveOffset);
 		}
 		else
 		{
 			///抵达目的地
 		}
 	}
 }

bool ABaseSplineAsset::SetDistance(float DeltaTime)
{
	if (LocalNumber < LoopNumber)
	{
		LastDistance = CurrentDistance;
		CurrentDistance += DeltaTime * MoveSpeed;
		if (CurrentDistance > SplineComponent->GetSplineLength())
		{
			LocalNumber++;
			if (LocalNumber == LoopNumber)
			{
				CurrentDistance = SplineComponent->GetSplineLength();
			}
			else
			{
				CurrentDistance = 0.0f;
			}
			return true;
		}
		else
		{
			return true;
		}
	}
	else
	{
		CurrentDistance = 0.0f;
		bMoving = false;
		return false;
	}
}

void ABaseSplineAsset::SetTargetTransform(UParticleSystemComponent* Target, bool bHaveOffset)
{
	while (CurrentDistance - LastDistance > 60)
	{
		LastDistance += 60;
		FRotator _R = SplineComponent->GetRotationAtDistanceAlongSpline(LastDistance, ESplineCoordinateSpace::World);
		FVector _FL = SplineComponent->GetLocationAtDistanceAlongSpline(LastDistance, ESplineCoordinateSpace::World);

		if (bHaveOffset)
		{
			FVector _FS = FVector(1, 1, 1);
			FTransform _T = FTransform(_R, _FL, _FS);
			
			Target->SetWorldLocationAndRotation(_FL, _T.TransformRotation(Offset.Quaternion()).Rotator());
		}
		else
		{
			Target->SetWorldLocationAndRotation(_FL, _R);
		}
	}

	FRotator _CR = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FVector _CFL = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

	if (bHaveOffset)
	{
		FVector _CFS = FVector(1, 1, 1);
		FTransform _CT = FTransform(_CR, _CFL, _CFS);
		Target->SetWorldLocationAndRotation(_CFL, _CT.TransformRotation(Offset.Quaternion()).Rotator());
	}
	else
	{
		Target->SetWorldLocationAndRotation(_CFL, _CR);
	}
}

void ABaseSplineAsset::SetTargetTransform(USkeletalMeshComponent* Target, bool bHaveOffset)
{
	while (CurrentDistance - LastDistance > 60)
	{
		LastDistance += 60;
		FRotator _R = SplineComponent->GetRotationAtDistanceAlongSpline(LastDistance, ESplineCoordinateSpace::World);
		FVector _FL = SplineComponent->GetLocationAtDistanceAlongSpline(LastDistance, ESplineCoordinateSpace::World);

		if (bHaveOffset)
		{
			FVector _FS = FVector(1, 1, 1);
			FTransform _T = FTransform(_R, _FL, _FS);
			Target->SetWorldLocationAndRotation(_FL, _T.TransformRotation(Offset.Quaternion()).Rotator());
		}
		else
		{
			Target->SetWorldLocationAndRotation(_FL, _R);
		}
	}

	FRotator _CR = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FVector _CFL = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

	if (bHaveOffset)
	{
		FVector _CFS = FVector(1, 1, 1);
		FTransform _CT = FTransform(_CR, _CFL, _CFS);
		Target->SetWorldLocationAndRotation(_CFL, _CT.TransformRotation(Offset.Quaternion()).Rotator());
	}
	else
	{
		Target->SetWorldLocationAndRotation(_CFL, _CR);
	}
}

void ABaseSplineAsset::ActorMoveOnSpline(AActor* Target, float DeltaTime)
{
	if (bMoving)
	{
		if (SetDistance(DeltaTime))
		{
			SetTargetTransform(Target);
		}
	}
}

void ABaseSplineAsset::SetTargetTransform(AActor* Target)
{
	FRotator Rotation;
	FVector Location;
	while (CurrentDistance - LastDistance > 60)
	{
		LastDistance += 60;
		Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(LastDistance, ESplineCoordinateSpace::World);
		Location = SplineComponent->GetLocationAtDistanceAlongSpline(LastDistance, ESplineCoordinateSpace::World);

		Target->SetActorLocationAndRotation(Location, Rotation);
	}

	Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	Location = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

	Target->SetActorLocationAndRotation(Location, Rotation);
}

