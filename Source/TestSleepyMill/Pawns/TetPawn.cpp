// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TetPawn.h"

#include "Camera/CameraComponent.h"

// Sets default values
ATetPawn::ATetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = m_playerCamera;
	m_playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("mainCamera"));
	m_playerCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	m_playerCamera->OrthoWidth = 1900.f;
}

// Called when the game starts or when spawned
void ATetPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetPawn::Tick(float a_deltaTime)
{
	Super::Tick(a_deltaTime);

}

