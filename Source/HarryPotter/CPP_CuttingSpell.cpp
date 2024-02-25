// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CuttingSpell.h"
#include "GameFramework/ProjectileMovementComponent.h"


ACPP_CuttingSpell::ACPP_CuttingSpell()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent->InitialSpeed = 4000.0f;
	MovementComponent->MaxSpeed = 4000.0f;
	MovementComponent->ProjectileGravityScale = 1.0f;

	Damage = 40.0f;
}
