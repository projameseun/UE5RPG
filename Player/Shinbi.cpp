// Fill out your copyright notice in the Description page of Project Settings.


#include "Shinbi.h"
#include "PlayerAnim.h"

// Sets default values
AShinbi::AShinbi()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//생성자에서 에셋을 로딩하는방법 
	//Actor종류는 A가붙고 
	//Component,에셋들은 U가붙는다 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>  ShinbiAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'"));
	
	//GetMesh()함수는 Chracter클래스에 만들어져 있는 SkeltalMeshComponent를 얻어오는 함수이다.
	if (ShinbiAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(ShinbiAsset.Object);
	}

	//anim
	static ConstructorHelpers::FClassFinder<UAnimInstance> ShinbiAnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/BP_ShinbiAnim.BP_ShinbiAnim_C'"));

	if (ShinbiAnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ShinbiAnimAsset.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Player/MT_ShinbiAttack.MT_ShinbiAttack'"));

	if (AttackMontageAsset.Succeeded())
	{
		mAttackMontageArray.Add(AttackMontageAsset.Object);
	}
}

// Called when the game starts or when spawned
void AShinbi::BeginPlay()
{
	//Super 부모클래스를 의미한다.
	Super::BeginPlay();

}

// Called every frame
void AShinbi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShinbi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShinbi::Attack()
{
	//0번이 재생안되고 있으면 재생
	if (!mAnimInstance->Montage_IsPlaying(mAttackMontageArray[0])&&
		!mAnimInstance->GetAttack())
	{
		mAnimInstance->Montage_Play(mAttackMontageArray[0]);
		
		mAnimInstance->SetAttack(true);

	}

}