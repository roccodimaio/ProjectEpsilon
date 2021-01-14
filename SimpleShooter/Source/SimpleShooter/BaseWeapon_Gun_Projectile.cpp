// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon_Gun_Projectile.h"
#include "BaseProjectile.h"
#include "TimerManager.h"

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
		// Calculate the time between calling Fire() if bIsAutomatic is true
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABaseWeapon_Gun_Projectile::Fire, ShotsPerSecond, true, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->PullTrigger - Automatic = False"));
		Fire();
	}
	
	
	/**
	if (ProjectileClass)
	{
		FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		ABaseProjectile* TempProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation);

		TempProjectile->SetOwner(GetOwner());
	}
	*/
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
	UE_LOG(LogTemp, Warning, TEXT("BaseWeapon_Gun_Projectile->Fire"));
	if (ProjectileClass)
	{
		FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		ABaseProjectile* TempProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation);

		TempProjectile->SetOwner(GetOwner());
	}
}
