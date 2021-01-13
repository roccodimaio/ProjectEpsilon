// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	/*** VARIABLES ***/

	/** Particle to play when projectile is spawned */
	UPROPERTY(EditAnywhere)
	class UParticleSystem* LaunchParticleSystem;

	/*** COMPONENTS ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/** Particle to play when projectile is in flight */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleTrail;

	/** Test of NiagaraSystem */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UNiagaraSystem* NiagaraEffect;

	/*** FUNCTIONS ***/
	UFUNCTION()
	void FireInDirection(const FVector& FireDirection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/** VARIABLES */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShake> HitShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ProjectileMovementSpeed = 1300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float ProjectileDamage = 50.f;

	/** Particles to play when projectile collides */
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticles;

	/** Sound to play when projectile collides*/
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	/** Sound to play when projectile is spawned*/
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* LaunchSound;

	FVector ViewPointLocation;
	FRotator ViewPointRotation;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;

	class UNiagaraComponent* NiagaraSystemInstance;

	/** FUNCTIONS */

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit);
	
	bool TracePath(FHitResult& Hit, FVector& ShortDirection);
};
