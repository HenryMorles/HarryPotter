// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"

// Sets default values
ACPP_BaseCharacter::ACPP_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100;

	Health = MaxHealth;

	bBattleMode = false;

	bIsPlayingAnimation = false;
}

// Called when the game starts or when spawned
void ACPP_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACPP_BaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= DamageApplied;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HP: %f"), Health));

	if (Health <= 0)
	{
		Death();
	}

	return DamageApplied;
}

void ACPP_BaseCharacter::Death()
{

}

void ACPP_BaseCharacter::BeginPlay_Anim(float AnimDuration)
{
	bIsPlayingAnimation = true;

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACPP_BaseCharacter::EndPlay_Anim, AnimDuration, false);
}

void ACPP_BaseCharacter::EndPlay_Anim()
{
	bIsPlayingAnimation = false;
}