// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "CPP_MagicWand.h"
#include "Animation/AnimMontage.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Blueprint/UserWidget.h"
#include "CPP_PlayerState.h"
#include "CPP_FireStormSpell.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CPP_BaseAICharacter.h"


ACPP_PlayerCharacter::ACPP_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	BattleModeWandSocket = CreateDefaultSubobject<USceneComponent>(TEXT("BattleModeWandSocket"));
	BattleModeWandSocket->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	BattleModeWandSocket->SetRelativeLocation(FVector(14.0f, -11.0f, 1.5f));
	BattleModeWandSocket->SetRelativeRotation(FRotator(0.0f, 172.7f, 350.0f));

	SafeModeWandSocket = CreateDefaultSubobject<USceneComponent>(TEXT("SafeModeWandSocket"));
	SafeModeWandSocket->SetupAttachment(GetMesh(), TEXT("RightUpLegSocket"));
	SafeModeWandSocket->SetRelativeLocation(FVector(-16.5f, 17.0f, -5.0f));
	SafeModeWandSocket->SetRelativeRotation(FRotator(126.0f, 100.0f, 126.0f));

	GrabPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrabPoint"));
	GrabPoint->SetupAttachment(FollowCamera);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	ForwardDashPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ForwardDashPoint"));
	ForwardDashPoint->SetupAttachment(RootComponent);

	BackDashPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BackDashPoint"));
	BackDashPoint->SetupAttachment(RootComponent);

	RightDashPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightDashPoint"));
	RightDashPoint->SetupAttachment(RootComponent);

	LeftDashPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftDashPoint"));
	LeftDashPoint->SetupAttachment(RootComponent);

	DashTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DashTimeline"));
	InterpFunction.BindUFunction(this, FName("TimelineProgress"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	DashParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DashParticle"));
	DashParticle->SetupAttachment(RootComponent);

	SealedCharacterRef = nullptr;

	bIsUsingLeviationSpell = false;
	bIsUsingFireStormSpell = false;

	MaxHealth = 100.0f;
	MaxMana = 100.0f;
	MaxStrength = 100.0f;

	ManaRegenPerSecond = 3.0f;
	HealthRegenPerSecond = 1.0;

	PurityOfSoul = 0;
}

void ACPP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Mana = MaxMana;
	Strength = MaxStrength;

	WandRef = GetWorld()->SpawnActor<ACPP_MagicWand>(WandClass);
	WandRef->AttachToComponent(SafeModeWandSocket, FAttachmentTransformRules::KeepRelativeTransform);
	WandRef->SetOwner(this);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACPP_PlayerCharacter::MoveGrabbedObject, 0.1f, true);

	FTimerHandle UnusedHandle1;
	GetWorldTimerManager().SetTimer(UnusedHandle1, this, &ACPP_PlayerCharacter::StatsRegen, 1.0f, true);

	DashParticle->Deactivate();

	PlayerStateRef = Cast<ACPP_PlayerState>(GetPlayerState());

	if (CurveFloat)  //Setting the Timeline
	{
		DashTimeline->AddInterpFloat(CurveFloat, InterpFunction, FName("Alpha"));
		DashTimeline->SetTimelineFinishedFunc(TimelineFinished);

		DashTimeline->SetLooping(false);
		DashTimeline->SetIgnoreTimeDilation(true);
	}
}

void ACPP_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForvard"), this, &ACPP_PlayerCharacter::MoveForvard);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACPP_PlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACPP_PlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &ACPP_PlayerCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("SwitchBattleMode"), EInputEvent::IE_Pressed, this, &ACPP_PlayerCharacter::SwitchBattleMode);
	PlayerInputComponent->BindAction(TEXT("UseMagic"), EInputEvent::IE_Pressed, this, &ACPP_PlayerCharacter::UseMagic);
	PlayerInputComponent->BindAction(TEXT("ChangeSpellUp"), EInputEvent::IE_Pressed, this, &ACPP_PlayerCharacter::ChangeSpellUp);
	PlayerInputComponent->BindAction(TEXT("ChangeSpellDown"), EInputEvent::IE_Pressed, this, &ACPP_PlayerCharacter::ChangeSpellDown);
}

void ACPP_PlayerCharacter::MoveForvard(float AxisValue)
{
	if (Controller && AxisValue)
	{
		if (bBattleMode)
		{
			AddMovementInput(GetActorForwardVector() * AxisValue);
		}
		else
		{
			const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(Direction, AxisValue);
		}
	}
}

void ACPP_PlayerCharacter::MoveRight(float AxisValue)
{
	if (Controller && AxisValue)
	{
		if (bBattleMode)
		{
			AddMovementInput(GetActorRightVector() * AxisValue);
		}
		else
		{
			const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(Direction, AxisValue);
		}
	}
}

void ACPP_PlayerCharacter::SwitchBattleMode()
{
	if (!bIsPlayingAnimation)
	{
		if (bBattleMode)
		{
			bBattleMode = false;

			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;

			WandRef->AttachToComponent(SafeModeWandSocket, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else
		{
			bBattleMode = true;

			GetCharacterMovement()->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = true;

			WandRef->AttachToComponent(BattleModeWandSocket, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void ACPP_PlayerCharacter::MoveGrabbedObject()  // Update location of grabbed object and add impuls for correct moving
{
	if (bIsUsingLeviationSpell && PlayerStateRef)
	{
		PhysicsHandle->SetTargetLocation(GrabPoint->GetComponentLocation());
		PhysicsHandle->GrabbedComponent->AddImpulse(FVector(0.0f, 0.0f, 0.0f));

		if (!UseMana(PlayerStateRef->LeviationSpell_Settings.ManaCost))
		{
			float AnimDuration = PlayAnimMontage(StopLeviationSpell_Montage);
			bIsUsingLeviationSpell = false;

			BeginPlay_Anim(AnimDuration, false);

			GetWorldTimerManager().SetTimer(SpellTimer, WandRef, &ACPP_MagicWand::StopUseLeviationSpell, AnimDuration / 2, false);
		}
	}
}

void ACPP_PlayerCharacter::Dash()
{
	if (bBattleMode && !bIsDashing && UseMana(PlayerStateRef->DashSpell_Settings.ManaCost))
	{
		FVector PlayerDirection = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), GetVelocity());

		if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey("W")))
		{
			bIsDashing = true;

			StartDashLocation = GetActorLocation();
			EndDashLocation = ForwardDashPoint->GetComponentLocation();

			DashParticle->Activate();

			DashTimeline->PlayFromStart();

			if (DashSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}
		}
		else if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey("S")))
		{
			bIsDashing = true;

			StartDashLocation = GetActorLocation();
			EndDashLocation = BackDashPoint->GetComponentLocation();

			DashParticle->Activate();

			DashTimeline->PlayFromStart();

			if (DashSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}
		}
		else if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey("A")))
		{
			bIsDashing = true;

			StartDashLocation = GetActorLocation();
			EndDashLocation = LeftDashPoint->GetComponentLocation();

			DashParticle->Activate();

			DashTimeline->PlayFromStart();

			if (DashSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}
		}
		else if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey("D")))
		{
			bIsDashing = true;

			StartDashLocation = GetActorLocation();
			EndDashLocation = RightDashPoint->GetComponentLocation();

			DashParticle->Activate();

			DashTimeline->PlayFromStart();

			if (DashSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}
		}
	}
}

void ACPP_PlayerCharacter::UseMagic()  // Choosing a spell to use
{
	if (bBattleMode && !bIsPlayingAnimation)
	{
		if (WandRef->CurrentSpell == Spell::LeviationSpell)
		{
			if (!bIsUsingLeviationSpell && LeviationSpell_Montage && UseMana(PlayerStateRef->LeviationSpell_Settings.ManaCost))
			{
				UseLeviationSpell();
			}
			else if (StopLeviationSpell_Montage && bIsUsingLeviationSpell)
			{
				StopUseLeviationSpell();
			}
		}

		else if (WandRef->CurrentSpell == Spell::FireBallSpell)
		{
			if (FireBallSpell_Montage)
			{
				UseFireBallSpell();
			}
		}

		else if (WandRef->CurrentSpell == Spell::FireStormSpell)
		{
			if (!bIsUsingFireStormSpell)
			{
				if (FireStormSpell_Montage)
				{
					UseFireStormSpell();
				}
			}
			else
			{
				if (StopFireStormSpell_Montage)
				{
					StopUseFireStormSpell();
				}
			}
		}
		else if (WandRef->CurrentSpell == Spell::SoulCleansingSpell)
		{
			if (!bIsUsingSoulCleansingSpell)
			{
				if (SoulCleansingSpell_Montage)
				{
					UseSoulCleansingSpell();
				}
			}
		}
		else if (WandRef->CurrentSpell == Spell::SealCreature)
		{
			if (!bIsUsingSoulCleansingSpell)
			{
				if (SoulCleansingSpell_Montage)
				{
					UseSealCreatureSpell();
				}
			}
		}
		else if (WandRef->CurrentSpell == Spell::CuttingSpell)
		{
			if (CuttingSpell_Montage)
			{
				UseCuttingSpell();
			}
		}
	}
}

void ACPP_PlayerCharacter::UseLeviationSpell()
{
	float AnimDuration = PlayAnimMontage(LeviationSpell_Montage);

	BeginPlay_Anim(AnimDuration, false);

	if (LeviationSpellCastSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LeviationSpellCastSound, GetActorLocation());
	}

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseLeviationSpell, AnimDuration / 2 - 0.2f, false);
}

void ACPP_PlayerCharacter::StopUseLeviationSpell()
{
	float AnimDuration = PlayAnimMontage(StopLeviationSpell_Montage);

	if (LeviationSpellStopSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LeviationSpellStopSound, GetActorLocation());
	}

	BeginPlay_Anim(AnimDuration, false);

	GetWorldTimerManager().SetTimer(SpellTimer, WandRef, &ACPP_MagicWand::StopUseLeviationSpell, AnimDuration / 2, false);
}

void ACPP_PlayerCharacter::UseFireBallSpell()
{
	float AnimDuration = PlayAnimMontage(FireBallSpell_Montage);

	BeginPlay_Anim(AnimDuration, false);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseFireBallSpell, AnimDuration / 2, false);

	UGameplayStatics::PlaySoundAtLocation(this, FireBallLaunchSound, GetActorLocation(), 1.0f, 1.0f, 0.7f);
}

void ACPP_PlayerCharacter::UseFireStormSpell()
{
	float AnimDuration = PlayAnimMontage(FireStormSpell_Montage);

	BeginPlay_Anim(AnimDuration, true);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseFireStormSpell, AnimDuration / 2 - 0.05f, false);

	UGameplayStatics::PlaySoundAtLocation(this, FireStormLaunchSound, GetActorLocation());
}

void ACPP_PlayerCharacter::StopUseFireStormSpell()
{
	bIsUsingFireStormSpell = false;

	float AnimDuration = PlayAnimMontage(StopFireStormSpell_Montage);

	BeginPlay_Anim(AnimDuration, true);

	GetWorldTimerManager().SetTimer(SpellTimer, WandRef, &ACPP_MagicWand::StopUseFireStormSpell, AnimDuration / 2 + 0.2f, false);

	UGameplayStatics::PlaySoundAtLocation(WandRef->FireStormRef, WandRef->FireStormRef->EndSound, WandRef->FireStormRef->GetActorLocation());

	WandRef->FireStormRef->LoopSoundRef->Stop();
}

void ACPP_PlayerCharacter::UseSoulCleansingSpell()
{
	FHitResult Hit;

	FVector TraceStart = FollowCamera->GetComponentLocation();
	FVector TraceEnd = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 1000.0f;  // Line from camera forward

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, QueryParams);

	if (Hit.GetActor() && Cast<ACPP_BaseAICharacter>(Hit.GetActor()) && Cast<ACPP_BaseAICharacter>(Hit.GetActor())->bIsDeath)
	{
		float AnimDuration = PlayAnimMontage(SoulCleansingSpell_Montage);

		BeginPlay_Anim(AnimDuration, true);

		WandRef->UseSoulCleansingSpell(Cast<ACPP_BaseAICharacter>(Hit.GetActor()));

		if (SoulCleansingLoopSound && SoulCleansingDestroyingSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SoulCleansingLoopSound, GetActorLocation());

			UGameplayStatics::PlaySoundAtLocation(this, SoulCleansingDestroyingSound, GetActorLocation());
		}

		PurityOfSoul++;
	}
}

void ACPP_PlayerCharacter::UseSealCreatureSpell()
{
	FHitResult Hit;

	FVector TraceStart = FollowCamera->GetComponentLocation();
	FVector TraceEnd = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 1000.0f;  // Line from camera forward

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, QueryParams);

	if (Hit.GetActor() && Cast<ACPP_BaseAICharacter>(Hit.GetActor()) && Cast<ACPP_BaseAICharacter>(Hit.GetActor())->bIsDeath)
	{
		float AnimDuration = PlayAnimMontage(SoulCleansingSpell_Montage);

		BeginPlay_Anim(AnimDuration, true);

		WandRef->UseSealCreatureSpell(Cast<ACPP_BaseAICharacter>(Hit.GetActor()));

		if (SealCreatureLoopSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SealCreatureLoopSound, GetActorLocation());
		}

		PurityOfSoul -= 5;

		SealedCharacterRef = Cast<ACPP_BaseAICharacter>(Hit.GetActor());
		SealedCharacterRef->SetOwner(this);
	}
}

void ACPP_PlayerCharacter::UseCuttingSpell()
{
	float AnimDuration = PlayAnimMontage(CuttingSpell_Montage) - 0.3f;

	BeginPlay_Anim(AnimDuration, false);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseCuttingSpell, AnimDuration / 2 - 0.3f, false);

	//UGameplayStatics::PlaySoundAtLocation(this, CuttingSpellLaunchSound, GetActorLocation(), 1.0f, 1.0f, 1.2f);
	UGameplayStatics::PlaySoundAtLocation(this, CuttingSpellLaunchSound, GetActorLocation(), 1.0f, 1.0f, 0.0f);
}

void ACPP_PlayerCharacter::EndPlay_Anim(bool bStopCharacter)
{
	if (bIsUsingFireStormSpell)  //When we use FireStormSpell we can't move
	{
		Super::EndPlay_Anim(false);
	}
	else
	{
		Super::EndPlay_Anim(bStopCharacter);
	}
}

void ACPP_PlayerCharacter::ChangeSpellUp()
{
	if (!bIsPlayingAnimation && !bIsUsingFireStormSpell && !bIsUsingLeviationSpell && !bIsUsingSoulCleansingSpell)
	{
		WandRef->ChangeSpellUp();
	}
}

void ACPP_PlayerCharacter::ChangeSpellDown()
{
	if (!bIsPlayingAnimation && !bIsUsingFireStormSpell && !bIsUsingLeviationSpell && !bIsUsingSoulCleansingSpell)
	{
		WandRef->ChangeSpellDown();
	}
}

void ACPP_PlayerCharacter::Death(AActor* KillerRef)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Death")));
}

bool ACPP_PlayerCharacter::UseMana(float ManaCost)
{
	if (SealedCharacterRef)
	{
		FHitResult Hit;
		FVector ShotDirection;
		FPointDamageEvent DamageEvent(ManaCost / 10, Hit, ShotDirection, nullptr);

		SealedCharacterRef->TakeDamage(ManaCost / 10, DamageEvent, nullptr, this->GetOwner()->GetOwner());

		return true;
	}

	else if (Mana - ManaCost > 0)
	{
		Mana -= ManaCost;

		MaxMana += ManaCost / 100;

		return true;
	}

	else if (Health - (ManaCost / 10) > 1)   // If mana == 0 then we use HP to cast spell. In this case we spend less HP then mana
	{
		Mana = 0.0f;

		Health -= (ManaCost / 10);

		if (MaxHealth > 50.0f)
		{
			MaxHealth -= ManaCost / 1000;
		}

		return true;
	}

	else
	{
		return false;
	}
}

void ACPP_PlayerCharacter::StatsRegen()
{
	if (Health < MaxHealth)
	{
		if (Health + HealthRegenPerSecond < MaxHealth)
		{
			Health += HealthRegenPerSecond;
		}
		else
		{
			Health = MaxHealth;
		}
	}
	else if (Mana < MaxMana || Strength < MaxStrength)
	{
		if (!bIsUsingLeviationSpell && !bIsUsingFireStormSpell && !bIsPlayingAnimation)
		{
			if (Mana + ManaRegenPerSecond < MaxMana)
			{
				Mana += ManaRegenPerSecond;
			}
			else
			{
				Mana = MaxMana;
			}

			if (Strength + StrengthRegenPerSecond < MaxStrength)
			{
				Strength += StrengthRegenPerSecond;
			}
			else
			{
				Strength = MaxStrength;
			}
		}
	}

}

void ACPP_PlayerCharacter::EndDash()
{
	bIsDashing = false;
}

void ACPP_PlayerCharacter::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartDashLocation, EndDashLocation, Value);
	SetActorLocation(NewLocation, true);
}

void ACPP_PlayerCharacter::OnTimelineFinished()
{
	DashParticle->Deactivate();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACPP_PlayerCharacter::EndDash, 1.0f, false);
}
