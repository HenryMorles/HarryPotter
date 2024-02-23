// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Enemy_Goblin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Spawner_MinionsOfTroll.h"


ACPP_Enemy_Goblin::ACPP_Enemy_Goblin()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	AttackColision_1->SetupAttachment(GetMesh(), FName("LeftHand_Socket"));
	AttackColision_2->SetupAttachment(GetMesh(), FName("RightHand_Socket"));

	MaxHealth = 40;

	Damage = 10.0f;

	bIsCalledToDuty = false;
}

void ACPP_Enemy_Goblin::Death(AActor* KillerRef)
{
	Super::Death(KillerRef);

	if (bIsCalledToDuty && Cast<ACPP_Spawner_MinionsOfTroll>(GetOwner()))
	{
		Cast<ACPP_Spawner_MinionsOfTroll>(GetOwner())->DeathGoblin();
	}
}
