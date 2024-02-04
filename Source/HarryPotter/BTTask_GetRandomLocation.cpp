// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"


UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	NodeName = "GetRandomLocation";
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner())
	{
		APawn* PawnRef = OwnerComp.GetAIOwner()->GetPawn();

		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

		if (NavSys)
		{
			FVector RandomLocation;

			NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), PawnRef->GetActorLocation(), RandomLocation, 5000.0f);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomLocation"), RandomLocation);
		}
	}

	return EBTNodeResult::Succeeded;
}
