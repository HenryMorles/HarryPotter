// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_BaseCharacter.generated.h"

UCLASS()
class HARRYPOTTER_API ACPP_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_BaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(BlueprintReadWrite)
	bool bBattleMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* TakeDamage_Montage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	virtual void Death();

	virtual void BeginPlay_Anim(float AnimDuration, bool bStopCharacter);
	virtual void EndPlay_Anim(bool bStopCharacter);
};
