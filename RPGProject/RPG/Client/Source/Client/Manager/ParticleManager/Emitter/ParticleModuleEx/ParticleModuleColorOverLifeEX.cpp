// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleModuleColorOverLifeEX.h"
// UE4
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/TypeData/ParticleModuleTypeDataBase.h"
#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"
#include "ParticleHelper.h"
// 数据类型

// CS3类型

UParticleModuleColorOverLifeEX::UParticleModuleColorOverLifeEX(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bSpawnModule = true;
	bUpdateModule = true;
}

void UParticleModuleColorOverLifeEX::Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase)
{
	// 这个地方不会为空，只是PVS检测问题，加个判断
	if (Owner == nullptr)
	{
		return;
	}

	// 获取自身的粒子
	AEffectEmitter* Emitter = Cast<AEffectEmitter>(Owner->Component->GetOwner());

	SPAWN_INIT;
	if (Emitter && Emitter->IsUseConstAlpha())
	{
		Particle.Color.A *= Emitter->GetAlpha();
		Particle.BaseColor.A *= Emitter->GetAlpha();
	}

	if (Emitter && Emitter->IsUseConstAlpha())
	{
		Particle.Color.A *= Emitter->GetInitAlpha();
		Particle.BaseColor.A *= Emitter->GetInitAlpha();
	}
}

void UParticleModuleColorOverLifeEX::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{
	// 这个地方不会为空，只是PVS检测问题，加个判断
	if (Owner == nullptr)
	{
		return;
	}

	AEffectEmitter* Emitter = Cast<AEffectEmitter>(Owner->Component->GetOwner());

	BEGIN_UPDATE_LOOP;
	{
		if (Emitter && Emitter->IsGraduallyHidden())
		{
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride));
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride) + PLATFORM_CACHE_LINE_SIZE);
			Particle.Color.A *= Emitter->GetAlpha();
		}

		if (Emitter && Emitter->IsUseConstAlpha())
		{
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride));
			FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride) + PLATFORM_CACHE_LINE_SIZE);
			Particle.Color.A *= Emitter->GetInitAlpha();
		}
	}
	END_UPDATE_LOOP;
}

UParticleModuleTransformEX::UParticleModuleTransformEX(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSpawnModule = true;
	bUpdateModule = true;
	CollisionType.Add(ECC_WorldStatic);
	CollisionType.Add(ECC_WorldDynamic);
	CollisionType.Add(ECC_GameTraceChannel3);
	CollisionType.Add(ECC_GameTraceChannel4);
	CollisionType.Add(ECC_GameTraceChannel5);
	CollisionType.Add(ECC_GameTraceChannel6);
	CollisionType.Add(ECC_GameTraceChannel7);
	CollisionType.Add(ECC_GameTraceChannel8);
}
#pragma optimize("", off)
void UParticleModuleTransformEX::Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime)
{
	// 这个地方不会为空，只是PVS检测问题，加个判断
	if (Owner == nullptr)
	{
		return;
	}
	
	AEffectEmitter* Emitter = Cast<AEffectEmitter>(Owner->Component->GetOwner());
	
	if (!Emitter)
	{
		return;
	}

	int32 MoudelIndex = 0;
	UParticleEmitter* CurrentEmitter;
	bool EndLoop = false;
	for (int32 i = 0; i < Owner->Component->Template->Emitters.Num(); i++)
	{
		CurrentEmitter = Owner->Component->Template->Emitters[i];
		if (Owner->CurrentLODLevel == CurrentEmitter->LODLevels[Owner->CurrentLODLevelIndex])
		{
			// 当前的EmitterIndex
			MoudelIndex = i;
			// 检测本帧粒子是否已经更新完毕
			if (i == Owner->Component->Template->Emitters.Num() - 1)
			{
				EndLoop = true;
			}
			break;
		}
	}

	// 只有MeshData可用
	if (!Owner->CurrentLODLevel->TypeDataModule || !(Owner->CurrentLODLevel->TypeDataModule->IsA(UParticleModuleTypeDataMesh::StaticClass())))
	{
		if (EndLoop && Emitter)
		{
			Emitter->ParticleUpdateLoopEnd();
		}
		return;
	}

	const int32 MeshRotationOffset = Owner->GetMeshRotationOffset();
	bool bUseLocalSpace = Owner->CurrentLODLevel->RequiredModule->bUseLocalSpace;

	FVector Location;
	FRotator Rotation;
	FVector ImpactPoint;

	BEGIN_UPDATE_LOOP;
	{

		FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride));
		FPlatformMisc::Prefetch(ParticleData, (ParticleIndices[i + 1] * ParticleStride) + PLATFORM_CACHE_LINE_SIZE);

		FMeshRotationPayloadData* PayloadData = (FMeshRotationPayloadData*)((uint8*)&Particle + MeshRotationOffset);
			
		///获取粒子的方向
		if (bUseLocalSpace)
		{
			Location = Owner->Component->GetComponentTransform().TransformPosition(Particle.Location);
			Rotation = Owner->Component->GetComponentTransform().TransformRotation(FRotator(PayloadData->Rotation.Y, PayloadData->Rotation.Z, PayloadData->Rotation.X).Quaternion()).Rotator();
		}
		else
		{
			Location = Particle.Location;
			Rotation = FRotator(PayloadData->Rotation.Y, PayloadData->Rotation.Z, PayloadData->Rotation.X);
		}

		if (bTrace)
		{
			ImpactPoint = GetLocation(Owner, Location, Rotation);
		}
		else
		{
			ImpactPoint = FVector(0.f);
		}

		// 发送粒子的位置，朝向和碰撞点
		Emitter->GetBaseParticleLocationAndRotation(Location, Rotation, MoudelIndex, i, bTrace, ImpactPoint, Template);

		// 调整缩放
		if ((ImpactPoint - FVector(0.f)).Size() > 10.f)
		{
			float Distance = (Location - ImpactPoint).Size();

			if (bTransformScaleX)
			{
				Particle.Size.X = Distance / MeshSize.X / Owner->Component->GetComponentScale().X;
			}
			if (bTransformScaleY)
			{
				Particle.Size.Y = Distance / MeshSize.Y / Owner->Component->GetComponentScale().Y;
			}
			if (bTransformScaleZ)
			{
				Particle.Size.Z = Distance / MeshSize.Z / Owner->Component->GetComponentScale().Z;
			}
		}
		
	}
	END_UPDATE_LOOP;

	if (EndLoop && Emitter)
	{
		Emitter->ParticleUpdateLoopEnd();
	}
}

FVector UParticleModuleTransformEX::GetLocation(FParticleEmitterInstance* Owner, FVector& Location, FRotator& Rotation)
{
	UWorld* World = Owner->Component->GetWorld();
	AEffectEmitter* Emitter = Cast<AEffectEmitter>(Owner->Component->GetOwner());

	if (World && Emitter)
	{
		// 射线的方向
		FVector Direction = FTransform(Rotation.Quaternion(), Location, FVector(1.f)).TransformRotation(RotationOffset.Quaternion()).GetAxisX();

		FHitResult Hit;
		
		FCollisionObjectQueryParams ObjectQueryParams;
		for (int32 i = 0; i < CollisionType.Num(); i++)
		{
			ObjectQueryParams.AddObjectTypesToQuery(CollisionType[i]);
		}

		FCollisionQueryParams QueryParams;
		//QueryParams.bTraceAsyncScene = true;

		QueryParams.AddIgnoredActor(Emitter);

		// 查询碰撞点
		if (World->LineTraceSingleByObjectType(Hit, Location, Location - Direction * TraceLength, ObjectQueryParams, QueryParams))
		{
			return Hit.ImpactPoint;
		}
	}
	return FVector(0.f);
}
#pragma optimize("", on)
