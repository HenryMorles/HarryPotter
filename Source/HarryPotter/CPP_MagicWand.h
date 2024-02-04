// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_MagicWand.generated.h"

UCLASS()
class HARRYPOTTER_API ACPP_MagicWand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_MagicWand();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SpellSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACPP_BaseProjectile> FireBallClass;

	enum Spell
	{
		LeviationSpell,
		FireBallSpell
	};

	Spell CurrentSpell;

	TArray<Spell> ListOfSpells;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseLeviationSpell();
	void StopUseLeviationSpell();

	void UseFireBallSpell();

	void ChangeSpellUp();
	void ChangeSpellDown();
};
