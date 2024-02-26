// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BossFightGate.h"
#include "Components/BoxComponent.h"
#include "CPP_PlayerCharacter.h"


ACPP_BossFightGate::ACPP_BossFightGate()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetBox"));
	TargetBox->SetupAttachment(RootComponent);

	bIsActive = true;
}

void ACPP_BossFightGate::BeginPlay()
{
	Super::BeginPlay();

	TargetBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_BossFightGate::BeginOverlap);
}

void ACPP_BossFightGate::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActive && Cast<ACPP_PlayerCharacter>(OtherActor))
	{
		CloseGate();
	}
}

void ACPP_BossFightGate::OpenGate()
{
	Super::CloseGate();
}

void ACPP_BossFightGate::CloseGate()
{
	Super::OpenGate();
}
