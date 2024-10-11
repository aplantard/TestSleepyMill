// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TetPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "TetGrid.h"
#include "BaseBlock.h"

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

	if (m_currentControlledBlock == nullptr)
	{
		m_currentControlledBlock = m_gameGrid->SpawnBlock();
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
			enhancedInputComponent->BindAction(m_moveBlockInputAction.LoadSynchronous(), ETriggerEvent::Started, this, &ATetPlayerController::MovePiece);
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
	FVector MovementVector = a_value.Get<FVector>();
	MovementVector.Normalize();

	if (m_currentControlledBlock)
	{
		int blockSize = m_gameGrid->GetBlockSize();
		FVector newPosition = m_currentControlledBlock->GetActorLocation() + (MovementVector * blockSize);
		if (CanMovePiece(m_currentControlledBlock.Get(), newPosition - m_currentControlledBlock->GetActorLocation()))
		{
			m_currentControlledBlock->AddActorWorldOffset(newPosition - m_currentControlledBlock->GetActorLocation());
		}
	}
}

bool ATetPlayerController::CanMovePiece(ABaseBlock* a_block, FVector a_vectorToAdd)
{
	int blockSize = m_gameGrid->GetBlockSize();
	FVector gridLocation = m_gameGrid->GetActorLocation();
	FVector gridWorldLocation = gridLocation + (FVector(1, 0, 0) * blockSize);
	FVector endgridWorldLocation = gridLocation + ((m_gameGrid->m_numCol + 1) * blockSize);
	FVector newBlockLocation = a_block->GetActorLocation() + a_vectorToAdd;

	return (newBlockLocation - gridWorldLocation).X > 0 && (newBlockLocation - endgridWorldLocation).X < 0;
}

void ATetPlayerController::MovePieceDown(const FInputActionValue& a_value)
{
}
