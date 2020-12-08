// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*** FUNCTIONS ***/

	/** Function to take damage */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Function to check if AShooterCharacter is dead */
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/** Function to return current health percentage */
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	/** Function to return current health */
	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;

	/** Function to return max health */
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	/** Function to call when AttackRangeShereComponent is overlapped */
	UFUNCTION()
	virtual void OnOverlapBeginAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function for when AttackRangeSphereComonent ends overlap */
	UFUNCTION()
	virtual void OnOverlapEndAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void Attack(); 

	/*** VARIABLES ***/
	
	UPROPERTY(VisibleAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackVocalSound;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSwingSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticleSystem;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(VisibleAnywhere)
	bool bIsWithinAttackRange;

	UPROPERTY(EditAnywhere)
	float AttackRange = 50.f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfAttacks; 

	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere)
	class USphereComponent* AttackRangeSphereComponent;
};
