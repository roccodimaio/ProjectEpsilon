// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class SIMPLESHOOTER_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	/*** COMPONENTS ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* TurretMesh;

	/*** VARIABLES ***/

	/** Missile blueprint class to fire */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseMissile> MissileClass;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*** FUNCTIONS ***/
	void InitMissileLaunch();
	void SpawnMissile();



private:
	
	/** Timer handle for repeating missile fire */
	FTimerHandle MissileHandle;



};
