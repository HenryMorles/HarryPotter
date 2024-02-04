// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HARRYPOTTER_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ACPP_PlayerController();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AimWidgetClass;

	UUserWidget* AimWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay()override;
	
};
