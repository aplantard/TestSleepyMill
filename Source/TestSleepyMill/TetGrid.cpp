// Fill out your copyright notice in the Description page of Project Settings.

#include "TetGrid.h"

#include "Components/TetPaperGroupedSpriteComponent.h"
#include "PaperSprite.h"
#include "BaseBlock.h"

ATetGrid::ATetGrid()
{
	m_paperGroupedSpriteComponent = CreateDefaultSubobject<UTetPaperGroupedSpriteComponent>(TEXT("PaperGroupedSpriteComponent"));
	m_paperGroupedSpriteComponent->TranslucencySortPriority = -5;
	RootComponent = m_paperGroupedSpriteComponent;

	m_spawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	m_spawnLocation->SetupAttachment(m_paperGroupedSpriteComponent);

}

ATetGrid::~ATetGrid()
{
}
	

void ATetGrid::OnConstruction(const FTransform& a_transform)
{
	Super::OnConstruction(a_transform);

	if (m_paperGroupedSpriteComponent && m_blockSprite && m_emptyBlockSprite)
	{
		m_gridData.Empty();

		//Fill Grid data with empty values
		for (int row = 0; row < m_numRow; row++)
		{
			m_gridData.Add(FGridRow());
			for (int col = 0; col < m_numCol; col++)
			{
				m_gridData[row].Add(FGridCellData());
			}
		}

		m_paperGroupedSpriteComponent->ClearInstances();

		FVector2D blockSize = m_blockSprite->GetSourceSize();
		FVector2D emptyBlockSize = m_emptyBlockSprite->GetSourceSize();

		FTransform actorTransform = m_paperGroupedSpriteComponent->GetRelativeTransform();
		// Here we are rotating the block to face top view
		actorTransform.SetRotation(FRotator(0, 0, -90).Quaternion());
		FTransform currentTransform = actorTransform;
		FVector currentLocation = currentTransform.GetLocation();
		currentLocation += FVector(blockSize.X / 2, blockSize.Y / 2, 0);
		currentTransform.SetLocation(currentLocation);

		FTransform oppositeTransform = currentTransform;
		oppositeTransform.SetLocation(currentLocation + FVector((m_numCol + 1) * blockSize.X, 0, 0));
		//First and last col are only block
		for (int row = 0; row < m_numRow; ++row)
		{
			m_paperGroupedSpriteComponent->AddInstance(currentTransform, m_blockSprite, true);
			m_paperGroupedSpriteComponent->AddInstance(oppositeTransform, m_blockSprite, true);
			currentLocation += FVector(0, emptyBlockSize.Y, 0);
			currentTransform.SetLocation(currentLocation);
			oppositeTransform.SetLocation(currentLocation + FVector((m_numCol + 1) * blockSize.X, 0, 0));
		}

		//Last line is also only block
		for (int col = 0; col <= m_numCol + 1; ++col)
		{
			m_paperGroupedSpriteComponent->AddInstance(currentTransform, m_blockSprite, true);
			currentLocation += FVector(blockSize.X, 0, 0);
			currentTransform.SetLocation(currentLocation);
		}

		currentLocation = FVector(actorTransform.GetLocation().X + (1.5 * emptyBlockSize.X), actorTransform.GetLocation().Y + (emptyBlockSize.Y / 2), actorTransform.GetLocation().Z);
		currentTransform.SetLocation(currentLocation);

		for (int col = 0; col < m_numCol; ++col)
		{
			for (int row = 0; row < m_numRow; ++row)
			{
				m_paperGroupedSpriteComponent->AddInstance(currentTransform, m_emptyBlockSprite, true);
				currentLocation += FVector(0, emptyBlockSize.Y, 0);
				currentTransform.SetLocation(currentLocation);
			}

			currentLocation = FVector(currentLocation.X + blockSize.X, actorTransform.GetLocation().Y + (emptyBlockSize.Y / 2), currentLocation.Z);
			currentTransform.SetLocation(currentLocation);

		}

		// place spawn location
		m_spawnLocation->SetRelativeLocation(FVector((blockSize.X * FMath::Floor(m_numCol/2)) + (blockSize.X/2), -1.5f*(blockSize.Y),0));
	}
}

ABaseBlock* ATetGrid::SpawnPiece()
{
	int32 idx = FMath::RandRange(0, m_blockToSpawn.Num() - 1);

	FTransform spawnLocationTransform = m_spawnLocation->GetComponentTransform();

	ABaseBlock* spawnedPiece = GetWorld()->SpawnActor<ABaseBlock>(m_blockToSpawn[idx], spawnLocationTransform);
	FVector deltaVectorToMove = FVector::One() * 1000;

	UPaperGroupedSpriteComponent* renderComponent = spawnedPiece->GetRenderComponent();
	for (int i = 0; i < renderComponent->GetInstanceCount(); ++i)
	{
		FTransform currentBlockTranform;
		renderComponent->GetInstanceTransform(i, currentBlockTranform, true);
		FVector currentBlockLocation = currentBlockTranform.GetTranslation();

		FVector deltaVector = currentBlockLocation - spawnLocationTransform.GetTranslation();

		if (deltaVector.IsZero())
		{
			return spawnedPiece;
		}
		else if (deltaVector.SizeSquared() < deltaVectorToMove.SizeSquared())
		{
			deltaVectorToMove = deltaVector;
		}
	}

	spawnedPiece->AddActorWorldOffset(deltaVectorToMove);

	return spawnedPiece;
}

int ATetGrid::GetBlockSize()
{
	return m_blockSprite->GetSourceSize().X;
}

bool ATetGrid::CanMovePiece(int a_row, int a_col, FVector a_vectorToAdd)
{
	a_vectorToAdd.Normalize();
	FVector piecePositionInGrid = FVector(a_col, a_row, 0);
	FVector newPiecePosition = piecePositionInGrid + a_vectorToAdd;

	if (newPiecePosition.Y >= m_numRow || newPiecePosition.X < 0 || newPiecePosition.X >= m_numCol)
	{
		return false;
	}

	if (newPiecePosition.Y >= 0 && m_gridData[newPiecePosition.Y][newPiecePosition.X].m_pieceOnCell != nullptr)
	{
		return false;
	}

	return true;
}
