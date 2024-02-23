// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Spawner_MinionsOfTroll.h"
#include "CPP_Enemy_Goblin.h"
#include "CPP_AIController.h"


// Sets default values
ACPP_Spawner_MinionsOfTroll::ACPP_Spawner_MinionsOfTroll()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnPoint_1 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint_1"));
	SpawnPoint_2 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint_2"));
	SpawnPoint_3 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint_3"));
	SpawnPoint_4 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint_4"));

	PointsArray.Add(SpawnPoint_1);
	PointsArray.Add(SpawnPoint_2);
	PointsArray.Add(SpawnPoint_3);
	PointsArray.Add(SpawnPoint_4);

	NumOfGoblins = 0;

	MaxNumOfGoblins = 10;
}

// Called when the game starts or when spawned
void ACPP_Spawner_MinionsOfTroll::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle UnusedHandle1;
	GetWorldTimerManager().SetTimer(UnusedHandle1, this, &ACPP_Spawner_MinionsOfTroll::SpawnGoblin, 10.0f, true);
}

// Called every frame
void ACPP_Spawner_MinionsOfTroll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Spawner_MinionsOfTroll::SpawnGoblin()
{
	if (NumOfGoblins < MaxNumOfGoblins)
	{
		int32 Index = FMath::RandRange(0, PointsArray.Num() - 1);

		ACPP_Enemy_Goblin* GoblinRef = GetWorld()->SpawnActor<ACPP_Enemy_Goblin>(GoblinClass, PointsArray[Index]->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));

		GoblinRef->AIControllerClass = ACPP_AIController::StaticClass();
		GoblinRef->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

		GoblinRef->SpawnDefaultController();

		Cast<ACPP_AIController>(GoblinRef->GetController())->RunBehaviorTree(GoblinRef->AIBehavior);

		GoblinRef->bIsCalledToDuty = true;
		GoblinRef->SetOwner(this);

		NumOfGoblins++;
	}
}

void ACPP_Spawner_MinionsOfTroll::DeathGoblin()
{
	NumOfGoblins--;
}
