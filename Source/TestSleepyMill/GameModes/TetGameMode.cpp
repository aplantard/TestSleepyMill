// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TetGameMode.h"

#include "Controllers/TetPlayerController.h"
#include "Pawns/TetPawn.h"

void ATetGameMode::InitGameState()
{
	Super::InitGameState();
	DefaultPawnClass = m_playerCustomPawn;
	PlayerControllerClass = m_customPlayerController;
}
