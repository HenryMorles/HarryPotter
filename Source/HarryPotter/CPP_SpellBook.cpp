// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_SpellBook.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "CPP_PlayerController.h"


// Sets default values
ACPP_SpellBook::ACPP_SpellBook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	TargetBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetBox"));
	TargetBox->SetupAttachment(Mesh);

	NameOfSpell = Spell::LeviationSpell;
}

// Called when the game starts or when spawned
void ACPP_SpellBook::BeginPlay()
{
	Super::BeginPlay();
	
	TargetBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_SpellBook::BeginOverlap);
}

// Called every frame
void ACPP_SpellBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_SpellBook::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_PlayerCharacter* PlayerRef = Cast<ACPP_PlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (OtherActor && OtherActor == PlayerRef)
	{
		PlayerRef->WandRef->ListOfSpells.Add(NameOfSpell);

		if (SpellInfo_WidgetClass)
		{
			CreateWidget<UUserWidget>(PlayerRef->GetController<ACPP_PlayerController>(), SpellInfo_WidgetClass)->AddToViewport();
		}

		Destroy();
	}
}

