// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"


UCLASS()
class CLIENT_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());///<初始组件（组件的创建）
	
	//--------- Begin Super Interface
	virtual void PostActorCreated()override;
	virtual void OnConstruction(const FTransform& Transform)override;///<可以处理与蓝图属性相关的操作
	virtual void PostRegisterAllComponents() override;///<Actor组件注册完成后
	virtual void PostInitializeComponents() override;///<Actor组件初始化完成后	（组件数据的更新）	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
	//-------------End Super Interface

	//-----------------Begin InitComponent
	virtual void InitCharacterDefaultComponent();
	virtual void InitCharacterComponentPostActorCreated();///<由于在构造函数中创建组件根据执行顺序，继承GameCharacter需要创建额外的组件在构造函数中调用自己的InitBaseComponent()，
	virtual void InitCapsuleComponent();///<初始化胶囊体组件数据（Character构造函数已创建）
	virtual void InitCharacterMovementComponent();///<初始化移动组件数据（Character构造函数已创建）
	virtual void InitAppearanceComponent();///<创建外观组件	

	//-----------------End InitComponent

	//-----------------Begin Appearance
	virtual void CreateAppearance();
	virtual void CreateApperanceOver();
	//-----------------End Appearance

	bool IsDestroyed();///<游戏层的逻辑判断 当前GameCharacter是否有效

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Heads;///<头部

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Hair;///<头发 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Adorn;///<头饰

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Hat;///<帽子

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Coats;///<上衣

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Wrist;///<护腕

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Gloves;///<手套

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Belt;///<腰带

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Necklace;///<项链

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_L_RING;///<左手戒指

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_R_RING;///<右手戒指

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Pants;///<裤子

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Shoes;///<鞋子

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SKMesh_Cloak;///<披风

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* STMesh_LHweapon;///<左手武器

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* STMesh_RHweapon;///<右手武器

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* STMesh_DefaultStatic;///<默认静态组件

protected:
	bool bIsDestroyed = false;///<当前GameCharacter是否销毁

	class UEquipComponent* EquipComponent;


};
