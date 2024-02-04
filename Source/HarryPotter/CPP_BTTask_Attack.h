// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CPP_BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API UCPP_BTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UCPP_BTTask_Attack();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
