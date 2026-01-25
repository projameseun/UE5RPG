// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"

UPlayerAnim::UPlayerAnim()
{
	mDirection = 0.f;
	mSpeed = 0.f;
	mAttack = false;
	mOnGround = true;
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
			mSpeed = FVector(velocity.X, velocity.Y, 0.f).Size();

			// 현재 땅을 밟고 있는지 확인
			bool bCurrentlyOnGround = movement->IsMovingOnGround();

			// 공격 중이 아닐 때만 이동/점프 애니메이션 처리
			if (mAnimType != EPlayerAnimType::Attack)
			{
				// 땅에 착지했을 때
				if (bCurrentlyOnGround)
				{
					// Jump 상태에서 착지하면 Ground로 전환
					if (mAnimType == EPlayerAnimType::Jump)
					{
						mAnimType = EPlayerAnimType::Ground;
					}
					// 평소에는 Ground 상태 유지
					else if (mAnimType != EPlayerAnimType::Jump)
					{
						mAnimType = EPlayerAnimType::Ground;
					}
				}
				// 공중에 있을 때 (점프 중)
				// PlayerCharacter에서 Jump 상태로 설정하므로 여기서는 유지만 함
			}

			// 이전 상태 저장
			mOnGround = bCurrentlyOnGround;
		}
	}
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
	mAttack = false;
}
void UPlayerAnim::AnimNotify_JumpEnd()
{
	mAnimType = EPlayerAnimType::Ground;
}
