// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Spawner_MinionsOfTroll.generated.h"

UCLASS()
class HARRYPOTTER_API ACPP_Spawner_MinionsOfTroll : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_Spawner_MinionsOfTroll();

	TArray<USceneComponent*> PointsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SpawnPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SpawnPoint_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SpawnPoint_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SpawnPoint_4;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACPP_Enemy_Goblin> GoblinClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumOfGoblins;

	int NumOfGoblins;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnGoblin();

	void DeathGoblin();
};
