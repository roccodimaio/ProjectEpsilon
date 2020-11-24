// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
#include "SimpleShooterGameModeBase.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AMeleeCharacter::AMeleeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionVolume"));
	RightHandCollision->SetupAttachment(GetMesh(), "RightHandSocket");
	
	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionVolume"));
	LeftHandCollision->SetupAttachment(GetMesh(), "LeftHandSocket");
}

// Called when the game starts or when spawned
void AMeleeCharacter::BeginPlay()
{
	Super::BeginPlay();

	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeCharacter::OnOverlapBeginRightHand);
	RightHandCollision->OnComponentEndOverlap.AddDynamic(this, &AMeleeCharacter::OnOverlapEndRightHand);
}

// Called every frame
void AMeleeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMeleeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMeleeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Health - DamageToApply <= 0.0f)
	{
		Health = 0;
		bIsDead = true;
		// Store reference to the SimpleShooterGameModeBase
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (GameMode != nullptr)
		{
			// GameMode will handle events after ShooterCharacter is killed (score, subtract lives, track wave numbers, etc.)
			GameMode->PawnKilled(this);
		}

		// Remove controller from ShooterCharacter.  Results in no longer being able to attack, mover, etc. 
		DetachFromControllerPendingDestroy();

		// Turn off capsule collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	else
	{
		Health -= DamageToApply;
	}

	return DamageToApply;
}

bool AMeleeCharacter::IsDead() const
{
	return bIsDead;
}

float AMeleeCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

float AMeleeCharacter::GetCurrentHealth() const
{
	return Health;
}

float AMeleeCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void AMeleeCharacter::AttackRight()
{
	//TODO Play Attack Animation
	UE_LOG(LogTemp, Warning, TEXT("MeleeCharacter->Attack"))
}

void AMeleeCharacter::EnableRightHandCollision()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeCharacter::DisableRightHandCollision()
{
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeCharacter::EnableLefttHandCollision()
{
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeCharacter::DisableLeftHandCollision()
{
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeCharacter::OnOverlapBeginRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeCharacter->OnOverLapBegin"))
}

void AMeleeCharacter::OnOverlapEndRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeCharacter->OnOverLapBegin"))
}

