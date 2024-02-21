// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACPP_BaseProjectile::ACPP_BaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void ACPP_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->OnComponentHit.AddDynamic(this, &ACPP_BaseProjectile::OnHit);
}

// Called every frame
void ACPP_BaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_BaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		FVector ShotDirection;
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, DamageClassEvent);
		OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this->GetOwner()->GetOwner());

		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation(), FVector(0.5f, 0.5f, 0.5f));
		}
		if (HitSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, HitSound, GetActorLocation());
		}
	}

	Destroy();
}

