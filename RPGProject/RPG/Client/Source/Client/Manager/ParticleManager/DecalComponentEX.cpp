// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalComponentEX.h"


void UDecalComponentEX::SendRenderTransform_Concurrent()
{
	//If Decal isn't hidden update its transform.
	if (ShouldComponentAddToScene() && ShouldRender())
	{
		//在渲染前修正贴花的Transform，每帧更新
		SetComponentTransform();
		GetWorld()->Scene->UpdateDecalTransform(this);
	}

	UDecalComponent::Super::SendRenderTransform_Concurrent();
}

void UDecalComponentEX::CreateRenderState_Concurrent()
{
	UDecalComponent::Super::CreateRenderState_Concurrent();

	// Mimics UPrimitiveComponent's visibility logic, although without the UPrimitiveCompoent visibility flags
	if (ShouldComponentAddToScene() && ShouldRender())
	{
		//在渲染前修正贴花的Transform，初始化更新
		SetComponentTransform();
		GetWorld()->Scene->AddDecal(this);
	}
}

void UDecalComponentEX::SetComponentTransform()
{
	RealYaw = 0;
	ParentComponent = this;

	FTransform& Ret = const_cast<FTransform&>(GetComponentToWorld());
	FVector CurScale = FVector(Ret.GetScale3D().Z, Ret.GetScale3D().X, Ret.GetScale3D().Y);

	CalculateDecalRealYaw();
	//根据up向量修正真实的Yaw值
	if (GetUpVector().Z < 0)
	{
		RealYaw += 180;
	}
	//修正贴花组件的真实旋转，交换scale X,Y,Z的位置
	//X,Y贴花的放缩比例，Z贴花的高度
	//原Z,Y贴花的放缩比例，X贴花的高度
	Ret.SetRotation(FRotator(-90, RealYaw, 0).Quaternion());
	Ret.SetScale3D(CurScale);
}

void UDecalComponentEX::CalculateDecalRealYaw()
{
	//递归累加组件真实的Yaw值
	ParentComponent = ParentComponent->GetAttachParent();
	if (ParentComponent && IsValid(ParentComponent))
	{
		RealYaw += ParentComponent->GetRelativeTransform().GetRotation().Rotator().Yaw;
		CalculateDecalRealYaw();
	}
	else
	{
		RealYaw += GetRelativeTransform().GetRotation().Rotator().Yaw;
	}
}