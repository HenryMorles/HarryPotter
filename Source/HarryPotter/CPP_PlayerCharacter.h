// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseCharacter.h"
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

	FTimerHandle SpellTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* GrabPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsingLeviationSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsingFireStormSpell;

	class ACPP_PlayerState* PlayerStateRef;

	float ManaRegenPerSecond;

	UPrimitiveComponent* GrabbedComponentRef;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;

private:

	void MoveForvard(float AxisValue);
	void MoveRight(float AxisValue);

	void SwitchBattleMode();

	void MoveGrabbedObject();

public:

	void UseMagic();

	void UseLeviationSpell();
	void StopUseLeviationSpell();

	void UseFireBallSpell();

	void UseFireStormSpell();
	void StopUseFireStormSpell();

	void ChangeSpellUp();
	void ChangeSpellDown();

	void Death()override;

	void ManaRegen();

	virtual void EndPlay_Anim(bool bStopCharacter)override;
};
