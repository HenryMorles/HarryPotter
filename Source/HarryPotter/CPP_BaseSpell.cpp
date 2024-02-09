// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseSpell.h"

// Sets default values
ACPP_BaseSpell::ACPP_BaseSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 0.0f;

}

// Called when the game starts or when spawned
void ACPP_BaseSpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_BaseSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_BaseSpell::StopUsingSpell()
{

}

