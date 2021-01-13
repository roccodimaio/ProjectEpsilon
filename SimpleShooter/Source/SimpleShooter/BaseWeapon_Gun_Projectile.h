// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseWeapon_Gun_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ABaseWeapon_Gun_Projectile : public ABaseWeapon
{
	GENERATED_BODY()

public:

	ABaseWeapon_Gun_Projectile();

	/*** COMPONENTS ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	/*** FUNCTIONS ***/
	void PullTrigger();

	

private:

	/*** VARIABLES ***/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileType", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlashParticleSystem;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticleSystem;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	FVector ViewPointLocation;

	FRotator ViewPointRotation;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;
};
