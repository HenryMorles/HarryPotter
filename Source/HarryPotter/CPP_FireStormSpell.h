// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseSpell.h"
#include "Components/TimelineComponent.h"
#include "CPP_FireStormSpell.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_FireStormSpell : public ACPP_BaseSpell
{
	GENERATED_BODY()
	
public:

	ACPP_FireStormSpell();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* SpellRange;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* CurveFloat;

	class UTimelineComponent* MyTimeline;

	UPROPERTY()
	FVector StartScale;

	UPROPERTY()
	FVector EndScale;

	FOnTimelineFloat InterpFunction{};

	UPROPERTY(EditAnywhere)
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere)
	USoundBase* LoopSound;

	UAudioComponent* LoopSoundRef;

	UPROPERTY(EditAnywhere)
	USoundBase* EndSound;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TimelineProgress(float Value);

	virtual void StopUsingSpell()override;

	void DestroySpell();
};
