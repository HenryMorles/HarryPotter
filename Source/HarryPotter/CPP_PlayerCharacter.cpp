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

	bIsUsingLeviationSpell = false;
	bIsUsingFireStormSpell = false;

	MaxHealth = 100.0f;
	MaxMana = 100.0f;
	MaxStrength = 100.0f;

	ManaRegenPerSecond = 3.0f;
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
	GetWorldTimerManager().SetTimer(UnusedHandle1, this, &ACPP_PlayerCharacter::ManaRegen, 1.0f, true);

	PlayerStateRef = Cast<ACPP_PlayerState>(GetPlayerState());
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
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
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

		if (Mana - PlayerStateRef->LeviationSpell_Settings.ManaCost > 0)
		{
			Mana -= PlayerStateRef->LeviationSpell_Settings.ManaCost;
		}
		else
		{
			float AnimDuration = PlayAnimMontage(StopLeviationSpell_Montage);
			bIsUsingLeviationSpell = false;

			BeginPlay_Anim(AnimDuration, false);

			GetWorldTimerManager().SetTimer(SpellTimer, WandRef, &ACPP_MagicWand::StopUseLeviationSpell, AnimDuration / 2, false);
		}
	}
}

void ACPP_PlayerCharacter::UseMagic()  // Choosing a spell to use
{
	if (bBattleMode && !bIsPlayingAnimation)
	{
		if (WandRef->CurrentSpell == Spell::LeviationSpell && Mana - PlayerStateRef->LeviationSpell_Settings.ManaCost > 0)
		{
			if (!bIsUsingLeviationSpell && LeviationSpell_Montage)
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
				if (StopFireStormSpell_Montage && bIsUsingFireStormSpell)
				{
					StopUseFireStormSpell();
				}
			}
		}
	}
}

void ACPP_PlayerCharacter::UseLeviationSpell()
{
	float AnimDuration = PlayAnimMontage(LeviationSpell_Montage);

	BeginPlay_Anim(AnimDuration, false);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseLeviationSpell, AnimDuration / 2 - 0.2f, false);
}

void ACPP_PlayerCharacter::StopUseLeviationSpell()
{
	float AnimDuration = PlayAnimMontage(StopLeviationSpell_Montage);

	BeginPlay_Anim(AnimDuration, false);

	GetWorldTimerManager().SetTimer(SpellTimer, WandRef, &ACPP_MagicWand::StopUseLeviationSpell, AnimDuration / 2, false);
}

void ACPP_PlayerCharacter::UseFireBallSpell()
{
	float AnimDuration = PlayAnimMontage(FireBallSpell_Montage);

	BeginPlay_Anim(AnimDuration, false);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseFireBallSpell, AnimDuration / 2, false);
}

void ACPP_PlayerCharacter::UseFireStormSpell()
{
	float AnimDuration = PlayAnimMontage(FireStormSpell_Montage);

	BeginPlay_Anim(AnimDuration, true);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseFireStormSpell, AnimDuration / 2 - 0.05f, false);
}

void ACPP_PlayerCharacter::StopUseFireStormSpell()
{
	bIsUsingFireStormSpell = false;

	float AnimDuration = PlayAnimMontage(StopFireStormSpell_Montage);

	BeginPlay_Anim(AnimDuration, true);

	GetWorldTimerManager().SetTimer(SpellTimer, WandRef, &ACPP_MagicWand::StopUseFireStormSpell, AnimDuration / 2 + 0.2f, false);
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
	WandRef->ChangeSpellUp();
}

void ACPP_PlayerCharacter::ChangeSpellDown()
{
	WandRef->ChangeSpellDown();
}

void ACPP_PlayerCharacter::Death()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Death")));
}

void ACPP_PlayerCharacter::ManaRegen()
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
	}
}
