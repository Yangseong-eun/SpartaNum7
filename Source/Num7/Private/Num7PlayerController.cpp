// Fill out your copyright notice in the Description page of Project Settings.


#include "Num7PlayerController.h"
#include "EnhancedInputSubsystems.h"

ANum7PlayerController::ANum7PlayerController()
	: InputMappingContext(nullptr), 
	MoveAction(nullptr), 
	JumpAction(nullptr), 
	LookAction(nullptr), 
	SprintAction(nullptr)
{
}

void ANum7PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext) {
				Subsystem->AddMappingContext(InputMappingContext,0);

			}
		}
	}
}

