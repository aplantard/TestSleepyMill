// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlock.h"

#include "PaperGroupedSpriteComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ABaseBlock::ABaseBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	RootComponent = m_rootComponent;

	GetRenderComponent()->SetupAttachment(m_rootComponent);
	GetRenderComponent()->AddWorldRotation(FRotator(0, 0, -90));
}


