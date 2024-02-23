// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_BaseAICharacter.h"


void ACPP_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<ACPP_BaseAICharacter>(GetPawn()))
	{
		AIBehavior = Cast<ACPP_BaseAICharacter>(GetPawn())->AIBehavior;
	}

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
	}
}