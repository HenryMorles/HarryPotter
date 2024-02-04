// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerController.h"
#include "Blueprint/UserWidget.h"


ACPP_PlayerController::ACPP_PlayerController()
{

}

void ACPP_PlayerController::BeginPlay()
{
	AimWidget = CreateWidget(this, AimWidgetClass);
	if (AimWidget)
	{
		AimWidget->AddToViewport();
	}
}
