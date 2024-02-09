// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ACPP_BaseCharacter::ACPP_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100;

	bBattleMode = false;

	bIsPlayingAnimation = false;

	bIsDeath = false;
}

// Called when the game starts or when spawned
void ACPP_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
}

// Called every frame
void ACPP_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACPP_BaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= DamageApplied;

	if (Health <= 0)
	{
		Death();
	}
	else
	{
		if (TakeDamage_Montage)
		{
			float AnimDuration = PlayAnimMontage(TakeDamage_Montage);

			BeginPlay_Anim(AnimDuration, true);
		}
	}

	return DamageApplied;
}

void ACPP_BaseCharacter::Death() //The function is overridden in other classes
{
	bIsDeath = true;
}

void ACPP_BaseCharacter::BeginPlay_Anim(float AnimDuration, bool bStopCharacter)   // Function for waiting and setting for animation to play
{
	if (bStopCharacter)  // If you want to stop a character during playing animation
	{
		bUseControllerRotationYaw = true;

		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}

	bIsPlayingAnimation = true;

	FTimerHandle UnusedHandle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACPP_BaseCharacter::EndPlay_Anim, bStopCharacter);

	GetWorldTimerManager().SetTimer(UnusedHandle, Delegate, AnimDuration, false);
}

void ACPP_BaseCharacter::EndPlay_Anim(bool bStopCharacter)
{
	if (bStopCharacter)
	{
		bUseControllerRotationYaw = true;

		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}

	bIsPlayingAnimation = false;
}