// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Enemy_Troll.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Spawner_MinionsOfTroll.h"
#include "CPP_BossFightGate.h"


ACPP_Enemy_Troll::ACPP_Enemy_Troll()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	AttackColision_1->SetupAttachment(GetMesh(), FName("LeftHand_Socket"));
	AttackColision_2->SetupAttachment(GetMesh(), FName("RightHand_Socket"));

	MaxHealth = 200;

	Damage = 60.0f;
}

float ACPP_Enemy_Troll::PlayAttackMontage()
{
	int32 MaxNamberOfAnimation = 2;

	int32 min = 0;

	int32 NamberOfAnimation = FMath::RandRange(min, MaxNamberOfAnimation - 1);

	if (NamberOfAnimation == 0)
	{
		return Super::PlayAttackMontage();
	}

	else if (NamberOfAnimation == 1)
	{
		return PlayAnimMontage(Attack_Montage_2);
	}

	return 0.0f;
}

void ACPP_Enemy_Troll::Death(AActor* KillerRef)
{
	Super::Death(KillerRef);

	TArray<AActor*> MinionsSpawnerRef;
	UGameplayStatics::GetAllActorsOfClass(this, ACPP_Spawner_MinionsOfTroll::StaticClass(), MinionsSpawnerRef);

	if (!MinionsSpawnerRef.IsEmpty())
	{
		MinionsSpawnerRef[0]->Destroy();
	}

	TArray<AActor*> ArrayOfGates;
	UGameplayStatics::GetAllActorsOfClass(this, GateClass, ArrayOfGates);

	if (!ArrayOfGates.IsEmpty())
	{
		for (AActor* GateRef : ArrayOfGates)
		{
			Cast<ACPP_BossFightGate>(GateRef)->OpenGate();
			Cast<ACPP_BossFightGate>(GateRef)->bIsActive = false;
		}
	}
}