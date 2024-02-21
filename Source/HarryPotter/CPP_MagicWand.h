// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_MagicWand.generated.h"

UENUM(BlueprintType)
enum class Spell : uint8
{
	LeviationSpell UMETA(DisplayName = "LeviationSpell"),
	FireBallSpell UMETA(DisplayName = "FireBallSpell"),
	FireStormSpell UMETA(DisplayName = "FireStormSpell"),
	SoulCleansingSpell UMETA(DisplayName = "SoulCleansingSpell"),
	SealCreature UMETA(DisplayName = "SealCreature"),
	CuttingSpell UMETA(DisplayName = "CuttingSpell")
};

UCLASS()
class HARRYPOTTER_API ACPP_MagicWand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_MagicWand();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SpellSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACPP_BaseProjectile> FireBallClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACPP_BaseProjectile> CuttingSpellClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACPP_FireStormSpell> FireStormClass;

	ACPP_FireStormSpell* FireStormRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Spell CurrentSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<Spell> ListOfSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* SoulCleansingParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* SealCreatureParticle;

public:
	
	void UseLeviationSpell();
	void StopUseLeviationSpell();

	void UseFireBallSpell();

	void UseFireStormSpell();
	void StopUseFireStormSpell();

	void UseCuttingSpell();

	void UseSoulCleansingSpell(class ACPP_BaseAICharacter* TargetPawn);

	void UseSealCreatureSpell(class ACPP_BaseAICharacter* TargetPawn);
	void SpawnSealParticles(ACPP_BaseAICharacter* TargetPawn, FVector SpawnLocation);

	void ChangeSpellUp();
	void ChangeSpellDown();
};



