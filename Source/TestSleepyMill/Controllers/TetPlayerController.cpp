// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TetPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void ATetPlayerController::BeginPlay()
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->ClearAllMappings();

		if (!m_inputMapping.IsNull())
		{
			subsystem->AddMappingContext(m_inputMapping.LoadSynchronous(), 100);
		}
	}
}

void ATetPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		OverridePlayerInputClass = UEnhancedPlayerInput::StaticClass();

		if (m_rotateInputAction)
		{
			enhancedInputComponent->BindAction(m_rotateInputAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ATetPlayerController::RotatePiece);
		}

		if (m_moveBlockInputAction)
		{
			enhancedInputComponent->BindAction(m_moveBlockInputAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ATetPlayerController::MovePiece);
		}

		if (m_moveBlockDownInputAction)
		{
			enhancedInputComponent->BindAction(m_moveBlockDownInputAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ATetPlayerController::MovePieceDown);
		}
	}
}

void ATetPlayerController::RotatePiece(const FInputActionValue& a_value)
{
}

void ATetPlayerController::MovePiece(const FInputActionValue& a_value)
{
}

void ATetPlayerController::MovePieceDown(const FInputActionValue& a_value)
{
}
