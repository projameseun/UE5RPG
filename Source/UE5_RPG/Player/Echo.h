// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "Echo.generated.h"

/**
 *
 */
UCLASS()
class UE5_RPG_API AEcho : public APlayerCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEcho();
protected:
	UPROPERTY(EditAnywhere, category = "EchoGroom", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UGroomComponent* HairComponent;

	UPROPERTY(EditAnywhere, category = "EchoMontage", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> mAttackMontageArray;

	UPROPERTY(EditAnywhere, category = "Weapon", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticWeapon;
		  
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Attack();


};
