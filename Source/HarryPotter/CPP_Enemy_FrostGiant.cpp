// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Enemy_FrostGiant.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ACPP_Enemy_FrostGiant::ACPP_Enemy_FrostGiant()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	AttackColision_1->SetupAttachment(GetMesh(), FName("LeftHand_Socket"));
	AttackColision_2->SetupAttachment(GetMesh(), FName("RightHand_Socket"));

	MaxHealth = 20;

	Damage = 50.0f;
}