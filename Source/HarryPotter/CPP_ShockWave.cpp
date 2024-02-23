// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ShockWave.h"
#include "Components/BoxComponent.h"
#include "CPP_BaseAICharacter.h"
#include "CPP_PlayerCharacter.h"


// Sets default values
ACPP_ShockWave::ACPP_ShockWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackArea = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackArea"));
	AttackArea->SetupAttachment(RootComponent);

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeline"));

	InterpFunction.BindUFunction(this, FName("TimelineProgress"));
	FinishFunction.BindUFunction(this, FName("TimelineFinish"));

	StartScale = FVector(1.0f, 1.0f, 1.0f);
	EndScale = FVector(8.0f, 8.0f, 8.0f);
}

// Called when the game starts or when spawned
void ACPP_ShockWave::BeginPlay()
{
	Super::BeginPlay();
	
	AttackArea->OnComponentBeginOverlap.AddDynamic(this, &ACPP_ShockWave::BeginOverlap);

	if (CurveFloat)  //Setting the Timeline
	{
		MyTimeline->AddInterpFloat(CurveFloat, InterpFunction, FName("Alpha"));
		MyTimeline->SetTimelineFinishedFunc(FinishFunction);

		MyTimeline->SetLooping(false);
		MyTimeline->SetIgnoreTimeDilation(true);

		MyTimeline->Play();
	}
}

// Called every frame
void ACPP_ShockWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_ShockWave::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		float TempDamage;

		if (Cast<ACPP_BaseAICharacter>(GetOwner()))
		{
			TempDamage = Cast<ACPP_BaseAICharacter>(GetOwner())->Damage;
		}

		FHitResult Hit;
		FVector ShotDirection;
		FPointDamageEvent DamageEvent(TempDamage, Hit, ShotDirection, nullptr);
		

		OtherActor->TakeDamage(TempDamage, DamageEvent, nullptr, GetOwner());
	}
}

void ACPP_ShockWave::TimelineProgress(float Value)
{
	FVector NewScale = FMath::Lerp(StartScale, EndScale, Value);
	SetActorScale3D(NewScale);
}

void ACPP_ShockWave::TimelineFinish()
{
	Destroy();
}

