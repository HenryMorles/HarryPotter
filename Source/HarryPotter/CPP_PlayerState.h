// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_MagicWand.h"
#include "GameFramework/PlayerState.h"
#include "CPP_PlayerState.generated.h"

class SpellSettings
{

public:

	SpellSettings();


	Spell Type;

	float BaseDamage;
	float Damage;

	float BaseManaCost;
	float ManaCost;

	int SpellLevel;

public:

	void SetSpellLevel(int NewSpellLevel);
};



UCLASS()
class HARRYPOTTER_API ACPP_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	ACPP_PlayerState();

	SpellSettings LeviationSpell_Settings;
	SpellSettings FireBallSpell_Settings;
	SpellSettings FireStormSpell_Settings;
	SpellSettings DashSpell_Settings;
	SpellSettings CuttingSpell_Settings;
};



