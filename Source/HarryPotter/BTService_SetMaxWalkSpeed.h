// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SetMaxWalkSpeed.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API UBTService_SetMaxWalkSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTService_SetMaxWalkSpeed();

	UPROPERTY(EditAnywhere)
	float NewSpeed;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
