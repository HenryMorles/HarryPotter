// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GateWithButton.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"


// Sets default values
ACPP_GateWithButton::ACPP_GateWithButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonCollider_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollider_1"));
	ButtonCollider_1->SetupAttachment(RootComponent);

	ButtonCollider_2 = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollider_2"));
	ButtonCollider_2->SetupAttachment(RootComponent);

	ButtonCollider_3 = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollider_3"));
	ButtonCollider_3->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeline"));

	InterpFunction.BindUFunction(this, FName("TimelineProgress"));

	ZOffset = 650.0f;

	bButtonState_1 = false;
	bButtonState_2 = false;
	bButtonState_3 = false;

	bIsOpen = false;
}

// Called when the game starts or when spawned
void ACPP_GateWithButton::BeginPlay()
{
	Super::BeginPlay();
	
	ButtonCollider_1->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GateWithButton::BeginOverlap);
	ButtonCollider_1->OnComponentEndOverlap.AddDynamic(this, &ACPP_GateWithButton::EndOverlap);

	ButtonCollider_2->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GateWithButton::BeginOverlap);
	ButtonCollider_2->OnComponentEndOverlap.AddDynamic(this, &ACPP_GateWithButton::EndOverlap);

	ButtonCollider_3->OnComponentBeginOverlap.AddDynamic(this, &ACPP_GateWithButton::BeginOverlap);
	ButtonCollider_3->OnComponentEndOverlap.AddDynamic(this, &ACPP_GateWithButton::EndOverlap);

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
		
		StartLocation = Mesh->GetComponentLocation();
		EndLocation = FVector(Mesh->GetComponentLocation().X, Mesh->GetComponentLocation().Y, Mesh->GetComponentLocation().Z + ZOffset);

		MyTimeline->SetLooping(false);
		MyTimeline->SetIgnoreTimeDilation(true);
	}
}

// Called every frame
void ACPP_GateWithButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_GateWithButton::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<UPrimitiveComponent*> OverlappingComponents;

	ButtonCollider_1->GetOverlappingComponents(OverlappingComponents);     // If an object falls into place, we should check if the remaining objects are in place
	bButtonState_1 = !OverlappingComponents.IsEmpty();

	ButtonCollider_2->GetOverlappingComponents(OverlappingComponents);
	bButtonState_2 = !OverlappingComponents.IsEmpty();

	ButtonCollider_3->GetOverlappingComponents(OverlappingComponents);
	bButtonState_3 = !OverlappingComponents.IsEmpty();

	if (OverlappedComponent == ButtonCollider_1)
	{
		bButtonState_1 = true;
	}
	else if (OverlappedComponent == ButtonCollider_2)
	{
		bButtonState_2 = true;
	}
	else if (OverlappedComponent == ButtonCollider_3)
	{
		bButtonState_3 = true;
	}

	if (bButtonState_1 && bButtonState_2 && bButtonState_3)
	{
		OpenGate();
	}
}

void ACPP_GateWithButton::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComponent == ButtonCollider_1)
	{
		bButtonState_1 = false;
	}
	else if (OverlappedComponent == ButtonCollider_2)
	{
		bButtonState_2 = false;
	}
	else if (OverlappedComponent == ButtonCollider_3)
	{
		bButtonState_3 = false;
	}
	CloseGate();
}

void ACPP_GateWithButton::OpenGate()
{
	if (!bIsOpen)
	{
		MyTimeline->Play();
		bIsOpen = true;
	}
}

void ACPP_GateWithButton::CloseGate()
{
	if (bIsOpen)
	{
		MyTimeline->Reverse();
		bIsOpen = false;
	}
}

void ACPP_GateWithButton::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	Mesh->SetWorldLocation(NewLocation);
}

