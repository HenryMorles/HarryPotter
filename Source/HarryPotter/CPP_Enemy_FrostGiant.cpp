// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Enemy_FrostGiant.h"
#include "GameFramework/CharacterMovementComponent.h"


ACPP_Enemy_FrostGiant::ACPP_Enemy_FrostGiant()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}
