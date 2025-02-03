#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Num7Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;
struct FInputActionValue;
class UFloatingPawnMovement;

UCLASS()
class NUM7_API ANum7Character : public APawn
{
	GENERATED_BODY()

public:

	ANum7Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;

	UFloatingPawnMovement* MovementComp;

	float BaseSpeed;
	float SprintSpeed;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	void Look(const FInputActionValue& value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& value);

	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
};
