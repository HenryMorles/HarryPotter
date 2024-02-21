// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MagicWand.h"
#include "Engine/StaticMesh.h"
#include "CPP_PlayerCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "CPP_BaseProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "CPP_FireStormSpell.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_PlayerState.h"
#include "CPP_BaseAICharacter.h"


// Sets default values
ACPP_MagicWand::ACPP_MagicWand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CurrentSpell = Spell::LeviationSpell;

	SpellSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpellSpawnPoint"));
	SpellSpawnPoint->SetupAttachment(RootComponent);

	ListOfSpells.Add(Spell::LeviationSpell);
	ListOfSpells.Add(Spell::FireBallSpell);
	ListOfSpells.Add(Spell::FireStormSpell);
	ListOfSpells.Add(Spell::SoulCleansingSpell);
	ListOfSpells.Add(Spell::SealCreature);
	ListOfSpells.Add(Spell::CuttingSpell);
}

void ACPP_MagicWand::UseLeviationSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef)
	{
		FHitResult Hit;

		FVector TraceStart = OwnerRef->FollowCamera->GetComponentLocation();
		FVector TraceEnd = OwnerRef->FollowCamera->GetComponentLocation() + OwnerRef->FollowCamera->GetForwardVector() * 1000.0f;  // Line from camera forward

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerRef);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);    // Custom channel

		GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, QueryParams);  

		if (Hit.GetComponent())
		{
			OwnerRef->PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.GetComponent(), TEXT(""), Hit.GetActor()->GetActorLocation(), Hit.GetActor()->GetActorRotation());  //Grabbing object to make it fly
			OwnerRef->GrabPoint->SetWorldLocation(FVector(Hit.GetActor()->GetActorLocation().X, Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z + 100));
			OwnerRef->bIsUsingLeviationSpell = true;
			OwnerRef->GrabbedComponentRef = Hit.GetComponent();
		}
	}
}

void ACPP_MagicWand::StopUseLeviationSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef)
	{
		UPrimitiveComponent* GrabbedComponentRef = OwnerRef->PhysicsHandle->GetGrabbedComponent();

		OwnerRef->PhysicsHandle->ReleaseComponent();
		OwnerRef->GrabPoint->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
		OwnerRef->bIsUsingLeviationSpell = false;

		OwnerRef->GrabbedComponentRef->AddImpulse(FVector(0.0f, 0.0f, 0.0f));
	}
}

void ACPP_MagicWand::UseFireBallSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef && OwnerRef->UseMana(OwnerRef->PlayerStateRef->FireBallSpell_Settings.ManaCost))
	{
		FHitResult Hit;

		FVector TraceStart = OwnerRef->FollowCamera->GetComponentLocation();
		FVector TraceEnd = OwnerRef->FollowCamera->GetComponentLocation() + OwnerRef->FollowCamera->GetForwardVector() * 10000.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerRef);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		FVector Location = SpellSpawnPoint->GetComponentLocation();
		GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, QueryParams);

		FRotator Rotation;

		if (Hit.GetActor())  // If I hit an actor, the projectile will fly to the Hit.Location
		{
			Rotation = UKismetMathLibrary::FindLookAtRotation(SpellSpawnPoint->GetComponentLocation(), Hit.Location);
		}
		else
		{
			Rotation = UKismetMathLibrary::FindLookAtRotation(SpellSpawnPoint->GetComponentLocation(), OwnerRef->FollowCamera->GetComponentLocation() + OwnerRef->FollowCamera->GetForwardVector() * 10000.0f);
		}

		ACPP_BaseProjectile* Projectile = GetWorld()->SpawnActor<ACPP_BaseProjectile>(FireBallClass, Location, Rotation);
		Projectile->SetOwner(this);
	}
}

void ACPP_MagicWand::UseFireStormSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef)
	{
		OwnerRef->bIsUsingFireStormSpell = true;

		FireStormRef = GetWorld()->SpawnActor<ACPP_FireStormSpell>(FireStormClass, SpellSpawnPoint->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
		FireStormRef->SetOwner(this);
		FireStormRef->AttachToActor(OwnerRef, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ACPP_MagicWand::StopUseFireStormSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	OwnerRef->bIsUsingFireStormSpell = false;

	if (FireStormRef && OwnerRef)
	{
		FireStormRef->StopUsingSpell();
	}
}

void ACPP_MagicWand::UseCuttingSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef && OwnerRef->UseMana(OwnerRef->PlayerStateRef->CuttingSpell_Settings.ManaCost))
	{
		FHitResult Hit;

		FVector TraceStart = OwnerRef->FollowCamera->GetComponentLocation();
		FVector TraceEnd = OwnerRef->FollowCamera->GetComponentLocation() + OwnerRef->FollowCamera->GetForwardVector() * 10000.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerRef);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		FVector Location = SpellSpawnPoint->GetComponentLocation();
		GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, QueryParams);

		FRotator Rotation;

		if (Hit.GetActor())  // If I hit an actor, the projectile will fly to the Hit.Location
		{
			Rotation = UKismetMathLibrary::FindLookAtRotation(SpellSpawnPoint->GetComponentLocation(), Hit.Location);
		}
		else
		{
			Rotation = UKismetMathLibrary::FindLookAtRotation(SpellSpawnPoint->GetComponentLocation(), OwnerRef->FollowCamera->GetComponentLocation() + OwnerRef->FollowCamera->GetForwardVector() * 10000.0f);
		}

		ACPP_BaseProjectile* Projectile = GetWorld()->SpawnActor<ACPP_BaseProjectile>(CuttingSpellClass, Location, Rotation);
		Projectile->SetOwner(this);
	}
}

void ACPP_MagicWand::UseSoulCleansingSpell(ACPP_BaseAICharacter* TargetPawn)
{
	if (TargetPawn)
	{
		if (SoulCleansingParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, SoulCleansingParticle, TargetPawn->GetActorLocation(), FRotator(180.0f, 0.0f, 0.0f), FVector(2.5f, 2.5f, 2.5f));
		}

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, TargetPawn, &ACPP_BaseAICharacter::CleansingCharacter, 2.5f, false);
	}
}

void ACPP_MagicWand::UseSealCreatureSpell(ACPP_BaseAICharacter* TargetPawn)
{
	if (TargetPawn)
	{
		if (SoulCleansingParticle)
		{
			FVector SpawnLocation = TargetPawn->GetMesh()->GetComponentLocation();

			FTimerHandle UnusedHandle;
			FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACPP_MagicWand::SpawnSealParticles, TargetPawn, SpawnLocation);

			GetWorldTimerManager().SetTimer(UnusedHandle, Delegate, 2.0f, false);
		}

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, TargetPawn, &ACPP_BaseAICharacter::SealCharacter, 2.5f, false);
	}
}

void ACPP_MagicWand::SpawnSealParticles(ACPP_BaseAICharacter* TargetPawn, FVector SpawnLocation)
{
	if (!TargetPawn->bIsSealed)
	{
		TargetPawn->SealCreatureParticleRef = UGameplayStatics::SpawnEmitterAtLocation(TargetPawn, SealCreatureParticle, FVector(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z + 5), FRotator(180.0f, 0.0f, 0.0f), FVector(3.0f, 3.0f, 3.0f));
	}
}

void ACPP_MagicWand::ChangeSpellUp()
{
	int32 NumTargetSpell = ListOfSpells.Find(CurrentSpell) + 1;
	if (NumTargetSpell < ListOfSpells.Num())
	{
		CurrentSpell = ListOfSpells[NumTargetSpell];
	}
	else
	{
		CurrentSpell = ListOfSpells[0];
	}
}

void ACPP_MagicWand::ChangeSpellDown()
{
	int32 NumTargetSpell = ListOfSpells.Find(CurrentSpell) - 1;
	if (NumTargetSpell >= 0)
	{
		CurrentSpell = ListOfSpells[NumTargetSpell];
	}
	else
	{
		CurrentSpell = ListOfSpells[ListOfSpells.Num() - 1];
	}
}
