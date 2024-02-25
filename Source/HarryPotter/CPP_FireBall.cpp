// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_FireBall.h"
#include "GameFramework/ProjectileMovementComponent.h"


ACPP_FireBall::ACPP_FireBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->MaxSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 1.0f;

	Damage = 20.0f;
}