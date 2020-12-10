// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseWeapon.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	EPS_Unarmed UMETA(DisplayName = "Unarmed"),
	EPS_Katana UMETA(DisplayName = "Katana"),
	EPS_Rifle UMETA(DisplayName = "Rifle"),
	EPS_Pistol UMETA(DisplayName = "Pistol"),
	EPS_MAX UMETA(DisplayName = "DefaultMax")
};

enum class EWeaponEquipedStatus : uint8
{
	EWES_Unarmed UMETA(DisplayName = "Unarmed"),
	EWES_MainEquipped UMETA(DisplayName = "MainEquipped"),
	EWES_SecondaryEquipped UMETA(DisplayName = "SecondaryEquipped"),
	EWES_MAX UMETA(DisplayName = "DefaultMax")
};
UCLASS()
class SIMPLESHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*** VARIABLES ***/

	
	/*** FUNCTIONS ***/
	void SetPlayerStance(EPlayerStance Stance);
	EPlayerStance GetPlayerStance();

	void SetWeaponEquippedStatus(EWeaponEquipedStatus Status);
	EWeaponEquipedStatus GetWeaponEquippedStatus();
	
	/** Function to take damage */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Function to check if PlayerCharacter is dead */
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/** Function to check if PlayerCharacter is dead */
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	/** Function to get current health */
	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

private:

	/*** VARIABLES ***/
	UPROPERTY(EditAnywhere)
	float RotationRateRight = 10.f;

	UPROPERTY(EditAnywhere)
	float RotationRateLookUp = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	bool bIsDead = false;
		
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseWeapon> MainWeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseWeapon> SecondaryWeaponClass;

	UPROPERTY(EditAnywhere)
	ABaseWeapon* MainWeapon = nullptr;

	UPROPERTY(EditAnywhere)
	ABaseWeapon* SecondaryWeapon = nullptr;
		
	EPlayerStance PlayerStance = EPlayerStance::EPS_Unarmed;

	EWeaponEquipedStatus WeaponEquippedStatus = EWeaponEquipedStatus::EWES_Unarmed; 

	FName MainSocketName;

	FName SecondarySocketName; 
	
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<class AGun> GunClass;

	//UPROPERTY(EditAnywhere)
	//AGun* EquippedGun = nullptr;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<class ABaseMeleeWeapon> MeleeClass; 

	//UPROPERTY(EditAnywhere)
	//ABaseMeleeWeapon* EquippedMelee = nullptr; 

	

	/*** FUNCTIONS ***/
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void SwapWeapon();
	
};
