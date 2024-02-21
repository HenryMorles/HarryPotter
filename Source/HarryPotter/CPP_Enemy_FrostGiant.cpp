// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Enemy_FrostGiant.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ACPP_Enemy_FrostGiant::ACPP_Enemy_FrostGiant()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	AttackColision_1->SetupAttachment(GetMesh(), FName("LeftHand_Socket"));
	AttackColision_2->SetupAttachment(GetMesh(), FName("RightHand_Socket"));

	MaxHealth = 100;

	Damage = 40.0f;
}

float ACPP_Enemy_FrostGiant::PlayAttackMontage()
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
