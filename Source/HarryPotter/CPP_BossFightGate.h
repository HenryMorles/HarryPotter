// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseGate.h"
#include "CPP_BossFightGate.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_BossFightGate : public ACPP_BaseGate
{
	GENERATED_BODY()
	
public:

	ACPP_BossFightGate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* TargetBox;

	bool bIsActive;

protected:
	
	virtual void BeginPlay()override;

public:

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OpenGate()override;
	virtual void CloseGate()override;
};
