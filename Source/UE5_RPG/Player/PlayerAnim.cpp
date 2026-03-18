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
			// 속도 계산
			FVector velocity = movement->Velocity;
			
			// 수평 속도 계산
			mSpeed = FVector(velocity.X, velocity.Y, 0.f).Size();
			
			// 수직 속도 계산 (부호 유지 - 양수: 상승, 음수: 하강)
			mVelocityZ = velocity.Z;
			//수정
			// 상태 업데이트
			if (movement->IsFalling())
			{
				mFalling = true;
				mGround = false;
			}
			else
			{		
				mGround = true;
				mFalling = false;
			}
		}
	}
}

void UPlayerAnim::AnimNotify_NomalAttack()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	if (player)
	{
		player->NomalAttack();
	}
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
	
	
	APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (player)
	{
		player->AttackEnd();
	}


}

void UPlayerAnim::AnimNotify_JumpEnd()
{
	//mAnimType = EPlayerAnimType::Ground;
}


void UPlayerAnim::AnimNotify_AttackCombo()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (player)
	{
		player->SetEnableAttack(true);
	}
}