// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseProjectile.h"
#include "CPP_FireBall.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_FireBall : public ACPP_BaseProjectile
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACPP_FireBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
