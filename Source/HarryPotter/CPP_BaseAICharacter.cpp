// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_AIController.h"
#include "CPP_PlayerCharacter.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"


ACPP_BaseAICharacter::ACPP_BaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	PawnSensor->SensingInterval = 0.25f; // 4 times per second
	PawnSensor->bOnlySensePlayers = false;
	PawnSensor->SetPeripheralVisionAngle(85.f);

	AttackColision_1 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackColision1"));

	AttackColision_2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackColision2"));

	Damage = 0.0f;

	bIsPlayerControlled = false;

	bIsDamageApplied = false;

	bIsSealed = false;
}

void ACPP_BaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensor->OnSeePawn.AddDynamic(this, &ACPP_BaseAICharacter::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &ACPP_BaseAICharacter::OnHearNoise);

	AttackColision_1->OnComponentBeginOverlap.AddDynamic(this, &ACPP_BaseAICharacter::BeginOverlap);
	AttackColision_2->OnComponentBeginOverlap.AddDynamic(this, &ACPP_BaseAICharacter::BeginOverlap);

	AttackColision_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackColision_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPP_BaseAICharacter::Death()
{
	if (bIsDeath)
	{
		if (SealCreatureParticleRef)
		{
			SealCreatureParticleRef->Deactivate();
		}

		ACPP_PlayerCharacter* PlayerRef = Cast<ACPP_PlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		PlayerRef->PurityOfSoul--;

		if (bIsSealed)
		{
			Cast<ACPP_PlayerCharacter>(GetOwner())->SealedCharacterRef = nullptr;
		}

		Destroy();
	}

	else
	{
		Super::Death();

		GetCharacterMovement()->MaxWalkSpeed = 0.0f;

		if (Death_Montage && bIsDeath)
		{
			float AnimDuration = PlayAnimMontage(Death_Montage);
		}

		Cast<ACPP_AIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("IsDeath"), true);
	}
}

void ACPP_BaseAICharacter::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{

}

void ACPP_BaseAICharacter::OnSeePawn(APawn* OtherPawn)
{
	if (OtherPawn && (Cast<ACPP_BaseAICharacter>(OtherPawn) && Cast<ACPP_BaseAICharacter>(OtherPawn)->bIsPlayerControlled) || Cast<ACPP_PlayerCharacter>(OtherPawn))   //Is OtherPawn the Player or a Player-controlled pawn
	{
		NoticedPawn(OtherPawn);
	}
}

float ACPP_BaseAICharacter::Attack(APawn* TargetPawn)
{
	if (Attack_Montage_1 && !bIsDeath && !bIsPlayingAnimation)
	{
		float AnimDuration = PlayAttackMontage();

		BeginPlay_Anim(AnimDuration, true);

		return AnimDuration;
	}

	return 0.0f;
}

void ACPP_BaseAICharacter::BeginPlay_Anim(float AnimDuration, bool bStopCharacter)
{
	Super::BeginPlay_Anim(AnimDuration, bStopCharacter);

	Cast<ACPP_AIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("IsPlayingAnimation"), true);
}

void ACPP_BaseAICharacter::EndPlay_Anim(bool bStopCharacter)
{
	if (!GetCurrentMontage())
	{
		Super::EndPlay_Anim(bStopCharacter);

		AttackColision_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackColision_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bIsDamageApplied = false;

		Cast<ACPP_AIController>(GetController())->GetBlackboardComponent()->ClearValue(FName("IsPlayingAnimation"));
	}
}

void ACPP_BaseAICharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsPlayingAnimation && OtherActor && OtherActor != this)
	{
		if ((Cast<ACPP_BaseAICharacter>(OtherActor) && (Cast<ACPP_BaseAICharacter>(OtherActor)->bIsPlayerControlled) || Cast<ACPP_PlayerCharacter>(OtherActor)))
		{
			FHitResult Hit;
			FVector ShotDirection;
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

			OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this);

			bIsDamageApplied = true;
		}
	}
}

void ACPP_BaseAICharacter::CleansingCharacter()
{
	if (SoulCleansingParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, SoulCleansingParticle, GetActorLocation(), GetActorRotation(), FVector(2.5f, 2.5f, 2.5f));
	}

	Destroy();
}

float ACPP_BaseAICharacter::PlayAttackMontage()
{
	return PlayAnimMontage(Attack_Montage_1);
}

void ACPP_BaseAICharacter::SealCharacter()
{
	if (!bIsSealed)
	{
		bIsSealed = true;

		Health = MaxHealth;
	}
	else
	{
		bIsDeath = true;

		if (bIsSealed)
		{
			Cast<ACPP_PlayerCharacter>(GetOwner())->SealedCharacterRef = nullptr;
		}

		Death();
	}
}

void ACPP_BaseAICharacter::NoticedPawn(APawn* OtherPawn)
{

}
