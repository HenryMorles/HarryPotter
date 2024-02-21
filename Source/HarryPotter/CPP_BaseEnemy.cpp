// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseEnemy.h"
#include "CPP_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


ACPP_BaseEnemy::ACPP_BaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsEverSawPlayer = false;
}

float ACPP_BaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!BurningTypeSettings.bIsApplied)
	{
		NoticedPawn(Cast<APawn>(DamageCauser));
	}

	return DamageApplied;
}

void ACPP_BaseEnemy::OnSeePawn(APawn* OtherPawn)
{
	Super::OnSeePawn(OtherPawn);

	if (!bIsDeath)
	{
		if (!bIsEverSawPlayer && OtherPawn && Cast<ACPP_PlayerCharacter>(OtherPawn) && Aggro_Montage)
		{
			if (AggroSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, AggroSound, GetActorLocation());
			}
			float AnimDuration = PlayAnimMontage(Aggro_Montage);

			BeginPlay_Anim(AnimDuration, true);

			bIsEverSawPlayer = true;
		}
	}
	else
	{
		Cast<ACPP_AIController>(GetController())->GetBlackboardComponent()->ClearValue(FName("TargetCharacter"));
	}
}

void ACPP_BaseEnemy::NoticedPawn(APawn* OtherPawn)
{
	if (!bIsDeath)
	{
		Super::NoticedPawn(OtherPawn);

		if (OtherPawn)
		{
			if (Cast<ACPP_AIController>(GetController()))
			{
				Cast<ACPP_AIController>(GetController())->GetBlackboardComponent()->SetValueAsObject(FName("TargetCharacter"), OtherPawn);
			}
		}
	}
}
