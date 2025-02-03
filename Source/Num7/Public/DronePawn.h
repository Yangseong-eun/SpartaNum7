// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class NUM7_API ADronePawn : public APawn
{
    GENERATED_BODY()

public:
    ADronePawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // 이동 속도와 회전 속도 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
    float MoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone Movement")
    float RotationSpeed;

    // 드론의 이동 함수
    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void MoveUp(const FInputActionValue& Value);

    // 드론의 회전 함수
    void Yaw(const FInputActionValue& Value);
    void Pitch(const FInputActionValue& Value);
    void Roll(const FInputActionValue& Value);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* DroneMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveForwardAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveRightAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveUpAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* YawAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* PitchAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* RollAction;

private:
    FVector CurrentVelocity;
    FRotator CurrentRotation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* DroneMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComp;

    void ProcessMouseInput(float DeltaTime);
};
