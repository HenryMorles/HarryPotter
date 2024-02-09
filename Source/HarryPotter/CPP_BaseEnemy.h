// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseAICharacter.h"
#include "CPP_BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_BaseEnemy : public ACPP_BaseAICharacter
{
	GENERATED_BODY()
	
public:

	ACPP_BaseEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* Aggro_Montage;

	bool bIsEverSawPlayer;


	void OnSeePawn(APawn* OtherPawn)override;
};
