#include "Num7Character.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Num7PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ANum7Character::ANum7Character()
{
    // 1. Tick 설정
    PrimaryActorTick.bCanEverTick = true;

    // 2. CapsuleComponent 설정 (루트 컴포넌트)
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComp;

    // Physics 설정: Simulate Physics 비활성화
    CapsuleComp->SetSimulatePhysics(false);

    // 캡슐 크기 설정
    CapsuleComp->SetCapsuleRadius(42.0f);
    CapsuleComp->SetCapsuleHalfHeight(96.0f);

    // 3. SkeletalMeshComponent 설정
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComp->SetupAttachment(CapsuleComp);

    // 4. SpringArmComponent 설정
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComp->SetupAttachment(CapsuleComp);
    SpringArmComp->TargetArmLength = 300.0f; // 카메라와 캐릭터 간의 거리
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComp->SetupAttachment(SpringArmComp);
    CameraComp->bUsePawnControlRotation = false;

    // MovementComponent 설정
    MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

    // 이동 속도 초기화
    BaseSpeed = 400.0f;   // 기본 이동 속도
    SprintSpeed = 800.0f; // 달리기 속도
}

void ANum7Character::BeginPlay()
{
    Super::BeginPlay();
}

void ANum7Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ANum7Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ANum7PlayerController* PlayerController = Cast<ANum7PlayerController>(GetController()))
        {
            // 이동
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANum7Character::Move
                );
            }
            // 회전 (마우스)
            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANum7Character::Look
                );
            }
            // 달리기
            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANum7Character::StartSprint
                );
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &ANum7Character::StopSprint
                );
            }
        }
    }
}

void ANum7Character::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();
    float CurrentSpeed = BaseSpeed; // 기본 이동 속도 사용

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddActorLocalOffset(FVector(MoveInput.X * CurrentSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f), true);
    }
    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddActorLocalOffset(FVector(0.0f, MoveInput.Y * CurrentSpeed * GetWorld()->GetDeltaSeconds(), 0.0f), true);
    }
}

void ANum7Character::Look(const FInputActionValue& value)
{
    const FVector2D LookInput = value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X); // 마우스로 좌우 회전
    AddControllerPitchInput(LookInput.Y); // 마우스로 위아래 회전
}

void ANum7Character::StartSprint(const FInputActionValue& value)
{
    // 달리기 속도 증가
    BaseSpeed = SprintSpeed;
}

void ANum7Character::StopSprint(const FInputActionValue& value)
{
    // 기본 속도로 복원
    BaseSpeed = 400.0f;
}
