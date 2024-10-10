// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperGroupedSpriteComponent.h"
#include "TetPaperGroupedSpriteComponent.generated.h"

/**
 * 
 */
UCLASS()
class TESTSLEEPYMILL_API UTetPaperGroupedSpriteComponent : public UPaperGroupedSpriteComponent
{
	GENERATED_BODY()
	
public: 
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) override;

};
