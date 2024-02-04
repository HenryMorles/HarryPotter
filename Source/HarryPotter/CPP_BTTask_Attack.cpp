// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BTTask_Attack.h"
#include "AIController.h"
#include "CPP_BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UCPP_BTTask_Attack::UCPP_BTTask_Attack()
{

}

EBTNodeResult::Type UCPP_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	return EBTNodeResult::Type();
}
