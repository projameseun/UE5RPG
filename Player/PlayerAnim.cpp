// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"

UPlayerAnim::UPlayerAnim()
{
	mDirection = 0.f;
	mSpeed = 0.f;
	mAttack = false;
	mFalling = true;
	mGround = false;
	mVelocityZ = 0.f;
	mAnimType = EPlayerAnimType::Ground;

}

void UPlayerAnim::NativeInitializeAnimation()
{

}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (player)
	{
		UCharacterMovementComponent* movement = player->GetCharacterMovement();

		if (movement)
		{
			// 수평 속도 계산
			FVector velocity = movement->Velocity;
			// 수직 속도 계산
			FVector zVelocity = movement->Velocity;
			mSpeed = FVector(velocity.X, velocity.Y, 0.f).Size();
			mVelocityZ = FVector(0.f, 0.f, velocity.Z).Size();
			if (movement->IsFalling())
			{
				mFalling = true;
				mGround = false;
				UE_LOG(LogTemp, Warning, TEXT("Falling"));
			}
			else
			{		
					mGround = true;
					mFalling = false;
					UE_LOG(LogTemp, Warning, TEXT("Ground"));
				
			}
			
		}
	}
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
	mAttack = false;
}
void UPlayerAnim::AnimNotify_JumpEnd()
{
	//mAnimType = EPlayerAnimType::Ground;
}
