// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

//이 Enum문은 블루프린트로 사용할수 있다고 명시하는거 그래야 블루프린트에서 Enum을 사용할수 있다.
UENUM(BlueprintType)
enum class EPlayerAnimType : uint8
{
	Ground,
	Jump,
	Death,
	Attack
};

UCLASS()
class UE5_RPG_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool mAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool mOnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerAnimType mAnimType;
	

public:
	float GetDirection() const
	{ 
		return mDirection;
	}

	void SetDircton(float dir)
	{
		mDirection = dir;
	}

	void SetAttack(bool attack)
	{
		mAttack = attack;
	}

	bool GetAttack() const
	{
		return mAttack;
	}
	void ChangeAnimType(EPlayerAnimType type)
	{
		mAnimType = type;
	}
	float GetSpeed() const
	{
		return mSpeed;
	}

	EPlayerAnimType GetAnimType() const
	{
		return mAnimType;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	//노티파이 호출함수를 만들때는 반드시 규칙을 지켜주기
	//AnimNotify_ 를 한 뒤에 뒤에는 내가만든 Notify이름을 붙여줍니다.
	//UFUNCTION를 붙여줘야 멤버 변수로 사용할수 있습니다.
	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_JumpEnd();
};
