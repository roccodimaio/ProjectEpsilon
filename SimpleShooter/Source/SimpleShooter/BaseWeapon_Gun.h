// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseWeapon_Gun.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ABaseWeapon_Gun : public ABaseWeapon
{
	GENERATED_BODY()

public:

	ABaseWeapon_Gun();

	/*** FUNCTIONS ***/
	void PullTrigger();

private:

	/*** VARIABLES ***/

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

	/*** FUNCTIONS ***/
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	//AController* GetOwnerController() const;
	
};
