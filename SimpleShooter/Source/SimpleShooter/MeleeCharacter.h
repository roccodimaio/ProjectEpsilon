// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AMeleeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMeleeCharacter();


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

	/** Function to Attack */
	void AttackRight();

	UFUNCTION(BlueprintCallable)
	void EnableRightHandCollision();

	UFUNCTION(BlueprintCallable)
	void DisableRightHandCollision();

	UFUNCTION(BlueprintCallable)
	void EnableLefttHandCollision();

	UFUNCTION(BlueprintCallable)
	void DisableLeftHandCollision();

	UFUNCTION()
	virtual void OnOverlapBeginRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEndRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	
	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere)
	class UBoxComponent* RightHandCollision; 

	UPROPERTY(EditAnywhere)
	class UBoxComponent* LeftHandCollision;

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
};