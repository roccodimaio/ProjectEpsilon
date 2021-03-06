// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"


void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner(); // Check what Pawn owns this AnimInstance and assign to Pawn

		if (Pawn != nullptr)
		{
			APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(Pawn); 
			
			if (OwnerCharacter)
			{
				RefToPlayerCharacter = OwnerCharacter;
			}
		}

	}

}


