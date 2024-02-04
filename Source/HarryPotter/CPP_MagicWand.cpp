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
}

// Called when the game starts or when spawned
void ACPP_MagicWand::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_MagicWand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_MagicWand::UseLeviationSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef)
	{
		FHitResult Hit;

		FVector TraceStart = OwnerRef->FollowCamera->GetComponentLocation();
		FVector TraceEnd = OwnerRef->FollowCamera->GetComponentLocation() + OwnerRef->FollowCamera->GetForwardVector() * 1000.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerRef);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

		GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, QueryParams);

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);

		if (Hit.GetComponent())
		{
			OwnerRef->PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.GetComponent(), TEXT(""), Hit.GetActor()->GetActorLocation(), Hit.GetActor()->GetActorRotation());
			OwnerRef->GrabPoint->SetWorldLocation(FVector(Hit.GetActor()->GetActorLocation().X, Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z + 100));
			OwnerRef->bIsHoldingObject = true;
		}
	}
}

void ACPP_MagicWand::StopUseLeviationSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef)
	{
		OwnerRef->PhysicsHandle->ReleaseComponent();
		OwnerRef->GrabPoint->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
		OwnerRef->bIsHoldingObject = false;
	}
}

void ACPP_MagicWand::UseFireBallSpell()
{
	ACPP_PlayerCharacter* OwnerRef = Cast<ACPP_PlayerCharacter>(GetOwner());

	if (OwnerRef)
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

		if (Hit.GetActor())
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

	if (CurrentSpell == Spell::LeviationSpell)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Leviation"));

	if (CurrentSpell == Spell::FireBallSpell)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Fire"));
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

	if (CurrentSpell == Spell::LeviationSpell)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Leviation"));

	if (CurrentSpell == Spell::FireBallSpell)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Fire"));
}
