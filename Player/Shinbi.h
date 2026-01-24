// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "Shinbi.generated.h"

/**
 * 
 */
UCLASS()
class UE5_RPG_API AShinbi : public APlayerCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShinbi();
protected:
	UPROPERTY(EditAnywhere, category = "Shinbi", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> mAttackMontageArray;

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
