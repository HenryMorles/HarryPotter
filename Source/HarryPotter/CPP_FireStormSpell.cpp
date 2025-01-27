// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_FireStormSpell.h"
#include "Components/BoxComponent.h"
#include "CPP_BaseAICharacter.h"
#include "CPP_PlayerCharacter.h"
#include "CPP_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


ACPP_FireStormSpell::ACPP_FireStormSpell()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(0.1f);

	SpellRange = CreateDefaultSubobject<UBoxComponent>(TEXT("SpellRange"));
	SpellRange->SetupAttachment(RootComponent);

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeline"));

	InterpFunction.BindUFunction(this, FName("TimelineProgress"));

	Damage = 10.0f;
}

void ACPP_FireStormSpell::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)  //Setting the Timeline
	{
		MyTimeline->AddInterpFloat(CurveFloat, InterpFunction, FName("Alpha"));

		StartScale = FVector(0.0f, 0.0f, 0.0f);
		EndScale = FVector(30.0f, 30.0f, 3.0f);

		MyTimeline->SetLooping(false);
		MyTimeline->SetIgnoreTimeDilation(true);

		MyTimeline->Play();
	}
	if (LoopSound)
	{
		LoopSoundRef = UGameplayStatics::SpawnSoundAtLocation(this, LoopSound, GetActorLocation());
	}
}

void ACPP_FireStormSpell::Tick(float DeltaTime)  // Tick Interval = 0.1f;  Every tick enemies that are within the radius of the spell take damage
{
	Super::Tick(DeltaTime);

	ACPP_PlayerCharacter* PlayerRef = Cast<ACPP_PlayerCharacter>(GetOwner()->GetOwner());

	if (PlayerRef && PlayerRef->bIsUsingFireStormSpell)
	{
		if (PlayerRef->UseMana(PlayerRef->PlayerStateRef->FireBallSpell_Settings.ManaCost))
		{
			TArray<AActor*> OverlappingActors;

			SpellRange->GetOverlappingActors(OverlappingActors, ACPP_BaseAICharacter::StaticClass());

			FHitResult Hit;
			FVector ShotDirection;
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, DamageClassEvent);

			for (AActor* OverlappingActor : OverlappingActors)
			{
				Cast<ACPP_BaseAICharacter>(OverlappingActor)->TakeDamage(Damage, DamageEvent, nullptr, this->GetOwner()->GetOwner());
			}
		}
		else
		{
			if (PlayerRef->bIsUsingFireStormSpell)
			{
				PlayerRef->StopUseFireStormSpell();

				if (EndSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, EndSound, GetActorLocation());
				}

				if (LoopSoundRef)
				{
					LoopSoundRef->Stop();
				}
			}
		}
	}
}

void ACPP_FireStormSpell::TimelineProgress(float Value)
{
	FVector NewScale = FMath::Lerp(StartScale, EndScale, Value);
	SpellRange->SetWorldScale3D(NewScale);
}

void ACPP_FireStormSpell::StopUsingSpell()
{
	Super::StopUsingSpell();

	MyTimeline->Reverse();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACPP_FireStormSpell::DestroySpell, 2.0f, true);
}

void ACPP_FireStormSpell::DestroySpell()
{
	Destroy();
}