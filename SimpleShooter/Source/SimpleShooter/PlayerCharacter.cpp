// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Gun.h"
#include "BaseMeleeWeapon.h"
#include "BaseWeapon.h"
#include "BaseWeapon_Gun.h"
#include "Blueprint/UserWidget.h"
#include "SimpleShooterPlayerController.h"
#include "Animation/AnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	PlayerStance = EPlayerStance::EPS_Unarmed;

	// Spawn MainWeapon
	MainWeapon = GetWorld()->SpawnActor<ABaseWeapon>(MainWeaponClass);

	// Attach MainWeapon to the appropriate Socket
	MainWeapon->UnequipWeapon(this);

	// Spawn Secondary Weapon
	SecondaryWeapon = GetWorld()->SpawnActor<ABaseWeapon>(SecondaryWeaponClass);

	// Attach SecondaryWeapon to the appropriate Socket
	SecondaryWeapon->UnequipWeapon(this);


	// Spawn blueprint of Gun.h
	//EquippedGun = GetWorld()->SpawnActor<AGun>(GunClass);

	//GunSocketName = EquippedGun->EquippedSocketName; 
	//Attach Gun to the specified socket on the character mesh
	//EquippedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, GunSocketName);

	//EquippedMelee = GetWorld()->SpawnActor<ABaseMeleeWeapon>(MeleeClass);

	//MeleeSocketName = EquippedMelee->EquippedSocketName;

	// Attach Melee weapon to the specific socket on the character mesh
	//EquippedMelee->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, MeleeSocketName);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &APlayerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &APlayerCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("SwapWeapon"), IE_Pressed, this, &APlayerCharacter::SwapWeapon);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), IE_Pressed, this, &APlayerCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("ActionButton"), IE_Pressed, this, &APlayerCharacter::ActionButtonPressed);
	PlayerInputComponent->BindAction(TEXT("ActionButton"), IE_Released, this, &APlayerCharacter::ActionButtonReleased);
	PlayerInputComponent->BindAction(TEXT("HeavyAttackButton"), IE_Pressed, this, &APlayerCharacter::HeavyAttackPressed);
	PlayerInputComponent->BindAction(TEXT("HeavyAttackButton"), IE_Released, this, &APlayerCharacter::HeavyAttackReleased);

}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Health - DamageToApply <= 0.f)
	{
		Health = 0;
		bIsDead = true;

		// Store reference to the SimpleShooterGameModeBase
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (GameMode != nullptr)
		{
			// GameMode will handle events after ShooterCharacter is killed (score, subtract lives, track wave numbers, etc.)
			GameMode->PawnKilled(this);
		}

		// Remove controller from ShooterCharacter.  Results in no longer being able to attack, mover, etc. 
		DetachFromControllerPendingDestroy();

		// Turn off capsule collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	else
	{
		Health -= DamageToApply;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
	return DamageToApply;
}

bool APlayerCharacter::IsDead() const
{
	return bIsDead;
}

float APlayerCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float APlayerCharacter::GetCurrentHealth() const
{
	return Health;
}

float APlayerCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void APlayerCharacter::PullTrigger()
{
	ABaseWeapon_Gun* GunWeapon = Cast<ABaseWeapon_Gun>(ActiveWeapon); 

	if (GunWeapon != nullptr)
	{
		GunWeapon->PullTrigger(); 
	}
}

void APlayerCharacter::ActionButtonPressed()
{
	if (bIsAttacking == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->ActionButtonPressed()-> bIsAttacking == true"));
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bCanAttack && bListeningForComboInput && ComboCount < ComboMax)
	{
		if (PunchMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(PunchMontage, 1.f);

			if (MontageDuration > 0.f)
			{
				ComboCount += 1;
				LightAttackNumber += 1;

				FString AttackNumber = FString::FromInt(LightAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, PunchMontage);
			}
			
		}
	}
	else if (bCanAttack && bIsAttacking == false)
	{
		if (PunchMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(PunchMontage, 1.f);

			if (MontageDuration > 0.f)
			{
				LightAttackNumber = 1;
				HeavyAttackNumber = 1;
				ComboCount = 0;
				FString AttackNumber = FString::FromInt(LightAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				bIsAttacking = true;

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, PunchMontage);
			}
		}
	}
}

void APlayerCharacter::ActionButtonReleased()
{
}

void APlayerCharacter::HeavyAttackPressed()
{
	if (bIsAttacking == true)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bCanAttack && bListeningForComboInput && ComboCount < ComboMax)
	{
		if (KickMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(KickMontage, 1.f);
			UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->HeavyAttackPressed()-> KickMontage, AnimInstance"));
			if (MontageDuration > 0.f)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->HeavyAttackPressed()-> MontageDuration"));
				ComboCount += 1;
				HeavyAttackNumber += 1;

				FString AttackNumber = FString::FromInt(HeavyAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				bIsAttacking = true;

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, KickMontage);
			}

		}
	}
	else if (bCanAttack && bIsAttacking == false)
	{
		if (KickMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(KickMontage, 1.f);

			if (MontageDuration > 0.f)
			{
				LightAttackNumber = 1;
				HeavyAttackNumber = 1;
				ComboCount = 0;
				FString AttackNumber = FString::FromInt(HeavyAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				bIsAttacking = true;

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, KickMontage);
			}
		}
	}
}

void APlayerCharacter::HeavyAttackReleased()
{
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRateLookUp * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRateLookUp * GetWorld()->GetDeltaSeconds());
	
	
}

void APlayerCharacter::SwapWeapon()
{
	switch (WeaponEquippedStatus)
	{
	case EWeaponEquipedStatus::EWES_Unarmed:
		MainWeapon->EquipWeapon(this);
		SetWeaponEquippedStatus(EWeaponEquipedStatus::EWES_MainEquipped);
		ActiveWeapon = MainWeapon;
		ActiveWeapon->SetOwner(this);
		break;
	case EWeaponEquipedStatus::EWES_MainEquipped:
		MainWeapon->UnequipWeapon(this);
		SecondaryWeapon->EquipWeapon(this); 
		SetWeaponEquippedStatus(EWeaponEquipedStatus::EWES_SecondaryEquipped);
		ActiveWeapon = SecondaryWeapon;
		ActiveWeapon->SetOwner(this);
		break;
	case EWeaponEquipedStatus::EWES_SecondaryEquipped:
		SecondaryWeapon->UnequipWeapon(this);
		SetWeaponEquippedStatus(EWeaponEquipedStatus::EWES_Unarmed);
		SetPlayerStance(EPlayerStance::EPS_Unarmed);
		ActiveWeapon = nullptr;
		//ActiveWeapon->SetOwner(nullptr); 
		break;
	case EWeaponEquipedStatus::EWES_MAX:
		break;
	default:
		break;
	}

	AController* OwnerController = GetController();

	if (OwnerController != nullptr)
	{
		ASimpleShooterPlayerController* PlayerController = Cast<ASimpleShooterPlayerController>(OwnerController);

		if (PlayerController)
		{
			PlayerController->DisplayWeaponHUD(ActiveWeapon);
		}
	}

}


void APlayerCharacter::SetPlayerStance(EPlayerStance Stance)
{
	PlayerStance = Stance; 
}

EPlayerStance APlayerCharacter::GetPlayerStance()
{
	return PlayerStance;
}

void APlayerCharacter::SetWeaponEquippedStatus(EWeaponEquipedStatus Status)
{
	WeaponEquippedStatus = Status; 
}

EWeaponEquipedStatus APlayerCharacter::GetWeaponEquippedStatus()
{
	return WeaponEquippedStatus;
}

