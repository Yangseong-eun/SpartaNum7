#include "DronePawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ADronePawn::ADronePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // 드론 Static Mesh 설정
    DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
    RootComponent = DroneMesh;

    // SpringArm 설정
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(DroneMesh);
    SpringArmComp->TargetArmLength = 300.0f; // 드론과 카메라 사이 거리
    SpringArmComp->bUsePawnControlRotation = true; // 카메라 회전을 Pawn의 컨트롤에 따라 변경

    // 카메라 설정
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false; // 카메라 자체 회전 방지

    // 이동 및 회전 초기값 설정
    MoveSpeed = 400.0f;
    RotationSpeed = 100.0f;

    CurrentVelocity = FVector::ZeroVector;
    CurrentRotation = FRotator::ZeroRotator;
}

void ADronePawn::BeginPlay()
{
    Super::BeginPlay();

    // Input Mapping Context 추가
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (DroneMappingContext)
            {
                Subsystem->AddMappingContext(DroneMappingContext, 0);
                UE_LOG(LogTemp, Warning, TEXT("Mapping Context added!"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("DroneMappingContext is NULL!"));
            }
        }
    }
}


void ADronePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 마우스 입력 처리
    ProcessMouseInput(DeltaTime);

    // 이동 적용
    AddActorLocalOffset(CurrentVelocity * DeltaTime, true);
    AddActorLocalRotation(CurrentRotation * DeltaTime);

    // 컨트롤러 회전값 업데이트
    if (AController* MyController = GetController())
    {
        FRotator NewControlRotation = GetActorRotation();
        MyController->SetControlRotation(NewControlRotation);
    }

    // 이동 속도 초기화 (중복 이동 방지)
    CurrentVelocity = FVector::ZeroVector;
    CurrentRotation = FRotator::ZeroRotator;
}

void ADronePawn::ProcessMouseInput(float DeltaTime)
{
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        float MouseX, MouseY;
        PlayerController->GetInputMouseDelta(MouseX, MouseY);

        // 마우스 입력에 따른 Yaw 및 Pitch 업데이트
        AddControllerYawInput(MouseX * RotationSpeed * DeltaTime);
        AddControllerPitchInput(-MouseY * RotationSpeed * DeltaTime);
    }
}

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 입력 액션이 nullptr인지 확인하고 로그 추가
        if (!MoveRightAction)
        {
            UE_LOG(LogTemp, Error, TEXT("MoveRightAction is NULL!"));
        }

        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ADronePawn::MoveForward);
        EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ADronePawn::MoveRight);
        EnhancedInput->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &ADronePawn::MoveUp);
        EnhancedInput->BindAction(YawAction, ETriggerEvent::Triggered, this, &ADronePawn::Yaw);
        EnhancedInput->BindAction(PitchAction, ETriggerEvent::Triggered, this, &ADronePawn::Pitch);
        EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ADronePawn::Roll);
    }
}


void ADronePawn::MoveForward(const FInputActionValue& Value)
{
    float AxisValue = Value.Get<float>();
    CurrentVelocity += GetActorForwardVector() * AxisValue * MoveSpeed;
}

void ADronePawn::MoveRight(const FInputActionValue& Value)
{
    float AxisValue = Value.Get<float>();

    // 입력 값 확인 로그 추가
    UE_LOG(LogTemp, Warning, TEXT("MoveRight Axis Value: %f"), AxisValue);

    // 만약 입력 값이 0이라면 실행하지 않음
    if (FMath::IsNearlyZero(AxisValue)) return;

    // 현재 Yaw 회전만 고려하여 Right 방향 벡터를 계산
    FVector RightVector = FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetScaledAxis(EAxis::Y);

    // 입력 값을 속도에 반영
    CurrentVelocity += RightVector * AxisValue * MoveSpeed;
}



void ADronePawn::MoveUp(const FInputActionValue& Value)
{
    float AxisValue = Value.Get<float>();
    CurrentVelocity.Z = AxisValue * MoveSpeed; // 누적되지 않도록 변경
}

void ADronePawn::Yaw(const FInputActionValue& Value)
{
    float AxisValue = Value.Get<float>();
    CurrentRotation.Yaw = AxisValue * RotationSpeed;
}

void ADronePawn::Pitch(const FInputActionValue& Value)
{
    float AxisValue = Value.Get<float>();
    CurrentRotation.Pitch = AxisValue * RotationSpeed;
}

void ADronePawn::Roll(const FInputActionValue& Value)
{
    float AxisValue = Value.Get<float>();
    CurrentRotation.Roll = AxisValue * RotationSpeed;
}
