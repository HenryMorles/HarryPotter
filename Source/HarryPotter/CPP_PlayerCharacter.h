// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "CPP_PlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class HARRYPOTTER_API ACPP_PlayerCharacter : public ACPP_BaseCharacter
{
	GENERATED_BODY()

public:

	ACPP_PlayerCharacter();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* BattleModeWandSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SafeModeWandSocket;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACPP_MagicWand> WandClass;

	UPROPERTY(BlueprintReadWrite)
	ACPP_MagicWand* WandRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* LeviationSpell_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* StopLeviationSpell_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* FireBallSpell_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* FireStormSpell_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* StopFireStormSpell_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* SoulCleansingSpell_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* CuttingSpell_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ForwardDashPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* BackDashPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RightDashPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* LeftDashPoint;

	FTimerHandle SpellTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* DashParticle;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* CurveFloat;

	class UTimelineComponent* DashTimeline;

	UPROPERTY()
	FVector StartDashLocation;

	UPROPERTY()
	FVector EndDashLocation;

	FOnTimelineFloat InterpFunction{};
	FOnTimelineEvent TimelineFinished{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* GrabPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsingLeviationSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsingFireStormSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsingSoulCleansingSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDashing;

	UPROPERTY(EditAnywhere)
	USoundBase* FireStormLaunchSound;

	UPROPERTY(EditAnywhere)
	USoundBase* FireBallLaunchSound;

	UPROPERTY(EditAnywhere)
	USoundBase* LeviationSpellCastSound;

	UPROPERTY(EditAnywhere)
	USoundBase* LeviationSpellStopSound;

	UPROPERTY(EditAnywhere)
	USoundBase* DashSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SoulCleansingLoopSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SoulCleansingDestroyingSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SealCreatureLoopSound;

	UPROPERTY(EditAnywhere)
	USoundBase* SealCreatureDestroyingSound;

	UPROPERTY(EditAnywhere)
	USoundBase* CuttingSpellLaunchSound;

	class ACPP_PlayerState* PlayerStateRef;

	class ACPP_BaseAICharacter* SealedCharacterRef;

	float ManaRegenPerSecond;

	float HealthRegenPerSecond;

	float StrengthRegenPerSecond;

	UPrimitiveComponent* GrabbedComponentRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PurityOfSoul;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;

private:

	void MoveForvard(float AxisValue);
	void MoveRight(float AxisValue);

	void SwitchBattleMode();

	void MoveGrabbedObject();

	void Dash();

public:

	void UseMagic();

	void UseLeviationSpell();
	void StopUseLeviationSpell();

	void UseFireBallSpell();

	void UseFireStormSpell();
	void StopUseFireStormSpell();

	void UseSoulCleansingSpell();

	void UseSealCreatureSpell();

	void UseCuttingSpell();

	void ChangeSpellUp();
	void ChangeSpellDown();

	void Death()override;

	bool UseMana(float ManaCost);

	void StatsRegen();

	void EndDash();

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void OnTimelineFinished();

	virtual void EndPlay_Anim(bool bStopCharacter)override;
};
