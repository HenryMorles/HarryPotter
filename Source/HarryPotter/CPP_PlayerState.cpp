// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerState.h"

ACPP_PlayerState::ACPP_PlayerState()
{
	LeviationSpell_Settings.Type = Spell::LeviationSpell;
	LeviationSpell_Settings.BaseManaCost = 0.5f;
	LeviationSpell_Settings.SetSpellLevel(1);

	FireBallSpell_Settings.Type = Spell::FireBallSpell;
	FireBallSpell_Settings.BaseDamage = 10;
	FireBallSpell_Settings.BaseManaCost = 10;
	FireBallSpell_Settings.SetSpellLevel(1);

	FireStormSpell_Settings.Type = Spell::FireStormSpell;
	FireStormSpell_Settings.BaseDamage = 10;
	FireStormSpell_Settings.BaseManaCost = 2;
	FireStormSpell_Settings.SetSpellLevel(1);

	CuttingSpell_Settings.Type = Spell::CuttingSpell;
	CuttingSpell_Settings.BaseDamage = 20;
	CuttingSpell_Settings.BaseManaCost = 20;
	CuttingSpell_Settings.SetSpellLevel(1);

	DashSpell_Settings.BaseManaCost = 5;
	DashSpell_Settings.SetSpellLevel(1);
}



SpellSettings::SpellSettings()
{
	SpellLevel = 0;
	Damage = 0.0f;
	ManaCost = 0.0f;
}

void SpellSettings::SetSpellLevel(int NewSpellLevel)
{
	SpellLevel = NewSpellLevel;

	if (SpellLevel > 1)
	{
		Damage = BaseDamage + ((BaseDamage / 100) * 10) * SpellLevel;

		ManaCost = BaseManaCost - ((BaseManaCost / 100) * 10) * SpellLevel;
	}
	else
	{
		Damage = BaseDamage;
		ManaCost = BaseManaCost;
	}
}
