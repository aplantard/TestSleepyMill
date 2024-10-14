// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TetPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PaperGroupedSpriteComponent.h"
#include "Components/TetPaperGroupedSpriteComponent.h"
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
		m_currentControlledBlock = m_gameGrid->SpawnPiece();
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
			enhancedInputComponent->BindAction(m_rotateInputAction.LoadSynchronous(), ETriggerEvent::Started, this, &ATetPlayerController::RotatePiece);
		}

		if (m_moveBlockInputAction)
		{
			enhancedInputComponent->BindAction(m_moveBlockInputAction.LoadSynchronous(), ETriggerEvent::Started, this, &ATetPlayerController::MovePiece);
		}

		if (m_moveBlockDownInputAction)
		{
			enhancedInputComponent->BindAction(m_moveBlockDownInputAction.LoadSynchronous(), ETriggerEvent::Started, this, &ATetPlayerController::MovePieceDown);
		}
	}
}

void ATetPlayerController::RotatePiece(const FInputActionValue& a_value)
{
	float rotationSide = a_value.Get<float>();
	rotationSide = rotationSide / FMath::Abs(rotationSide);

	if (m_currentControlledBlock)
	{
		m_currentControlledBlock->AddActorLocalRotation(FRotator(0, rotationSide * 90,0).Quaternion());
	}

	FVector deltaVectorToMove = FVector::Zero();
	int blockSize = m_gameGrid->GetBlockSize();

	FTransform gridFirstBlockTranform;
	m_gameGrid->m_paperGroupedSpriteComponent->GetInstanceTransform(0, gridFirstBlockTranform, true);
	FVector gridFirstBlockLocation = gridFirstBlockTranform.GetTranslation();
	FVector gridLastBlockLocation = gridFirstBlockLocation + FVector(m_gameGrid->m_numCol * blockSize, 0,0);

	gridFirstBlockLocation += FVector(blockSize, 0, 0);

	UPaperGroupedSpriteComponent* renderComponent = m_currentControlledBlock->GetRenderComponent();
	for (int i = 0; i < renderComponent->GetInstanceCount(); ++i)
	{
		FTransform currentBlockTranform;
		renderComponent->GetInstanceTransform(i, currentBlockTranform, true);
		FVector currentBlockLocation = currentBlockTranform.GetTranslation();

		FVector deltaVectorFirstBlock = currentBlockLocation - gridFirstBlockLocation;
		FVector deltaVectorLastBlock = currentBlockLocation - gridLastBlockLocation;

		if (deltaVectorFirstBlock.X < 0)
		{
			deltaVectorFirstBlock *= FVector(-1, 0, 0);
			if (deltaVectorFirstBlock.SizeSquared() > deltaVectorToMove.SizeSquared())
			{
				deltaVectorToMove = deltaVectorFirstBlock;
			}
		}
		else if (deltaVectorLastBlock.X >= 0)
		{
			deltaVectorLastBlock *= FVector(-1, 0, 0);
			if (deltaVectorLastBlock.SizeSquared() > deltaVectorToMove.SizeSquared())
			{
				deltaVectorToMove = deltaVectorLastBlock;
			}
		}
	}

	if (deltaVectorToMove != FVector::Zero())
	{
		m_currentControlledBlock->AddActorWorldOffset(deltaVectorToMove);
		m_currentControlledBlock->GetRenderComponent()->OnUpdateTransform(EUpdateTransformFlags::PropagateFromParent, ETeleportType::TeleportPhysics);
	}
}

void ATetPlayerController::MovePiece(const FInputActionValue& a_value)
{
	FVector MovementVector = a_value.Get<FVector>();
	MovementVector.Normalize();

	if (m_currentControlledBlock)
	{
		int blockSize = m_gameGrid->GetBlockSize();
		if (CanMovePiece(m_currentControlledBlock.Get(), MovementVector * blockSize))
		{
			m_currentControlledBlock->AddActorWorldOffset(MovementVector * blockSize);
		}
	}
}

bool ATetPlayerController::CanMovePiece(ABaseBlock* a_block, FVector a_vectorToAdd)
{
	int blockSize = m_gameGrid->GetBlockSize();

	FTransform gridFirstBlockTranform;
	m_gameGrid->m_paperGroupedSpriteComponent->GetInstanceTransform(0, gridFirstBlockTranform, true);
	FVector gridFirstBlockLocation = gridFirstBlockTranform.GetTranslation();

	UPaperGroupedSpriteComponent* renderComponent = a_block->GetRenderComponent();
	for (int i = 0; i < renderComponent->GetInstanceCount(); ++i)
	{
		FTransform currentBlockTranform;
		renderComponent->GetInstanceTransform(i, currentBlockTranform, true);
		FVector currentBlockLocation = currentBlockTranform.GetTranslation();

		FVector currentInstancePosInGrid = currentBlockLocation - gridFirstBlockLocation;

		int row = FMath::RoundToInt(currentInstancePosInGrid.Y) / blockSize;
		int col = (FMath::RoundToInt(currentInstancePosInGrid.X) / blockSize) - 1;

		if (!m_gameGrid->CanMovePiece(row, col, a_vectorToAdd))
		{
			return false;
		}
	}

	return true;
}

void ATetPlayerController::MovePieceDown(const FInputActionValue& a_value)
{
	FVector MovementVector = FVector(0, 1,0);

	if (m_currentControlledBlock)
	{
		int blockSize = m_gameGrid->GetBlockSize();
		FVector newPosition = m_currentControlledBlock->GetActorLocation() + (MovementVector * blockSize);
		while (CanMovePiece(m_currentControlledBlock.Get(), newPosition - m_currentControlledBlock->GetActorLocation()))
		{
			m_currentControlledBlock->AddActorWorldOffset(newPosition - m_currentControlledBlock->GetActorLocation());
			newPosition = m_currentControlledBlock->GetActorLocation() + (MovementVector * blockSize);
		}

		FTransform gridFirstBlockTranform;
		m_gameGrid->m_paperGroupedSpriteComponent->GetInstanceTransform(0, gridFirstBlockTranform, true);
		FVector gridFirstBlockLocation = gridFirstBlockTranform.GetTranslation();

		UPaperGroupedSpriteComponent* renderComponent = m_currentControlledBlock->GetRenderComponent();
		for (int i = 0; i < renderComponent->GetInstanceCount(); ++i)
		{
			FTransform currentBlockTranform;
			renderComponent->GetInstanceTransform(i, currentBlockTranform, true);
			FVector currentBlockLocation = currentBlockTranform.GetTranslation();

			FVector currentInstancePosInGrid = currentBlockLocation - gridFirstBlockLocation;

			int row = FMath::RoundToInt(currentInstancePosInGrid.Y / blockSize);
			int col = FMath::RoundToInt(currentInstancePosInGrid.X / blockSize) -1;

			FGridCellData& currentGridCellData = m_gameGrid->m_gridData[row][col];

			currentGridCellData.m_pieceOnCell = m_currentControlledBlock.Get();
			currentGridCellData.m_instanceIdx = i;
		}

		m_currentControlledBlock = m_gameGrid->SpawnPiece();
	}
}
