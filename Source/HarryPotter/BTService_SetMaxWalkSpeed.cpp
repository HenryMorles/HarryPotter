// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetMaxWalkSpeed.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "CPP_BaseAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTService_SetMaxWalkSpeed::UBTService_SetMaxWalkSpeed()
{
	NodeName = "Set Max Walk Speed";

	NewSpeed = 0;
}

void UBTService_SetMaxWalkSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACPP_BaseAICharacter* OwnerPawn = Cast<ACPP_BaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!OwnerPawn->bIsPlayingAnimation && !OwnerPawn->bIsDeath)
	{
		OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
	else
	{
		OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = 0.0f;  //If some animation is played, the speed set to 0
	}
}
