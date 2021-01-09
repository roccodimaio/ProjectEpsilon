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

UENUM(BlueprintType)
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

	/*** COMPONENTS ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* RightHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* LeftHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* RightFootCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* LeftFootCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SkillOneSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SkillTwoSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* RadarSphereCollisiion;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*** VARIABLES ***/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseWeapon* ActiveWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsingSkill01 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bListeningForComboInput = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LightAttackNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HeavyAttackNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ComboCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ComboMax = 6;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* PunchMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* KickMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* KatanaMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HeavyKatanaMontage;

	UPROPERTY(EditAnywhere)
	float RadarSphereRadius = 1000.f; 

	float Skill01MaxRange = 10000.f;

	bool bGetRadarInput = false; 

	/** Missile blueprint class to fire */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseMissile> MissileClass;

	/*********************************************************************************/
	/*** PLAYER STATS ***/
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	float UnarmedDamage = 10.f;

	
	/*** FUNCTIONS ***/
	void SetPlayerStance(EPlayerStance Stance);

	UFUNCTION(BlueprintPure)
	EPlayerStance GetPlayerStance();

	void SetWeaponEquippedStatus(EWeaponEquipedStatus Status);
	EWeaponEquipedStatus GetWeaponEquippedStatus();
	
	/** Function to take damage 
	*/
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

	/** Function to shoot equipped BaseGun */
	void PullTrigger();

	UFUNCTION(BlueprintCallable)
	void ActionButtonPressed();

	UFUNCTION(BlueprintCallable)
	void ActionButtonReleased();

	UFUNCTION(BlueprintCallable)
	void HeavyAttackPressed();

	UFUNCTION(BlueprintCallable)
	void HeavyAttackReleased();

	void Attack(UAnimMontage* AttackMontage); 

	void HeavyAttack(UAnimMontage* AttackMontage);

	void SkillAttack();


	/** Function for when Right hand begins overlap with collision volume */
	UFUNCTION()
	virtual void OnOverlapBeginRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function for when Right hand ends overlap with collision volume*/
	UFUNCTION()
	virtual void OnOverlapEndRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Function for when Right hand begins overlap with collision volume */
	UFUNCTION()
	virtual void OnOverlapBeginLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function for when Right hand ends overlap with collision volume*/
	UFUNCTION()
	virtual void OnOverlapEndLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Function for when Right hand begins overlap with collision volume */
	UFUNCTION()
	virtual void OnOverlapBeginRightFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function for when Right hand ends overlap with collision volume*/
	UFUNCTION()
	virtual void OnOverlapEndRightFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Function for when Right hand begins overlap with collision volume */
	UFUNCTION()
	virtual void OnOverlapBeginLeftFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function for when Right hand ends overlap with collision volume*/
	UFUNCTION()
	virtual void OnOverlapEndLeftFoot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void EnableRightHandCollision();

	UFUNCTION(BlueprintCallable)
	void DisableRightHandCollision();

	UFUNCTION(BlueprintCallable)
	void EnableLeftHandCollision();

	UFUNCTION(BlueprintCallable)
	void DisableLeftHandCollision();

	UFUNCTION(BlueprintCallable)
	void EnableRightFootCollision();

	UFUNCTION(BlueprintCallable)
	void DisableRightFootCollision();

	UFUNCTION(BlueprintCallable)
	void EnableLeftFootCollision();

	UFUNCTION(BlueprintCallable)
	void DisableLeftFootCollision();

	UFUNCTION(BlueprintCallable)
	void GetEnemiesWithinRadar();
		
	void Skill02ButtonPressed();

	void Skill02ButtonReleased();

private:

	/*** VARIABLES ***/
	UPROPERTY(EditAnywhere)
	float RotationRateRight = 10.f;

	UPROPERTY(EditAnywhere)
	float RotationRateLookUp = 10.f;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WeaponHUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileSkillBase> ProjectileSkillClass;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class ABaseMissile> MissileSkillClass;

	UPROPERTY(EditAnywhere)
	UUserWidget* EquippedWeaponHUD;

	class UAnimInstance* OwnerAnimInstance;

	class UPlayerCharacterAnimInstance* PlayerAnimInstance;

	FVector ViewPointLocation;

	FRotator ViewPointRotation;

	ABaseAICharacter* BaseAICharacterTarget = nullptr;

	bool bProcessingRadarData = false;
	
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
