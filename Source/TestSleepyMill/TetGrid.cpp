// Fill out your copyright notice in the Description page of Project Settings.

#include "TetGrid.h"

#include "Components/TetPaperGroupedSpriteComponent.h"
#include "PaperSprite.h"
#include "BaseBlock.h"

ATetGrid::ATetGrid()
{
	m_paperGroupedSpriteComponent = CreateDefaultSubobject<UTetPaperGroupedSpriteComponent>(TEXT("PaperGroupedSpriteComponent"));
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
		m_spawnLocation->SetRelativeLocation(FVector((blockSize.X * ((m_numCol+2)/2)) + (blockSize.X/2), -blockSize.Y,0));
	}
}

ABaseBlock* ATetGrid::SpawnBlock()
{
	return GetWorld()->SpawnActor<ABaseBlock>(m_objectToSpawn, m_spawnLocation->GetComponentTransform());
}

int ATetGrid::GetBlockSize()
{
	return m_blockSprite->GetSourceSize().X;
}
