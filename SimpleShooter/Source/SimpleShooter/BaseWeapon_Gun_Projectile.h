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

	/*** VARIABLES ***/
	FTimerHandle FireTimerHandle;
	
	/*** FUNCTIONS ***/
	void PullTrigger();

	void ReleaseTrigger();

	void Fire();

	virtual FVector GetAdjustedAim() const;

	/** get the muzzle location of the weapon */
	FVector GetMuzzleLocation() const;

	/** get direction of weapon's muzzle */
	FVector GetMuzzleDirection() const;

	/** get the originating location for camera damage */
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;

	/** find hit */
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	bool ProjectileTrace(FHitResult& Hit);

	

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

	UPROPERTY(EditAnywhere)
	float ShotsPerSecond = 10.f;

	UPROPERTY(EditAnywhere)
	bool bIsAutomaticFire = false; 
};
