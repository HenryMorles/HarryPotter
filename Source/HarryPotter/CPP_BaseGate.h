// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CPP_BaseGate.generated.h"

UCLASS()
class HARRYPOTTER_API ACPP_BaseGate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_BaseGate();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpen;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* CurveFloat;

	class UTimelineComponent* MyTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndLocation;

	FOnTimelineFloat InterpFunction{};

	UPROPERTY()
	float ZOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void OpenGate();
	virtual void CloseGate();

	UFUNCTION()
	void TimelineProgress(float Value);

};
