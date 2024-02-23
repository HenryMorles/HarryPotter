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
	class UAnimMontage* Attack_Montage_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* Death_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* AttackColision_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* AttackColision_2;

	bool bIsDamageApplied;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* SoulCleansingParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* HitFloorParticle;

	bool bIsPlayerControlled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSealed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* SealCreatureParticleRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	virtual void Death(AActor* KillerRef)override;

	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);

	UFUNCTION()
	virtual void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
	float Attack(APawn* TargetPawn);

	virtual void BeginPlay_Anim(float AnimDuration, bool bStopCharacter);

	virtual void EndPlay_Anim(bool bStopCharacter)override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CleansingCharacter();

	virtual float PlayAttackMontage();

	void SealCharacter();

	virtual void NoticedPawn(APawn* OtherPawn);
};
