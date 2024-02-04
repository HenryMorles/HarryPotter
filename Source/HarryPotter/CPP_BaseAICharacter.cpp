// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_AIController.h"
#include "CPP_PlayerCharacter.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"



ACPP_BaseAICharacter::ACPP_BaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	PawnSensor->SensingInterval = 0.25f; // 4 times per second
	PawnSensor->bOnlySensePlayers = false;
	PawnSensor->SetPeripheralVisionAngle(85.f);
}

void ACPP_BaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensor->OnSeePawn.AddDynamic(this, &ACPP_BaseAICharacter::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &ACPP_BaseAICharacter::OnHearNoise);
}

void ACPP_BaseAICharacter::Death()
{
	Super::Death();

	Destroy();
}

void ACPP_BaseAICharacter::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{

}

void ACPP_BaseAICharacter::OnSeePawn(APawn* OtherPawn)
{
	if (Cast<ACPP_PlayerCharacter>(OtherPawn))
	{
		Cast<ACPP_AIController>(GetController())->GetBlackboardComponent()->SetValueAsObject(FName("TargetCharacter"), OtherPawn);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("I see you")));
	}
}

void ACPP_BaseAICharacter::Attack(APawn* TargetPawn)
{
	if (Attack_Montage)
	{
		float AnimDuration = PlayAnimMontage(Attack_Montage);

		BeginPlay_Anim(AnimDuration);

	}
}

void ACPP_BaseAICharacter::BeginPlay_Anim(float AnimDuration)
{
	Super::BeginPlay_Anim(AnimDuration);

	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	bUseControllerRotationYaw = false;
}

void ACPP_BaseAICharacter::EndPlay_Anim()
{
	Super::EndPlay_Anim();

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	bUseControllerRotationYaw = true;
}
