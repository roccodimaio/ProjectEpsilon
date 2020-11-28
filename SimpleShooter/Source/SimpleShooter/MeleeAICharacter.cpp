// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAICharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
#include "SimpleShooterGameModeBase.h"
#include "Engine/SkeletalMeshSocket.h"

AMeleeAICharacter::AMeleeAICharacter()
{
	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionVolume"));
	RightHandCollision->SetupAttachment(GetMesh(), "RightHandSocket");

	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionVolume"));
	LeftHandCollision->SetupAttachment(GetMesh(), "LeftHandSocket");

	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeAICharacter::BeginPlay()
{
	Super::BeginPlay();

	//RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAICharacter::OnOverlapBeginRightHand);
	RightHandCollision->OnComponentEndOverlap.AddDynamic(this, &AMeleeAICharacter::OnOverlapEndRightHand);
}

void AMeleeAICharacter::AttackRight()
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeAICharacter->AttackRight")); 
}

void AMeleeAICharacter::EnableRightHandCollision()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeAICharacter::DisableRightHandCollision()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeAICharacter::EnableLefttHandCollision()
{
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeAICharacter::DisableLeftHandCollision()
{
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeAICharacter::OnOverlapBeginRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeCharacter->OnOverLapBeginRightHand"));

	if (OtherActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeCharacter->OnOverLapBeginRightHand->Other Actor is valid"));
		
		AShooterCharacter* MainCharacter = Cast<AShooterCharacter>(OtherActor);

		if (MainCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("MeleeCharacter->OnOverLapBeginRightHand->OtherActor is MainCharacter"));
			
			AController* OwnerController = GetController();

			OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

			FPointDamageEvent DamageEvent(Damage, SweepResult, -ViewPointRotation.Vector(), nullptr);

			MainCharacter->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

void AMeleeAICharacter::OnOverlapEndRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeCharacter->OnOverLapEndRightHand"))
}
