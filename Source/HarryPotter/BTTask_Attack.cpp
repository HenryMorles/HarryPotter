// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "CPP_BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"


UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner())
	{
		ACPP_BaseAICharacter* OwnerPawnRef = Cast<ACPP_BaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

		APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetCharacter")));

		if (TargetPawn)
		{
			OwnerPawnRef->Attack(TargetPawn);
		}
	}

	return EBTNodeResult::Succeeded;
}