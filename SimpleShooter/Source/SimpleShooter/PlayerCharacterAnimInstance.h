// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APlayerCharacter* PlayerCharacter = nullptr; 

};
