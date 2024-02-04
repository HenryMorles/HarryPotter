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

	bIsHoldingObject = false;
}

void ACPP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	WandRef = GetWorld()->SpawnActor<ACPP_MagicWand>(WandClass);
	WandRef->AttachToComponent(SafeModeWandSocket, FAttachmentTransformRules::KeepRelativeTransform);
	WandRef->SetOwner(this);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACPP_PlayerCharacter::MoveGrabbedObject, 0.1f, true);
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

void ACPP_PlayerCharacter::MoveGrabbedObject()
{
	if (bIsHoldingObject)
	{
		PhysicsHandle->SetTargetLocation(GrabPoint->GetComponentLocation());
		PhysicsHandle->GrabbedComponent->AddImpulse(FVector(0.0f, 0.0f, 0.0f));
	}
}

void ACPP_PlayerCharacter::UseMagic()
{
	if (bBattleMode && !bIsPlayingAnimation)
	{
		switch (WandRef->CurrentSpell)
		{
		case WandRef->Spell::LeviationSpell:
		{
			if (bIsHoldingObject)
			{
				if (StopLeviationSpell_Montage)
				{
					float AnimDuration = PlayAnimMontage(StopLeviationSpell_Montage);

					BeginPlay_Anim(AnimDuration);

					FTimerHandle UnusedHandle;
					GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::StopUseLeviationSpell, AnimDuration / 2, false);
				}
			}
			else
			{
				if (LeviationSpell_Montage)
				{
					float AnimDuration = PlayAnimMontage(LeviationSpell_Montage);

					BeginPlay_Anim(AnimDuration);

					FTimerHandle UnusedHandle;
					GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseLeviationSpell, AnimDuration / 2 - 0.2f, false);
				}
			}
		}break;

		case WandRef->Spell::FireBallSpell:
		{
			if (FireSpell_Montage)
			{
				float AnimDuration = PlayAnimMontage(FireSpell_Montage);

				BeginPlay_Anim(AnimDuration);

				//WandRef->UseFireBallSpell();

				FTimerHandle UnusedHandle;
				GetWorldTimerManager().SetTimer(UnusedHandle, WandRef, &ACPP_MagicWand::UseFireBallSpell, AnimDuration / 2, false);
			}
		}break;

		}
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