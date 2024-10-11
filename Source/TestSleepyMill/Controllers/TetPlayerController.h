// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TetPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ATetGrid;
class ABaseBlock;

UCLASS()
class TESTSLEEPYMILL_API ATetPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> m_inputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> m_rotateInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> m_moveBlockInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> m_moveBlockDownInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	TSoftObjectPtr<ATetGrid> m_gameGrid;

	UFUNCTION()
	void RotatePiece(const FInputActionValue& a_value);

	UFUNCTION()
	void MovePiece(const FInputActionValue& a_value);

	UFUNCTION()
	void MovePieceDown(const FInputActionValue& a_value);

	TSoftObjectPtr<ABaseBlock> m_currentControlledBlock = nullptr;
};
