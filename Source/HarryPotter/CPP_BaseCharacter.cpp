// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "CPP_Fire_DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ACPP_BaseCharacter::ACPP_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100;

	bBattleMode = false;

	bIsPlayingAnimation = false;

	bIsDeath = false;

	FireDamage = 3.0f;

	BurningDamageClass = UCPP_Fire_DamageType::StaticClass();

	BurningTypeSettings.bIsApplied = false;
	BurningTypeSettings.DamageSeconds_Max = 5;
	BurningTypeSettings.DamageSeconds_Proggres = 0;

	BurningParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BurningParticle"));
	BurningParticle->SetupAttachment(RootComponent);
	BurningParticle->SetVisibility(false);
}

// Called when the game starts or when spawned
void ACPP_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	BurningParticle->SetVisibility(true);

	BurningParticle->Deactivate();
}

// Called every frame
void ACPP_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACPP_BaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bool PlayReactAnim;

	if (BurningTypeSettings.bIsApplied)
	{
		PlayReactAnim = false;
	}
	else
	{
		PlayReactAnim = true;
	}

	if (DamageEvent.DamageTypeClass == BurningDamageClass)
	{
		GetWorldTimerManager().SetTimer(BurningEffect_Handle, this, &ACPP_BaseCharacter::TakeFireDamage, 1.0f, true);

		BurningParticle->Activate();

		BurningTypeSettings.DamageSeconds_Proggres = 0.0f;

		PlayReactAnim = true;
	}

	Health -= DamageApplied;

	if (Health <= 0)
	{
		StopBurningEffect();

		Death();
	}
	else
	{
		if (TakeDamage_Montage && !bIsDeath && PlayReactAnim)
		{
			StopAnimMontage(GetCurrentMontage());

			float AnimDuration = PlayAnimMontage(TakeDamage_Montage);

			BeginPlay_Anim(AnimDuration, true);
		}
	}

	return DamageApplied;
}

void ACPP_BaseCharacter::TakeFireDamage()
{
	if (BurningTypeSettings.DamageSeconds_Proggres < BurningTypeSettings.DamageSeconds_Max)
	{
		BurningTypeSettings.bIsApplied = true;

		FHitResult Hit;
		FVector ShotDirection;
		FPointDamageEvent DamageEvent(FireDamage, Hit, ShotDirection, nullptr);

		BurningTypeSettings.DamageSeconds_Proggres++;

		TakeDamage(FireDamage, DamageEvent, nullptr, this);
	}
	else
	{
		StopBurningEffect();
	}
}

void ACPP_BaseCharacter::Death() //The function is overridden in other classes
{
	bIsDeath = true;
}

void ACPP_BaseCharacter::StopBurningEffect()
{
	GetWorldTimerManager().ClearTimer(BurningEffect_Handle);

	BurningTypeSettings.bIsApplied = false;

	BurningParticle->Deactivate();

	BurningTypeSettings.DamageSeconds_Proggres = 0;
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