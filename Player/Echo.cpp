// Fill out your copyright notice in the Description page of Project Settings.


#include "Echo.h"
#include "GroomComponent.h"
#include "PlayerAnim.h"

// Sets default values
AEcho::AEcho()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//생성자에서 에셋을 로딩하는방법 
	//Actor종류는 A가붙고 
	//Component,에셋들은 U가붙는다 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>  EchoAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/AncientContent/Characters/Echo/Meshes/Echo.Echo'"));

	//GetMesh()함수는 Chracter클래스에 만들어져 있는 SkeltalMeshComponent를 얻어오는 함수이다.
	if (EchoAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(EchoAsset.Object);
	}
	
	//hair
	
	HairComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("HairComponent"));
	HairComponent->SetupAttachment(GetMesh());
	
	StaticWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	StaticWeapon->SetupAttachment(GetMesh(), TEXT("weapon_r"));

	
	//anim
	static ConstructorHelpers::FClassFinder<UAnimInstance> EchoAnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/BP_EchoAnim.BP_EchoAnim_C'"));

	if (EchoAnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(EchoAnimAsset.Class);
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Player/MT_EchoAttack.MT_EchoAttack'"));

	if (AttackMontageAsset.Succeeded())
	{
		mAttackMontageArray.Add(AttackMontageAsset.Object);
	}
}

// Called when the game starts or when spawned
void AEcho::BeginPlay()
{
	//Super 부모클래스를 의미한다.
	Super::BeginPlay();

}

// Called every frame
void AEcho::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEcho::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEcho::Attack()
{
	//0번이 재생안되고 있으면 재생
	if (!mAnimInstance->Montage_IsPlaying(mAttackMontageArray[0]) &&
		!mAnimInstance->GetAttack())
	{
		mAnimInstance->Montage_Play(mAttackMontageArray[0]);

		mAnimInstance->SetAttack(true);

	}

}