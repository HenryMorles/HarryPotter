// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseEnemy.h"
#include "CPP_Enemy_Goblin.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_Enemy_Goblin : public ACPP_BaseEnemy
{
	GENERATED_BODY()
	
public:

	ACPP_Enemy_Goblin();

	bool bIsCalledToDuty;

public:

	virtual void Death(AActor* KillerRef)override;

};
