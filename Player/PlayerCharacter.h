// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class UE5_RPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))	//private를 editor에서 사용할수 잇게됨
	UCameraComponent* mCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))	//private를 editor에서 사용할수 잇게됨
	USpringArmComponent* mArm;

private:
	float TargetZoomLength = 500.f; // 목표 줌 거리 (초기값 설정)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MoveFront(float scale);
	void MoveBack(float scale);
	void Rotation(float scale);
	void CameraZoom(float scale);
	void CameraLoopUp(float scale);
};
