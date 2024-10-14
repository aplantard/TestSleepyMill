// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperGroupedSpriteActor.h"
#include "TetGrid.generated.h"

class UPaperSprite;
class UTetPaperGroupedSpriteComponent;
class ABaseBlock;

USTRUCT(BlueprintType)
struct FGridCellData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int m_instanceIdx = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseBlock* m_pieceOnCell = nullptr;

};

USTRUCT(BlueprintType)
struct FGridRow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FGridCellData> m_cellData;

	void Add(FGridCellData a_cellData)
	{
		m_cellData.Add(a_cellData);
	}

	int Num() const
	{
		return m_cellData.Num();
	}

	FGridCellData& operator[] (int32 a_cellDataIdx)
	{
		return m_cellData[a_cellDataIdx];
	}

};

UCLASS()
class TESTSLEEPYMILL_API ATetGrid : public AActor
{
	GENERATED_BODY()

public:
	ATetGrid();
	~ATetGrid();

	virtual void OnConstruction(const FTransform& a_transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int m_numCol = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int m_numRow = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* m_blockSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* m_emptyBlockSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTetPaperGroupedSpriteComponent* m_paperGroupedSpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* m_spawnLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseBlock>> m_blockToSpawn;

	UPROPERTY()
	TArray<FGridRow> m_gridData;

	UFUNCTION(BlueprintCallable)
	FTransform GetSpawnLocation() { return m_spawnLocation->GetComponentTransform(); };

	UFUNCTION(BlueprintCallable)
	ABaseBlock* SpawnPiece();

	UFUNCTION(BlueprintCallable)
	int GetBlockSize();

	UFUNCTION()
	bool CanMovePiece(int a_row, int a_col, FVector a_vectorToAdd);
};
