// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CPP_ShockWave.generated.h"

UCLASS()
class HARRYPOTTER_API ACPP_ShockWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_ShockWave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* AttackArea;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* CurveFloat;

	class UTimelineComponent* MyTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndScale;

	FOnTimelineFloat InterpFunction{};
	FOnTimelineEvent FinishFunction{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void TimelineFinish();

};
