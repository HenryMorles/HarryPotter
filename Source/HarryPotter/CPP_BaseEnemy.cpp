// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseEnemy.h"
#include "CPP_PlayerCharacter.h"

ACPP_BaseEnemy::ACPP_BaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsEverSawPlayer = false;
}

void ACPP_BaseEnemy::OnSeePawn(APawn* OtherPawn)
{
	Super::OnSeePawn(OtherPawn);

	if (!bIsEverSawPlayer && OtherPawn && Cast<ACPP_PlayerCharacter>(OtherPawn) && Aggro_Montage)
	{
		float AnimDuration = PlayAnimMontage(Aggro_Montage);

		BeginPlay_Anim(AnimDuration, true);

		bIsEverSawPlayer = true;
	}
}
