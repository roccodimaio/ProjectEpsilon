// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EPS_Katana UMETA(DisplayName = "Katana"),
	EPS_Rifle UMETA(DisplayName = "Rifle"),
	EPS_Pistol UMETA(DisplayName = "Pistol"),
	EPS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class SIMPLESHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	/*** COMPONENTS ***/
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FName UnequippedSocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*** VARIABLES ***/
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType = EWeaponType::EPS_MAX;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	
	/*** FUNCTIONS ***/
	UFUNCTION()
	void SetWeaponType(EWeaponType Type);

	UFUNCTION()
	EWeaponType GetWeaponType();

	void EquipWeapon(ACharacter* Character);

	void UnequipWeapon(ACharacter* Character);

	void SwapWeapon(ACharacter* Character);

	

private:

	/*** COMPONENTS ***/
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
		
};
