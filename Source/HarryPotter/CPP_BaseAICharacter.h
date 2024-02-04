// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseCharacter.h"
#include "CPP_BaseAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_BaseAICharacter : public ACPP_BaseCharacter
{
	GENERATED_BODY()

public:

	ACPP_BaseAICharacter();

	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* Attack_Montage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	virtual void Death()override;

	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);

	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
	void Attack(APawn* TargetPawn);

	virtual void BeginPlay_Anim(float AnimDuration)override;
	virtual void EndPlay_Anim()override;
};
