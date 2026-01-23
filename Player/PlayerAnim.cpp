// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"

UPlayerAnim::UPlayerAnim()
{
	mDirection = 0.f;
	mSpeed = 0.f;
}

void UPlayerAnim::NativeInitializeAnimation()
{

}
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (player)
	{
		UCharacterMovementComponent* movement = player->GetCharacterMovement();

		if (movement)
		{
			//속도를 구함
			mSpeed = movement->Velocity.Size();
		}

	}
} 