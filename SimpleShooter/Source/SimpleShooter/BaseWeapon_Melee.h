// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseWeapon_Melee.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ABaseWeapon_Melee : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ABaseWeapon_Melee();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionComponent;

public:


	/*** VARIABLES ***/
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticleSystem;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	FVector ViewPointLocation;

	FRotator ViewPointRotation;

	/** Function for when Right hand begins overlap with collision volume */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function for when Right hand ends overlap with collision volume*/
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void EnableCollision();

	UFUNCTION(BlueprintCallable)
	void DisableCollision();

};
