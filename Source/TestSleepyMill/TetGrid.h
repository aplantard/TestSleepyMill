// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperGroupedSpriteActor.h"
#include "TetGrid.generated.h"

class UPaperSprite;
class UTetPaperGroupedSpriteComponent;
class ABaseBlock;

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
	TSubclassOf<ABaseBlock> m_objectToSpawn;

	UFUNCTION(BlueprintCallable)
	FTransform GetSpawnLocation() { return m_spawnLocation->GetComponentTransform(); };

	UFUNCTION(BlueprintCallable)
	ABaseBlock* SpawnBlock();

	UFUNCTION(BlueprintCallable)
	int GetBlockSize();


};
