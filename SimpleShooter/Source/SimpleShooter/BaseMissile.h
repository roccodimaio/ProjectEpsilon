// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMissile.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABaseMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseMissile();

	/*** COMPONENTS ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MissileMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovementComponent; 


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Reference to player in world */
	class APlayerCharacter* PlayerCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*** VARIABLES ***/
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ExplosionParticleSystem;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	float UpwardsLaunchDistance;

	UPROPERTY(EditAnywhere)
	float MissileHomingTightness;

	UPROPERTY(EditAnywhere)
	float MissileTurnSensativity;

	UPROPERTY(EditAnywhere)
	float MissileSpeed;

	UPROPERTY(EditAnywhere)
	float MissileBoosterDelay;

	/*** FUNCTIONS ***/
	void DelayLogic(float dTime);

	void FindPlayer();
	void SetTarget(AActor* Actor);
	void UpdateTarget();

	class UParticleSystemComponent* PlayExplosion(class UParticleSystem* ParticleSystem);

	class UAudioComponent* PlayExplosionSound(class USoundBase* Sound);

private: 
	
	/*** VARIABLES ***/
	bool bHasTargetPosition; 
	bool bHasNoTarget;
	class AActor* target;

	float DelayTimer;
	bool bHasFinishedDelay;

	float LifeTimeCountdown;
	bool bCanBeDestroyed;
	
	/*** FUNCTIONS ***/
	void Explode();

	/** Function for when BaseMille overlaps with collision volume */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
