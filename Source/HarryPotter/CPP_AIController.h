// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CPP_AIController.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_AIController : public AAIController
{
	GENERATED_BODY()
	
public:

	//ACPP_AIController();

	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;

protected:

	virtual void BeginPlay()override;
};
