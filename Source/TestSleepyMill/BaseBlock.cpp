// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlock.h"

#include "PaperGroupedSpriteComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ABaseBlock::ABaseBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	RootComponent = m_rootComponent;

	GetRenderComponent()->SetupAttachment(m_rootComponent);
}

void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();

	UPaperGroupedSpriteComponent* renderComponent = GetRenderComponent();
	for (int i = 0; i < renderComponent->GetInstanceCount(); ++i)
	{
		FTransform currentBlockTranform;
		renderComponent->GetInstanceTransform(i, currentBlockTranform, false);

		UTextRenderComponent* currentTextRenderComponent = Cast<UTextRenderComponent>(AddComponentByClass(UTextRenderComponent::StaticClass(), true, currentBlockTranform, false));
		currentTextRenderComponent->AttachToComponent(m_rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		currentTextRenderComponent->RegisterComponent();
		currentTextRenderComponent->CreationMethod = EComponentCreationMethod::Instance;

		currentTextRenderComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
		currentTextRenderComponent->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
		currentTextRenderComponent->SetTextRenderColor(FColor::Black);

		currentTextRenderComponent->SetRelativeRotation(FRotator(90, 0, -90));
		currentTextRenderComponent->AddWorldOffset(FVector(0,0,1));

		currentTextRenderComponent->TranslucencySortPriority = 2;

		TCHAR currentChar = FMath::RandRange((int)'A', (int)'Z');
		FString currentCharStr = FString::Chr(currentChar);
		currentTextRenderComponent->SetText(FText::FromString(currentCharStr));

		m_lettersOnPiece.Add(i, currentTextRenderComponent);
	}
}


