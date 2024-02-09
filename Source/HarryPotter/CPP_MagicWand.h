// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_MagicWand.generated.h"


enum Spell;

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
	TSubclassOf<class ACPP_FireStormSpell> FireStormClass;

	ACPP_FireStormSpell* FireStormRef;

	Spell CurrentSpell;

	TArray<Spell> ListOfSpells;

public:
	
	void UseLeviationSpell();
	void StopUseLeviationSpell();

	void UseFireBallSpell();

	void UseFireStormSpell();
	void StopUseFireStormSpell();

	void ChangeSpellUp();
	void ChangeSpellDown();
};



enum Spell
{
	LeviationSpell,
	FireBallSpell,
	FireStormSpell
};