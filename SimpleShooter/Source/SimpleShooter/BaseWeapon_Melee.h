// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseWeapon_Melee.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ABaseWeapon_Melee : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ABaseWeapon_Melee();

private:
	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionComponent;

	
};
