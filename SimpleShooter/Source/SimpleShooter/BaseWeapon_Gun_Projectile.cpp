// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon_Gun_Projectile.h"
#include "BaseProjectile.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SimpleShooterPlayerController.h"
#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"

ABaseWeapon_Gun_Projectile::ABaseWeapon_Gun_Projectile()
{
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetRootComponent());
	ShotsPerSecond = 1 / ShotsPerSecond;
}

void ABaseWeapon_Gun_Projectile::PullTrigger()
{
	if (bIsAutomaticFire == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->PullTrigger - Automatic = True"));
		// Call Fire() every ShotsPerSecond.  Loops until FireTimerHandle is cleared
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABaseWeapon_Gun_Projectile::Fire, ShotsPerSecond, true, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->PullTrigger - Automatic = False"));
		Fire();
	}
}

void ABaseWeapon_Gun_Projectile::ReleaseTrigger()
{
	if (bIsAutomaticFire)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->ReleaseTrigger"));
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
	
}

void ABaseWeapon_Gun_Projectile::Fire()
{
	
	if (ProjectileClass)
	{
		FVector ProjectileSkillSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSkillSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		FVector TargetLocation = FVector(0.f);
		FVector AimDirection = FVector(0.f);

		FActorSpawnParameters SpawnParams;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		//Params.AddIgnoredActor(this);

		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();


		AController* OwnerController = GetOwnerController();
		OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

		FHitResult Hit;
		// Calculate end point of line trace (distance bullet can travel from the camera)
		FVector End = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

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
			APawn* OwnerActor = Cast<APawn>(GetOwner());
			
			if (OwnerActor != nullptr)
			{
				AimDirection = OwnerActor->GetBaseAimRotation().Vector();
				AimDirection.Normalize();
			}
			
		}

		ABaseProjectile* TempProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSkillSpawnLocation, ProjectileSkillSpawnRotation, SpawnParams);
		
		if (TempProjectile)
		{
			TempProjectile->SetOwner(this);
			TempProjectile->FireInDirection(AimDirection);
		}
		

		
	}
	/**
	if (bSucess)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->Fire->bSuccess TRUE"));
		
		// Direction of shot
		ShotDirection = ViewPointRotation.Vector();
		
				ProjectileTransform.SetLocation(Hit.ImpactPoint);
		ProjectileTransform.SetRotation(ProjectileSpawnRotation.Quaternion());

		ABaseProjectile* TempProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, ProjectileTransform, GetOwner(), GetOwner()->GetInstigator());
		TempProjectile->SetOwner(this);
		if (TempProjectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->Fire-TempProjectile"));
			TempProjectile->ProjectileMovement->Velocity = Hit.ImpactPoint * TempProjectile->ProjectileMovement->InitialSpeed;
			UGameplayStatics::FinishSpawningActor(TempProjectile, TempProjectile->GetTransform());
		}
	}
	*/
	

}

FVector ABaseWeapon_Gun_Projectile::GetAdjustedAim() const
{
	ASimpleShooterPlayerController* const PlayerController = GetInstigatorController<ASimpleShooterPlayerController>();
	FVector FinalAim = FVector::ZeroVector;

	if (PlayerController)
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
	}
	else if (GetInstigator())  // if no PlayerController then Owner is AI
	{
		ABaseAIController* AIController = Cast<ABaseAIController>(GetOwnerController());

		if (AIController != NULL)
		{
			FinalAim = AIController->GetControlRotation().Vector();
		}
		else
		{
			FinalAim = GetInstigator()->GetBaseAimRotation().Vector();
		}
	}
	return FinalAim;
}

FVector ABaseWeapon_Gun_Projectile::GetMuzzleLocation() const
{
	return ProjectileSpawnPoint->GetComponentLocation();
}

FVector ABaseWeapon_Gun_Projectile::GetMuzzleDirection() const
{
	return ProjectileSpawnPoint->GetComponentRotation().Vector();
}

FVector ABaseWeapon_Gun_Projectile::GetCameraDamageStartLocation(const FVector& AimDir) const
{
	UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->GetCameraDamageStartLocation"));

	ASimpleShooterPlayerController* PlayerController = Cast<ASimpleShooterPlayerController>(GetOwnerController());
	ABaseAIController* AIPlayerController = Cast<ABaseAIController>(GetOwnerController());
	FVector OutStartTrace = FVector::ZeroVector;

	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->GetCameraDamageStartLocation->PlayerController"));
		FRotator UnusedRot;
		PlayerController->GetPlayerViewPoint(OutStartTrace, UnusedRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		OutStartTrace = OutStartTrace + AimDir * ((GetInstigator()->GetActorLocation() - OutStartTrace) | AimDir);
	}
	else if (AIPlayerController)
	{
		OutStartTrace = GetMuzzleLocation();
	}
	
	return OutStartTrace;
}

FHitResult ABaseWeapon_Gun_Projectile::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->WeaponTrace"));
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, ECollisionChannel::ECC_GameTraceChannel1, TraceParams);

	return Hit;
}

bool ABaseWeapon_Gun_Projectile::ProjectileTrace(FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->ProjectileTrace"));

	FVector ShotDirection;
	AActor* HitActor = Hit.GetActor();
	
	AController* OwnerController = GetOwnerController();

	// Call GetplayerViewPoint and store the Viewpoint location and rotation into variables
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	// Direction of shot
	ShotDirection = -ViewPointRotation.Vector();

	// Calculate end point of line trace (distance bullet can travel from the camera)
	FVector End = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;

	// Ignore the Gun class when line tracing
	Params.AddIgnoredActor(this);

	// Ignore the Owner of the Gun class when line tracing
	Params.AddIgnoredActor(GetOwner());

	AActor* WeaponOwner = GetOwner();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(WeaponOwner);

	if (PlayerCharacter)
	{
		USphereComponent* SphereComponent;
		SphereComponent = PlayerCharacter->RadarSphereCollisiion;
		Params.AddIgnoredComponent(SphereComponent);
	}

	// LineTrace on custom LineTrace(Bullet = ECC_GameTraceChanel1) and store object hit into OutHit
	// If successfully hit something return true
	return GetWorld()->LineTraceSingleByChannel(Hit, ViewPointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}
