// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetGameMode.generated.h"

class ATetPlayerController;
class ATetPawn;

UCLASS()
class TESTSLEEPYMILL_API ATetGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void InitGameState() override;

	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<ATetPlayerController> m_customPlayerController;

	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<ATetPawn> m_playerCustomPawn;
};
