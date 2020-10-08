// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGunBase;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	/** VARIABLES */
	UPROPERTY(EditAnywhere)
	float RotationRateUp = 10.f;

	UPROPERTY(EditAnywhere)
	float RotationRateRight = 10.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGunBase> GunClass;

	UPROPERTY()
	AGunBase* Gun; 
	
	
	/** FUNCTIONS */
	void MoveForward(float AxisValue); /** Move Forward / Backward with mouse or controller */

	void MoveRight(float AxisValue);  /** Move Right / Left with mouse or controller */

	void LookUpRate(float AxisValue); /** Look Up / Down with controller */

	void LookRightRate(float AxisValue); /** Look Right / Left with controller */

	void Shoot(); 

};
