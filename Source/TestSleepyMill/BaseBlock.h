// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperGroupedSpriteActor.h"
#include "BaseBlock.generated.h"

class UPaperSprite;
class UFloatingPawnMovement;

UCLASS()
class TESTSLEEPYMILL_API ABaseBlock : public APaperGroupedSpriteActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBlock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* m_rootComponent;

};
