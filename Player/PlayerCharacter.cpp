// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	//Arm의 부모 Component를 Mesh로 지정해서 Mesh를 따라다니게 한다.
	mArm->SetupAttachment(GetMesh());


	//Camera의 부모 Component로 Arm을 지정
	mCamera->SetupAttachment(mArm);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	PlayerInputComponent->BindAxis(TEXT("MoveKey"), this, & APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis(TEXT("MoveKey"), this, &APlayerCharacter::MoveBack);

}

void APlayerCharacter::MoveFront(float scale)
{	
	AddMovementInput(GetActorForwardVector(), scale, false);
}

void APlayerCharacter::MoveBack(float scale)
{
	AddMovementInput(GetActorForwardVector(), scale, false);
}