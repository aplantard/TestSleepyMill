// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperGroupedSpriteActor.h"
#include "TetGrid.generated.h"

class UPaperSprite;
class UTetPaperGroupedSpriteComponent;

UCLASS()
class TESTSLEEPYMILL_API ATetGrid : public AActor
{
	GENERATED_BODY()

public:
	ATetGrid();
	~ATetGrid();

	virtual void OnConstruction(const FTransform& Transform) override;;

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

private:
	TArray<TArray<bool>> m_gridCellOccupied;
};
