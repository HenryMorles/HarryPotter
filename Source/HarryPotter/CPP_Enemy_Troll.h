// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseEnemy.h"
#include "CPP_Enemy_Troll.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_Enemy_Troll : public ACPP_BaseEnemy
{
	GENERATED_BODY()
	
public:

	ACPP_Enemy_Troll();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* Attack_Montage_2;

public:

	virtual float PlayAttackMontage();
};
