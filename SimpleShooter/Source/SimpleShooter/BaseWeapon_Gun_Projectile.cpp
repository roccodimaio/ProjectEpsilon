// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon_Gun_Projectile.h"
#include "BaseProjectile.h"

ABaseWeapon_Gun_Projectile::ABaseWeapon_Gun_Projectile()
{
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetRootComponent());
}

void ABaseWeapon_Gun_Projectile::PullTrigger()
{
	if (ProjectileClass)
	{
		FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		ABaseProjectile* TempProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation);

		TempProjectile->SetOwner(GetOwner());
	}
}
