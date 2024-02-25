// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseGate.h"

// Sets default values
ACPP_BaseGate::ACPP_BaseGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeline"));

	InterpFunction.BindUFunction(this, FName("TimelineProgress"));

	ZOffset = 650.0f;

	bIsOpen = false;

	StartLocation = Mesh->GetComponentLocation();
	EndLocation = FVector(Mesh->GetComponentLocation().X, Mesh->GetComponentLocation().Y, Mesh->GetComponentLocation().Z + ZOffset);
}

// Called when the game starts or when spawned
void ACPP_BaseGate::BeginPlay()
{
	Super::BeginPlay();
	
	if (bIsOpen)
	{
		OpenGate();
	}
	else
	{
		CloseGate();
	}
	if (CurveFloat)  // Setting the timeline
	{
		MyTimeline->AddInterpFloat(CurveFloat, InterpFunction, FName("Alpha"));

		MyTimeline->SetLooping(false);
		MyTimeline->SetIgnoreTimeDilation(true);
	}
}

void ACPP_BaseGate::OpenGate()
{
	if (!bIsOpen)
	{
		MyTimeline->Play();
		bIsOpen = true;
	}
}

void ACPP_BaseGate::CloseGate()
{
	if (bIsOpen)
	{
		MyTimeline->Reverse();
		bIsOpen = false;
	}
}

void ACPP_BaseGate::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	Mesh->SetWorldLocation(NewLocation);
}

