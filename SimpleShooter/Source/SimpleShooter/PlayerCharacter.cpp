// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Gun.h"
#include "BaseMeleeWeapon.h"
#include "BaseWeapon.h"
#include "BaseWeapon_Gun.h"
#include "BaseWeapon_Gun_Projectile.h"
#include "Blueprint/UserWidget.h"
#include "SimpleShooterPlayerController.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "BaseAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ProjectileSkillBase.h"
#include "BaseMissile.h"
#include "TimerManager.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionVolume"));
	RightHandCollision->SetupAttachment(GetMesh(), "RightHandSocket");

	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(GetMesh(), "LeftHandSocket");

	RightFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootCollision"));
	RightFootCollision->SetupAttachment(GetMesh(), "RightFootSocket");

	LeftFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootCollision"));
	LeftFootCollision->SetupAttachment(GetMesh(), "LeftFootSocket");

	SkillOneSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SkillOneSpawnPoint"));
	SkillOneSpawnPoint->SetupAttachment(GetMesh());

	SkillTwoSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SkillTwoSpawnPoint"));
	SkillTwoSpawnPoint->SetupAttachment(GetMesh());

	RadarSphereCollisiion = CreateDefaultSubobject<USphereComponent>(TEXT("RadarSphereCollision"));
	RadarSphereCollisiion->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RadarSphereCollisiion->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBeginRightHand);
	RightHandCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEndRightHand);

	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBeginLeftHand);
	LeftHandCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEndLeftHand); 

	RightFootCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBeginRightFoot);
	RightFootCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEndRightFoot);

	LeftFootCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBeginLeftFoot);
	LeftFootCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEndLeftFoot);

	Health = MaxHealth;

	PlayerStance = EPlayerStance::EPS_Unarmed;

	if (MainWeaponClass != NULL)
	{
		// Spawn MainWeapon
		MainWeapon = GetWorld()->SpawnActor<ABaseWeapon>(MainWeaponClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("*** NO MAIN WEAPON CLASS SELECTED ***"));
	}
	
	if (MainWeapon != nullptr)
	{
		// Attach MainWeapon to the appropriate Socket
		MainWeapon->UnequipWeapon(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("*** NO MAIN WEAPON ***"));
	}
	
	if (SecondaryWeaponClass != NULL)
	{
		// Spawn Secondary Weapon
		SecondaryWeapon = GetWorld()->SpawnActor<ABaseWeapon>(SecondaryWeaponClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("*** NO SECONDARY WEAPON CLASS SELECTED ***"));
	}
	
	if (SecondaryWeapon != nullptr)
	{
		// Attach SecondaryWeapon to the appropriate Socket
		SecondaryWeapon->UnequipWeapon(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("*** NO SECONDARY WEAPON ***"));
	}
	
	OwnerAnimInstance = GetMesh()->GetAnimInstance();

	if (OwnerAnimInstance != nullptr)
	{
		UPlayerCharacterAnimInstance* VerifyAnimInstance = Cast<UPlayerCharacterAnimInstance>(OwnerAnimInstance);

		if (VerifyAnimInstance)
		{
			PlayerAnimInstance = VerifyAnimInstance;
		}
	}

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

	if ((bGetRadarInput == true) && (bProcessingRadarData == false))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Tick -> bGetRadarInput"));
		GetEnemiesWithinRadar();
	}

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
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), IE_Released, this, &APlayerCharacter::ReleaseTrigger);
	PlayerInputComponent->BindAction(TEXT("ActionButton"), IE_Pressed, this, &APlayerCharacter::ActionButtonPressed);
	PlayerInputComponent->BindAction(TEXT("ActionButton"), IE_Released, this, &APlayerCharacter::ActionButtonReleased);
	PlayerInputComponent->BindAction(TEXT("HeavyAttackButton"), IE_Pressed, this, &APlayerCharacter::HeavyAttackPressed);
	PlayerInputComponent->BindAction(TEXT("HeavyAttackButton"), IE_Released, this, &APlayerCharacter::HeavyAttackReleased);
	PlayerInputComponent->BindAction(TEXT("SkillButton"), IE_Pressed, this, &APlayerCharacter::SkillAttack);
	PlayerInputComponent->BindAction(TEXT("Skill02Button"), IE_Pressed, this, &APlayerCharacter::Skill02ButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Skill02Button"), IE_Released, this, &APlayerCharacter::Skill02ButtonReleased);

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
	ABaseWeapon_Gun_Projectile* GunWeapon = Cast<ABaseWeapon_Gun_Projectile>(ActiveWeapon);
	
	//ABaseWeapon_Gun* GunWeapon = Cast<ABaseWeapon_Gun>(ActiveWeapon); 

	if (GunWeapon != nullptr)
	{
		GunWeapon->PullTrigger(); 
		
		//GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABaseWeapon_Gun_Projectile::PullTrigger, 1 / ShotsPerSecond, true, -1.f);
	}
}

void APlayerCharacter::ReleaseTrigger()
{
	ABaseWeapon_Gun_Projectile* GunWeapon = Cast<ABaseWeapon_Gun_Projectile>(ActiveWeapon);

	//ABaseWeapon_Gun* GunWeapon = Cast<ABaseWeapon_Gun>(ActiveWeapon); 

	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->ReleaseTrigger"));

	if (GunWeapon != nullptr)
	{
		GunWeapon->ReleaseTrigger();
	}
}


void APlayerCharacter::ActionButtonPressed()
{
	if (bIsAttacking == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->ActionButtonPressed()-> bIsAttacking == true"));
		return;
	}

	switch (PlayerStance)
	{
	case EPlayerStance::EPS_Unarmed:
		Attack(PunchMontage);
		break;
	case EPlayerStance::EPS_Katana:
		Attack(KatanaMontage);
		break;
	case EPlayerStance::EPS_Rifle:
		if (ActiveWeapon != nullptr)
		{
			ABaseWeapon_Gun_Projectile* EquippedGun = Cast<ABaseWeapon_Gun_Projectile>(ActiveWeapon);
			//ABaseWeapon_Gun* EquippedGun = Cast<ABaseWeapon_Gun>(ActiveWeapon);

			if (EquippedGun)
			{
				EquippedGun->PullTrigger();
			}
		}
		break;
	case EPlayerStance::EPS_Pistol:
		break;
	case EPlayerStance::EPS_MAX:
		break;
	default:
		break;
	}

	/**
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
	*/
}

void APlayerCharacter::ActionButtonReleased()
{
	if (PlayerStance == EPlayerStance::EPS_Rifle)
	{
		ABaseWeapon_Gun_Projectile* GunWeapon = Cast<ABaseWeapon_Gun_Projectile>(ActiveWeapon);

		//ABaseWeapon_Gun* GunWeapon = Cast<ABaseWeapon_Gun>(ActiveWeapon); 

		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->ActionButtonReleased"));

		if (GunWeapon != nullptr)
		{
			GunWeapon->ReleaseTrigger();
		}
	}
}

void APlayerCharacter::HeavyAttackPressed()
{
	if (bIsAttacking == true)
	{
		return;
	}
	
	switch (PlayerStance)
	{
	case EPlayerStance::EPS_Unarmed:
		HeavyAttack(KickMontage);
		break;
	case EPlayerStance::EPS_Katana:
		HeavyAttack(HeavyKatanaMontage);
		break;
	case EPlayerStance::EPS_Rifle:
		
		break;
	case EPlayerStance::EPS_Pistol:
		break;
	case EPlayerStance::EPS_MAX:
		break;
	default:
		break;
	}
	
	/**
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
	*/
}

void APlayerCharacter::HeavyAttackReleased()
{
}

void APlayerCharacter::Attack(UAnimMontage* AttackMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AttackMontage == nullptr)
	{
		return; 
	}
	
	if (bCanAttack && bListeningForComboInput && ComboCount < ComboMax)
	{
		if (AttackMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(AttackMontage, 1.f);

			if (MontageDuration > 0.f)
			{
				ComboCount += 1;
				LightAttackNumber += 1;

				FString AttackNumber = FString::FromInt(LightAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, AttackMontage);
			}

		}
	}
	else if (bCanAttack && bIsAttacking == false)
	{
		if (AttackMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(AttackMontage, 1.f);

			if (MontageDuration > 0.f)
			{
				LightAttackNumber = 1;
				HeavyAttackNumber = 1;
				ComboCount = 0;
				FString AttackNumber = FString::FromInt(LightAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				bIsAttacking = true;

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, AttackMontage);
			}
		}
	}

}

void APlayerCharacter::HeavyAttack(UAnimMontage* AttackMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AttackMontage == nullptr)
	{
		return;
	}
	
	if (bCanAttack && bListeningForComboInput && ComboCount < ComboMax)
	{
		if (AttackMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(AttackMontage, 1.f);
			//UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->HeavyAttackPressed()-> KickMontage, AnimInstance"));
			if (MontageDuration > 0.f)
			{
				//UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->HeavyAttackPressed()-> MontageDuration"));
				ComboCount += 1;
				HeavyAttackNumber += 1;

				FString AttackNumber = FString::FromInt(HeavyAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				bIsAttacking = true;

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, AttackMontage);
			}

		}
	}
	else if (bCanAttack && bIsAttacking == false)
	{
		if (AttackMontage && AnimInstance)
		{
			float MontageDuration = AnimInstance->Montage_Play(AttackMontage, 1.f);

			if (MontageDuration > 0.f)
			{
				LightAttackNumber = 1;
				HeavyAttackNumber = 1;
				ComboCount = 0;
				FString AttackNumber = FString::FromInt(HeavyAttackNumber);
				FName ConvertedAttackNumber(AttackNumber);

				bIsAttacking = true;

				AnimInstance->Montage_JumpToSection(ConvertedAttackNumber, AttackMontage);
			}
		}
	}
}

void APlayerCharacter::SkillAttack()
{
	if (ProjectileSkillClass)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->SkillAttack->Class is valid"));
		
		FVector ProjectileSkillSpawnLocation = SkillOneSpawnPoint->GetComponentLocation();
		FRotator ProjectileSkillSpawnRotation = SkillOneSpawnPoint->GetComponentRotation();

		FVector TargetLocation = FVector(0.f);
		FVector AimDirection = FVector(0.f);

		FActorSpawnParameters SpawnParams;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		Params.AddIgnoredActor(MainWeapon);
		Params.AddIgnoredActor(SecondaryWeapon);
		//Params.AddIgnoredActor(this);

		SpawnParams.Owner = this;
		SpawnParams.Instigator = this; 
	

		AController* OwnerController = GetController();
		OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

		FHitResult Hit; 
		// Calculate end point of line trace (distance bullet can travel from the camera)
		FVector End = ViewPointLocation + ViewPointRotation.Vector() * Skill01MaxRange;

		GetWorld()->LineTraceSingleByChannel(Hit, ViewPointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

		if (Hit.bBlockingHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->SkillAttack->Hit.bBlockingHit"));
			TargetLocation = Hit.Location;
			AimDirection = (TargetLocation - ProjectileSkillSpawnLocation).GetSafeNormal();
			AimDirection.Normalize();
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->SkillAttack->Hit.bBlockingHit - ELSE"));
			TargetLocation = Hit.TraceEnd;
			AimDirection = (TargetLocation - ProjectileSkillSpawnLocation).GetSafeNormal();
			AimDirection.Normalize();
		}
			
		bIsUsingSkill01 = true; 
		
		AProjectileSkillBase* TempProjectile = GetWorld()->SpawnActor<AProjectileSkillBase>(ProjectileSkillClass, ProjectileSkillSpawnLocation, ProjectileSkillSpawnRotation, SpawnParams);

		if (TempProjectile->InitialParticleSystem)
		{
			UParticleSystemComponent* SpawnParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TempProjectile->InitialParticleSystem, ProjectileSkillSpawnLocation, ProjectileSkillSpawnRotation, 1.f);
		}

		if (TempProjectile)
		{
			//UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->SkillAttack->TempProjectile"));
			TempProjectile->SetOwner(this);
			
			// Ignore the Owner of the Gun class when line tracing
			TempProjectile->FireInDirection(AimDirection);
			TempProjectile->SetActorRotation(ViewPointRotation);
		}
	}
}

void APlayerCharacter::OnOverlapBeginRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ABaseAICharacter* EnemyCharacter = Cast<ABaseAICharacter>(OtherActor);

		if (EnemyCharacter)
		{
			
			const USkeletalMeshSocket* FistSocket = GetMesh()->GetSocketByName("FX_RightHand");
			if (EnemyCharacter->ImpactParticleSystem && FistSocket)
			{
					FVector SocketLocation = FistSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EnemyCharacter->ImpactParticleSystem, SocketLocation, FRotator(0.f), false); 
			}
			
			AController* OwnerController = GetController();
			OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
			FPointDamageEvent DamageEvent(UnarmedDamage, SweepResult, -ViewPointRotation.Vector(), nullptr);

			EnemyCharacter->TakeDamage(UnarmedDamage, FDamageEvent(), OwnerController, this);
		}
	}
}

void APlayerCharacter::OnOverlapEndRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APlayerCharacter::OnOverlapBeginLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ABaseAICharacter* EnemyCharacter = Cast<ABaseAICharacter>(OtherActor);

		if (EnemyCharacter)
		{
			AController* OwnerController = GetController();

			OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

			FPointDamageEvent DamageEvent(UnarmedDamage, SweepResult, -ViewPointRotation.Vector(), nullptr);

			EnemyCharacter->TakeDamage(UnarmedDamage, FDamageEvent(), OwnerController, this);
		}
	}
}

void APlayerCharacter::OnOverlapEndLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APlayerCharacter::OnOverlapBeginRightFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ABaseAICharacter* EnemyCharacter = Cast<ABaseAICharacter>(OtherActor);

		if (EnemyCharacter)
		{
			AController* OwnerController = GetController();

			OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

			FPointDamageEvent DamageEvent(UnarmedDamage, SweepResult, -ViewPointRotation.Vector(), nullptr);

			EnemyCharacter->TakeDamage(UnarmedDamage, FDamageEvent(), OwnerController, this);
		}
	}
}

void APlayerCharacter::OnOverlapEndRightFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APlayerCharacter::OnOverlapBeginLeftFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ABaseAICharacter* EnemyCharacter = Cast<ABaseAICharacter>(OtherActor);

		if (EnemyCharacter)
		{
			AController* OwnerController = GetController();

			OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

			FPointDamageEvent DamageEvent(UnarmedDamage, SweepResult, -ViewPointRotation.Vector(), nullptr);

			EnemyCharacter->TakeDamage(UnarmedDamage, FDamageEvent(), OwnerController, this);
		}
	}
}

void APlayerCharacter::OnOverlapEndLeftFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APlayerCharacter::EnableRightHandCollision()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void APlayerCharacter::DisableRightHandCollision()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::EnableLeftHandCollision()
{
	
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::DisableLeftHandCollision()
{
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::EnableRightFootCollision()
{
	RightFootCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::DisableRightFootCollision()
{
	RightFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::EnableLeftFootCollision()
{
	LeftFootCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::DisableLeftFootCollision()
{
	LeftFootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::GetEnemiesWithinRadar()
{
	
	bProcessingRadarData = true; 

	//UE_LOG(LogTemp, Warning, TEXT("GetEnemiesWithinRadar"));
	RadarSphereCollisiion->SetSphereRadius(RadarSphereRadius);

	TArray<AActor*> OverlappingActors;
	TArray<ABaseAICharacter*> OverlappingBaseAIActors;

	// Identify all actors within the RadarSphereCollision component and add to OverlappingActors array
	RadarSphereCollisiion->GetOverlappingActors(OverlappingActors);


	// Loop through OverlappingActors, cast to ABaseAICharacter and if successful add to OverlappingBaseAIActors
	for (AActor* Actor : OverlappingActors)
	{
		ABaseAICharacter* BaseAICharacter = Cast<ABaseAICharacter>(Actor);

		if (BaseAICharacter)
		{
			OverlappingBaseAIActors.Add(BaseAICharacter);
		}
	}
	
	ABaseAICharacter* ClosestAICharacter = nullptr;

	float currentClosestDistance = RadarSphereRadius;
	FVector MyLocation = GetActorLocation();
	

	// Loop through all OverlappingActors
	for (ABaseAICharacter* TempBaseAICharacter : OverlappingBaseAIActors)
	{
		FVector DeltaVector = TempBaseAICharacter->GetActorLocation() - MyLocation;
		FVector DistToTargetVector = DeltaVector;
		float Delta = DeltaVector.Size();
		float DotP = 0.f;
		
		DistToTargetVector.Normalize();

		DotP = FVector::DotProduct(GetActorForwardVector(), DistToTargetVector);
		UE_LOG(LogTemp, Warning, TEXT("DotP = %f"), DotP);

		if (Delta < currentClosestDistance && (DotP >= 0))
		{
			currentClosestDistance = Delta;
			ClosestAICharacter = TempBaseAICharacter;
		}

		/**
		float DotP = 0.f;
		//FVector DistanceToTarget = Actor->GetActorLocation() - GetActorLocation();

		float DistanceToTarget = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());

		//DistanceToTarget.Normalize();

		//DotP = FVector::DotProduct(DistanceToTarget, GetActorForwardVector());
		
		// if DotP is greater than 0 then Actor is in front of PlayerCharacter.  (0 = to the side, -1 = behind)
		if (DotP > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("DotP >0"));
			
			float Distance = FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation());
			
			if (Distance < currentClosestDistance)
			{
				currentClosestDistance = Distance;
				ClosestAICharacter = Actor;
			}
		}

		*/
	
	}

	BaseAICharacterTarget = ClosestAICharacter;
	bProcessingRadarData = false; 
}

void APlayerCharacter::Skill02ButtonPressed()
{
	//UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter->Skill02ButtonPressed()"));
	bGetRadarInput = true;  
	RadarSphereCollisiion->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void APlayerCharacter::Skill02ButtonReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter->Skill02ButtonReleased()"));

	bGetRadarInput = false;
	RadarSphereCollisiion->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (MissileClass != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter->Skill02ButtonPressed()->MissileClass NOT NULL"));
		
		UWorld* const World = GetWorld();

		if (World != NULL && bProcessingRadarData == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter->Skill02ButtonPressed()->World NOT NULL"));
			FVector MissileSkillSpawnLocation = SkillTwoSpawnPoint->GetComponentLocation();
			FRotator MissileSkillSpawnRotation = SkillTwoSpawnPoint->GetComponentRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			ABaseMissile* FiredMissile = World->SpawnActor<ABaseMissile>(MissileClass, MissileSkillSpawnLocation, MissileSkillSpawnRotation, SpawnParams);

			if (BaseAICharacterTarget)
			{
				UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter->Skill02ButtonPressed()->Target is BaseAICharacter"));
				FiredMissile->SetTarget(BaseAICharacterTarget);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter->Skill02ButtonPressed()->Target is NOT BaseAICharacter"));
				FiredMissile->SetTarget(nullptr);
			}
			
			if (FiredMissile != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter->Skill02ButtonPressed()->MissileFired!"));
				FRotator MeshRot = FiredMissile->MissileMeshComponent->GetComponentRotation();
				MeshRot.Roll = 0.f;
				MeshRot.Pitch = 0.f;
				MeshRot.Yaw = 0.f;
				FiredMissile->MissileMeshComponent->SetRelativeRotation(MeshRot);
			}
		}
	}

	
	
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
	LightAttackNumber = 1;
	HeavyAttackNumber = 1;
	ComboCount = 0;

	bIsAttacking = false;
	bCanAttack = true; 

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
	
	switch (Stance)
	{
	case EPlayerStance::EPS_Unarmed:
		PlayerAnimInstance->bUnarmed = true;
		PlayerAnimInstance->bRifleEquipped = false; 
		PlayerAnimInstance->bKatanaEquipped = false; 
		PlayerAnimInstance->bPistolEquipped = false; 
		break;
	case EPlayerStance::EPS_Katana:
		PlayerAnimInstance->bUnarmed = false;
		PlayerAnimInstance->bRifleEquipped = false;
		PlayerAnimInstance->bKatanaEquipped = true;
		PlayerAnimInstance->bPistolEquipped = false;
		break;
	case EPlayerStance::EPS_Rifle:
		PlayerAnimInstance->bUnarmed = false;
		PlayerAnimInstance->bRifleEquipped = true;
		PlayerAnimInstance->bKatanaEquipped = false;
		PlayerAnimInstance->bPistolEquipped = false;
		break;
	case EPlayerStance::EPS_Pistol:
		PlayerAnimInstance->bUnarmed = false;
		PlayerAnimInstance->bRifleEquipped = false;
		PlayerAnimInstance->bKatanaEquipped = false;
		PlayerAnimInstance->bPistolEquipped = true;
		break;
	case EPlayerStance::EPS_MAX:
		break;
	default:
		break;
	}
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

