// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h"
#include "PlayerAnim.h"

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


		// CharacterMovement 설정 개선
	GetCharacterMovement()->AirControl = 0.4f;              // 공중 제어력
	GetCharacterMovement()->GravityScale = 1.75f;           // 중력 (높을수록 빠르게 떨어짐)
	GetCharacterMovement()->JumpZVelocity = 600.f;          // 점프 높이
	GetCharacterMovement()->BrakingDecelerationFalling = 0.f; // 낙하 중 감속 (0 = 공중에서 미끄러지지 않음)

	//inputsystem
	static ConstructorHelpers::FObjectFinder<class UInputAction> MoveAsset = (TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));

	if (MoveAsset.Succeeded())
	{
		mMoveAction = MoveAsset.Object;
	}

	//jump
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpAsset = (TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	
	if (JumpAsset.Succeeded())
	{
		mJumpAction = JumpAsset.Object;
	}

	//attack
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackAsset = (TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'"));

	if (AttackAsset.Succeeded())
	{
		mAttackAction = AttackAsset.Object;
	}


	static ConstructorHelpers::FObjectFinder<class UInputMappingContext> IMC = (TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_RPG.IMC_RPG'"));

	if (IMC.Succeeded())
	{
		mMappingContext = IMC.Object;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsytem->AddMappingContext(mMappingContext, 0);
		}
	}


	mAnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	
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

	
	//PlayerInputComponent->BindAxis(TEXT("MoveKey"), this, & APlayerCharacter::MoveFront);
	//PlayerInputComponent->Bin  dAxis(TEXT("MoveKey"), this, &APlayerCharacter::MoveBack);

	PlayerInputComponent->BindAxis(TEXT("RotationKey"), this, &APlayerCharacter::Rotation);
	PlayerInputComponent->BindAxis(TEXT("ZoomKey"), this, &APlayerCharacter::CameraZoom);
	PlayerInputComponent->BindAxis(TEXT("LoopUpKey"), this, &APlayerCharacter::CameraLoopUp);

	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	enhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EnhancedInputMove);
	enhancedInputComponent->BindAction(mJumpAction, ETriggerEvent::Started, this, &APlayerCharacter::EnhancedInputJump);
	enhancedInputComponent->BindAction(mJumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::EnhancedInputJump);
	enhancedInputComponent->BindAction(mAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::EnhancedInputAttack);
	
}

//void APlayerCharacter::MoveFront(float scale)
//{	
//	//AddMovementInput(GetActorForwardVector(), scale, false);
//	
//}
//
//void APlayerCharacter::MoveBack(float scale)
//{
//	//AddMovementInput(GetActorForwardVector(), scale, false);
//}

void APlayerCharacter::Rotation(float scale)
{
	AddControllerYawInput(scale);
}

void APlayerCharacter::CameraZoom(float scale)
{
	mArm->TargetArmLength -= scale * 20.f;

	//if (mArm->TargetArmLength < 30.f)
	//{
	//	mArm->TargetArmLength = 30.f;
	//}
	//else if (mArm->TargetArmLength > 500.f)
	//{
	//	mArm->TargetArmLength = 500.f;
	//}

	mArm->TargetArmLength =FMath::Clamp(mArm->TargetArmLength, 30.f, 500.f);
	
		
}

//zoom보간
//void APlayerCharacter::CameraZoom(float scale)
//{
//	// 1. 목표치 계산: 마우스 입력을 받아 가고자 하는 목적지를 갱신
//	// scale이 0이 아닐 때만 목표치를 변경합니다.
//	TargetZoomLength -= scale * 50.f;
//
//	// 2. 목표치 제한 (Clamp): 30 ~ 500 사이로 유지
//	TargetZoomLength = FMath::Clamp(TargetZoomLength, 30.f, 500.f);
//
//	// 3. 현재 거리에서 목표 거리까지 부드럽게 보간 (FInterpTo)
//	// InterpSpeed(여기서는 10.f)가 높을수록 줌이 빠르고, 낮을수록 천천히 움직입니다.
//	mArm->TargetArmLength = FMath::FInterpTo(
//		mArm->TargetArmLength,
//		TargetZoomLength,
//		GetWorld()->GetDeltaSeconds(),
//		10.f
//	);
//}
// 
//LoopUp보간
void APlayerCharacter::CameraLoopUp(float scale)
{
	// 1. 현재 회전값 가져오기
	FRotator CurrentRot = mArm->GetRelativeRotation();

	// 2. 목표 각도 계산 (가고자 하는 최종 목적지)
	// 보정치를 주어 더 큰 범위로 움직이게 하려면 10.f 대신 더 높은 감도값을 사용하세요.
	float TargetPitch = CurrentRot.Pitch + (scale * 20.f);

	// 3. 각도 제한 (Clamp)
	TargetPitch = FMath::Clamp(TargetPitch, -45.f, -15.f);

	// 4. 부드러운 보간 (FInterpTo) 사용
	// 현재 각도에서 목표 각도까지 DeltaSeconds와 속도(InterpSpeed)를 이용해 부드럽게 이동합니다.
	FRotator NewRot = CurrentRot;
	NewRot.Pitch = FMath::FInterpTo(CurrentRot.Pitch, TargetPitch, GetWorld()->GetDeltaSeconds(), 10.f);

	// 5. 최종 회전값 적용
	mArm->SetRelativeRotation(NewRot);
}


//void APlayerCharacter::CameraLoopUp(float scale)
//{
//	FRotator Rot = mArm->GetRelativeRotation();
//
//	Rot.Pitch += scale * 10.f * GetWorld()->GetDeltaSeconds();
//
//	if (Rot.Pitch > -15.f)
//	{
//		Rot.Pitch = -15.f;
//	}
//	else if (Rot.Pitch < -45.f)
//	{
//		Rot.Pitch = -45.f;
//	}
//
//	mArm->SetRelativeRotation(Rot);
//}

//EnhancedInput
void APlayerCharacter::EnhancedInputMove(const FInputActionValue& value)
{
	
	FVector2D MoveVector = value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("EnhancedInputMove"));

	//if (Controller != nullptr)
	//{
	//	// 컨트롤러 회전 방향을 기준으로 앞방향 계산
	//	const FRotator Rotation = Controller->GetControlRotation();
	//	const FRotator YawRotation(0, Rotation.Yaw, 0);

	//	// 앞/뒤 이동
	//	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//	AddMovementInput(ForwardDirection, MoveVector.Y);

	//	// 좌/우 이동
	//	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//	AddMovementInput(RightDirection, MoveVector.X);
	//}

	//또다른방법
	if (Controller != nullptr)
	{
		const FRotator MoveRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		
		if (MoveVector.X != 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveVectorX"));
			const FVector directionVector = MoveRotation.RotateVector(FVector::RightVector);
			AddMovementInput(directionVector, MoveVector.X);

			
		}

		if (MoveVector.Y != 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveVectorY"));
			const FVector directionVector = MoveRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(directionVector, MoveVector.Y);

		}
	}

	//또 다른 이동방법 카메라자체가 이동됨
	/*FVector2D MovementVector = value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();

	if (MovementVectorSizeSquared > 1.0f) {
		MovementVectorSizeSquared = 1.0f;
		MovementVector.Normalize();
	}

	FVector MoveDirection = FVector{ MovementVector.X, MovementVector.Y, 0.0f };
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSizeSquared);*/

}


void APlayerCharacter::EnhancedInputJump(const FInputActionInstance& instance)
{
	//ETriggerEvent CurrentEvent = instance.GetTriggerEvent();

	//UCharacterMovementComponent* Movement = GetCharacterMovement()

	
			Jump(); // 시작 이벤트일 때만 점프 실행
			//mAnimInstance->ChangeAnimType(EPlayerAnimType::Jump);
		
	
	
}

void APlayerCharacter::EnhancedInputAttack(const FInputActionInstance& key)
{
	ETriggerEvent CurrentEvent = key.GetTriggerEvent();
	if (CurrentEvent == ETriggerEvent::Started)
	{
		Attack();
	}
}

void APlayerCharacter::Attack()
{

}

