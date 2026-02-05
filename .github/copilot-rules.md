# GitHub Copilot 사용 지침 - UE5_RPG 프로젝트

## 프로젝트 환경
- **운영체제**: Windows
- **엔진 버전**: Unreal Engine 5.5.4
- **프로그래밍 언어**: C++
- **네트워크 구조**: 소켓 프로그래밍 (커스텀 구현)
- **게임 장르**: RPG, FPS
- **서버 타입**: 
  - Dedicated Server (데디케이티드 서버)
  - Listen Server (리슨 서버)

---

## 핵심 규칙

### 1. 언어 사용 규칙
- **주석**: 한국어 사용 가능하며 권장됨
  ```cpp
  // 속도 계산
  // 수평 속도 계산
  mSpeed = FVector(velocity.X, velocity.Y, 0.0f).Size();
  ```
- **변수명**: 반드시 영어로 작성
- **함수명**: 반드시 영어로 작성
- **클래스명**: 반드시 영어로 작성
- 한국어 주석은 이해를 돕지만, 모든 식별자는 호환성을 위해 영어로 유지

### 2. 네트워크 프로그래밍
- 클라이언트-서버 통신을 위한 커스텀 소켓 프로그래밍 구현
- Dedicated Server와 Listen Server 아키텍처 모두 지원
- 모든 게임플레이 코드에서 네트워크 복제 및 동기화 고려
- 클라이언트-서버 권한을 적절히 처리

### 3. 멀티 장르 지원
- RPG와 FPS 메커닉을 모두 지원할 수 있는 유연한 코드 작성
- 장르별 로직은 적절한 모듈로 분리
- 장르 간 공유 동작은 인터페이스 사용

---

## 네이밍 규칙

### 변수
- **멤버 변수**: `m` 접두사 + PascalCase
  ```cpp
  float mSpeed;
  bool mIsFalling;
  FVector mVelocity;
  int32 mAttack;
  ```
- **지역 변수**: camelCase (접두사 없음)
  ```cpp
  float deltaTime;
  APlayerCharacter* player;
  UCharacterMovementComponent* movement;
  ```
- **상수**: `k` 접두사 + PascalCase 또는 ALL_CAPS
  ```cpp
  const float kMaxSpeed = 600.0f;
  const int32 MAX_PLAYERS = 4;
  ```
- **불리언 변수**: `Is`, `Has`, `Can` 접두사 또는 `m` + 설명적 이름
  ```cpp
  bool mIsFalling;
  bool mGround;
  bool mAttack;
  ```

### 클래스 및 구조체
- **UObject 파생 클래스**: `U` 접두사
  ```cpp
  UPlayerAnim
  UInventoryComponent
  ```
- **AActor 파생 클래스**: `A` 접두사
  ```cpp
  APlayerCharacter
  AWeaponBase
  ```
- **구조체**: `F` 접두사
  ```cpp
  FPlayerStats
  FNetworkPacket
  ```
- **열거형**: `E` 접두사
  ```cpp
  EPlayerAnimType
  EWeaponType
```
- **인터페이스**: `I` 접두사
  ```cpp
  IInteractable
  IDamageable
  ```

### 함수
- **Public 함수**: PascalCase
  ```cpp
  void UpdatePlayerState();
  float GetCurrentHealth();
  void NativeUpdateAnimation(float DeltaSeconds);
  ```
- **Private 함수**: PascalCase (public과 동일)
  ```cpp
  void CalculateMovement();
  bool ValidateNetworkPacket();
  ```
- **애니메이션 노티파이**: `AnimNotify_` 접두사 + PascalCase
  ```cpp
  void AnimNotify_AttackEnd();
  void AnimNotify_JumpEnd();
  void AnimNotify_FootStep();
  ```
- **네트워크 RPC**: Unreal 규칙 따름
  ```cpp
  UFUNCTION(Server, Reliable)
  void Server_SendAttack();
  
  UFUNCTION(Client, Reliable)
  void Client_ReceiveDamage(float Damage);
  ```

### 파일 및 폴더
- **파일명**: 클래스명과 정확히 일치
  ```
  PlayerCharacter.h / PlayerCharacter.cpp
  PlayerAnim.h / PlayerAnim.cpp
  WeaponBase.h / WeaponBase.cpp
  ```
- **폴더 구조**: 기능/시스템별로 구성
  ```
  Source/UE5_RPG/
  ├── Player/
  │   ├── PlayerCharacter.h
  │   ├── PlayerCharacter.cpp
  │   ├── PlayerAnim.h
  │   └── PlayerAnim.cpp
  ├── Weapon/
  ├── Network/
  ├── AI/
  └── UI/
  ```

---

## 코드 구조 표준

### Include 순서
1. 자신의 헤더 파일 (가장 먼저)
2. 프로젝트 헤더
3. 엔진 헤더
4. 서드파티 헤더
5. C++ 표준 라이브러리

```cpp
#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include <vector>
```

### 헤더 가드
- 항상 `#pragma once` 사용
```cpp
#pragma once

#include "CoreMinimal.h"
// ... 헤더 나머지
```

### 클래스 구조 순서
1. GENERATED_BODY() 매크로
2. Public 멤버
3. Protected 멤버
4. Private 멤버

각 섹션 내부:
1. 생성자/소멸자
2. Override 함수
3. 인터페이스 구현
4. Public/Protected/Private 함수
5. 멤버 변수 (UPROPERTY 포함)

```cpp
class UE5_RPG_API UMyClass : public UObject
{
    GENERATED_BODY()

public:
    // 생성자
    UMyClass();

    // Override 함수
    virtual void BeginPlay() override;

    // Public 함수
    void PublicFunction();

protected:
    // Protected 함수
    void ProtectedFunction();

    // Protected 변수
    UPROPERTY()
    float mProtectedValue;

private:
    // Private 함수
    void PrivateFunction();

    // Private 변수
    float mPrivateValue;
};
```

---

## 네트워크 프로그래밍 규칙

### 서버 권한
- 서버가 게임플레이 로직에 대한 권한을 가짐
- 클라이언트는 입력을 전송하고, 서버가 검증 후 응답
- RPC 사용 규칙:
  - `Server_` 접두사: 클라이언트→서버 호출
  - `Client_` 접두사: 서버→클라이언트 호출
  - `Multicast_` 접두사: 서버→모든 클라이언트 호출

### 소켓 프로그래밍 가이드라인
- 스레드 안전한 소켓 작업 구현
- 가능한 경우 논블로킹 소켓 사용
- 연결 끊김을 우아하게 처리
- 적절한 패킷 직렬화/역직렬화 구현
- 모든 네트워크 작업에 에러 핸들링 추가

```cpp
// RPC 패턴 예제
UFUNCTION(Server, Reliable, WithValidation)
void Server_RequestAttack(FVector Location);
bool Server_RequestAttack_Validate(FVector Location);
void Server_RequestAttack_Implementation(FVector Location);
```

### 리플리케이션
- 복제할 변수는 `Replicated` 또는 `ReplicatedUsing`으로 표시
- `GetLifetimeReplicatedProps` 오버라이드 필요
- 대역폭을 고려하여 데이터 복제

```cpp
UPROPERTY(Replicated)
float mHealth;

UPROPERTY(ReplicatedUsing = OnRep_Ammo)
int32 mAmmo;

UFUNCTION()
void OnRep_Ammo();
```

---

## 베스트 프랙티스

### Null 안전성
- 포인터 역참조 전에 항상 체크
- UObject 포인터는 `IsValid()` 사용,
- 잘못된 상태는 early return 사용
-더좋은게 있으면 변경해서 사용,
```cpp
APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());
if (!IsValid(player))
{
    return;
}

UCharacterMovementComponent* movement = player->GetCharacterMovement();
if (movement)
{
    // 안전하게 사용
}
```

### 성능
- 자주 접근하는 컴포넌트는 캐싱
- Tick/Update 함수에서 무거운 연산 피하기
- 자주 스폰되는 액터는 오브젝트 풀링 사용
- 네트워크 대역폭 사용량 프로파일링

### 메모리 관리
- 비UObject 클래스는 스마트 포인터 사용: `TSharedPtr`, `TUniquePtr`
- UObject는 수동으로 delete하지 말 것 (가비지 컬렉션)
- 순환 참조 주의

### 에러 핸들링
- `check()`: 크리티컬 에러 (개발 중 크래시)
- `ensure()`: 비크리티컬 단언 (로그 출력, 계속 실행)
- 디버깅용 `UE_LOG` 추가

```cpp
check(IsValid(Player)); // 크리티컬: null이면 크래시
ensure(Health > 0.0f);   // 경고: 로그 출력 후 계속

UE_LOG(LogTemp, Warning, TEXT("Player health low: %f"), mHealth);
```

---

## 애니메이션 시스템

### Animation Blueprint (UAnimInstance)
- `UAnimInstance`에서 파생
- `NativeInitializeAnimation()`: 초기화용 오버라이드
- `NativeUpdateAnimation(float DeltaSeconds)`: 업데이트용 오버라이드
- 업데이트 로직은 가볍게 유지 (매 프레임 실행됨)

```cpp
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (!player) return;

  // 애니메이션 변수 업데이트
UCharacterMovementComponent* movement = player->GetCharacterMovement();
  if (movement)
    {
   FVector velocity = movement->Velocity;
     
        // 수평 속도 계산
  mSpeed = FVector(velocity.X, velocity.Y, 0.0f).Size();
        
     // 수직 속도 계산 (부호 유지 - 양수: 상승, 음수: 하강)
        mVelocityZ = velocity.Z;
        
        // 상태 업데이트
        mIsFalling = movement->IsFalling();
     mGround = !mIsFalling;
    }
}
```

### Animation Notifies
- 애니메이션이 트리거하는 게임플레이 이벤트용
- Notify 함수는 가볍게 유지
- 필요시 네트워크 복제 고려

```cpp
void UPlayerAnim::AnimNotify_AttackEnd()
{
    mAttack = false;
    
    if (APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner()))
    {
    player->OnAttackFinished();
    }
}

void UPlayerAnim::AnimNotify_JumpEnd()
{
    // 점프 종료 처리
}
```

---

## 서버 아키텍처 가이드라인

### Dedicated Server (데디케이티드 서버)
- 렌더링 없이 실행 (헤드리스)
- 모든 게임플레이에 대한 권한 보유
- 모든 클라이언트 관리
- 영속성 및 데이터베이스 작업 처리

### Listen Server (리슨 서버)
- 호스트 플레이어 + 서버
- 호스트는 약간의 권한 이점 (로컬 처리)
- P2P 또는 소규모 게임에 적합
- 공정성 문제 고려 (호스트 이점)

### 코드 고려사항
```cpp
// 서버에서 실행 중인지 체크
if (HasAuthority())
{
    // 서버 전용 코드
}

// 클라이언트에서 실행 중인지 체크
if (GetLocalRole() == ROLE_AutonomousProxy)
{
    // 클라이언트 소유 캐릭터 코드
}

// 데디케이티드 서버인지 체크
if (IsRunningDedicatedServer())
{
    // 데디케이티드 서버 전용 코드 (렌더링 없음)
}
```

---

## 자주 사용하는 패턴

### 컴포넌트 안전하게 가져오기
```cpp
APlayerCharacter* player = Cast<APlayerCharacter>(TryGetPawnOwner());
if (!IsValid(player))
{
    return;
}

UCharacterMovementComponent* movement = player->GetCharacterMovement();
if (!IsValid(movement))
{
    return;
}

// 안전하게 컴포넌트 사용
```

### 벡터 연산
```cpp
// 수평 속도 (Z 무시)
float horizontalSpeed = FVector(velocity.X, velocity.Y, 0.0f).Size();

// 수직 속도 (부호 유지 - 양수: 상승, 음수: 하강)
float verticalSpeed = velocity.Z;

// 안전하게 정규화
FVector direction = velocity;
if (direction.Normalize())
{
    // direction이 유효함 (0이 아님)
}
```

### 상태 관리
```cpp
// 단순 상태: 불리언 사용
bool mIsFalling;
bool mIsAttacking;
bool mGround;

// 복잡한 상태: enum 사용
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle,
    Moving,
    Jumping,
    Attacking,
    Dead
};

EPlayerState mCurrentState;
```

---

## 문서화 요구사항

### 파일 헤더
```cpp
// Fill out your copyright notice in the Description page of Project Settings.
```

### 함수 주석 (한국어 허용)
```cpp
/**
 * 애니메이션 블렌딩을 위한 플레이어 이동 속도 계산
 * @param DeltaTime - 마지막 프레임 이후 시간
 * @return 정규화된 속도 값 (0.0 ~ 1.0)
 */
float CalculateMovementSpeed(float DeltaTime);
```

### 복잡한 로직 주석 (한국어 권장)
```cpp
// 애니메이션 블렌딩을 위한 수평 속도 계산
// 수직 이동이 달리기 애니메이션에 영향을 주지 않도록 Z 속도를 무시
mSpeed = FVector(velocity.X, velocity.Y, 0.0f).Size();

// 수직 속도 계산 (부호 유지 - 양수: 상승, 음수: 하강)
mVelocityZ = velocity.Z;

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
```

---

## 하지 말아야 할 것

### ? 금지사항
- 변수명, 함수명, 클래스명에 한국어 사용
- 포인터 null 체크 무시
- Tick/Update 함수에서 무거운 계산 수행
- 헤더 간 순환 종속성 생성
- 네트워크 관련 속성 복제 누락
- UObject 저장에 raw 포인터 사용 (UPROPERTY 사용 필수)
- Unreal 코딩 표준 및 네이밍 규칙 무시
- 네트워크 주소나 포트 하드코딩
- 클라이언트 측 검증만으로 충분하다고 가정
- 서버와 클라이언트 권한 혼재

### ? 권장사항
- 변수명, 함수명, 클래스명은 영어로 작성
- 주석은 한국어 사용 가능 (이해를 돕기 위해 권장)
- 포인터 사용 전 항상 체크
- 자주 접근하는 컴포넌트 캐싱
- 헤더에서 전방 선언 사용
- 네트워크 함수는 적절한 RPC로 표시
- UObject 참조는 UPROPERTY 사용
- Unreal Engine 코딩 표준 준수
- 네트워크 설정을 구성 가능하게 만들기
- 서버에서 검증, 클라이언트는 신뢰하지 말 것
- 서버와 클라이언트 로직 명확히 분리

---

## 테스팅 가이드라인

### 네트워크 테스팅
- 시뮬레이션된 레이턴시 및 패킷 손실로 테스트
- Dedicated 및 Listen 서버 모드 모두 테스트
- 여러 클라이언트로 테스트
- 재연결 시나리오 테스트
- 대역폭 사용량 프로파일링

### 애니메이션 테스팅
- 모든 애니메이션 상태 전환 테스트
- 애니메이션 노티파이가 올바르게 발동하는지 확인
- 애니메이션 블렌딩 부드러움 테스트
- 다양한 프레임 레이트에서 테스트

---

## 추가 리소스
- [Unreal Engine 문서](https://docs.unrealengine.com/)
- [Unreal Engine 네트워킹 개요](https://docs.unrealengine.com/ko/InteractiveExperiences/Networking/)
- [애니메이션 블루프린트](https://docs.unrealengine.com/ko/AnimatingObjects/SkeletalMeshAnimation/AnimBlueprints/)
- [Unreal Engine 코딩 표준](https://docs.unrealengine.com/ko/ProductionPipelines/DevelopmentSetup/CodingStandard/)
