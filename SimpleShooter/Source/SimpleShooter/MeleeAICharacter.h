// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "MeleeAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AMeleeAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()
	

public:

	AMeleeAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/*** FUNCTIONS ***/

	/** Function to enable collision on Right hand */
	UFUNCTION(BlueprintCallable)
	void EnableRightHandCollision();

	/** Function to disable collision on Right hand */
	UFUNCTION(BlueprintCallable)
	void DisableRightHandCollision();

	/** Function to enable collision on Left hand */
	UFUNCTION(BlueprintCallable)
	void EnableLefttHandCollision();

	/** Function to disable collision on Left hand */
	UFUNCTION(BlueprintCallable)
	void DisableLeftHandCollision();

	/** Function for when Right hand begins overlap with collision volume */
	UFUNCTION()
	virtual void OnOverlapBeginRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function for when Right hand ends overlap with collision volume*/
	UFUNCTION()
	virtual void OnOverlapEndRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere)
	class UBoxComponent* RightHandCollision;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* LeftHandCollision;

	FVector ViewPointLocation;

	FRotator ViewPointRotation;

};
