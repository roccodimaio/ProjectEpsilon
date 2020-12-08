// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
#include "SimpleShooterGameModeBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "BaseAIController.h"
#include "Animation/AnimInstance.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere Component"));
	AttackRangeSphereComponent->SetupAttachment(GetRootComponent()); 

}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	AttackRangeSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackRangeSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	AttackRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseAICharacter::OnOverlapBeginAttackSphere);
	AttackRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseAICharacter::OnOverlapEndAttackSphere);

}

// Called every frame
void ABaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("BaseAICharacter->DamageTaken"));
	
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Health - DamageToApply <= 0.0f)
	{
		Health = 0;
		
		bIsDead = true;

		//UE_LOG(LogTemp, Warning, TEXT("BaseAICharacter->TakeDamage->Health <= 0 ->bIsDead %s"), bIsDead ? TEXT("True") : TEXT("False"));

		// Store reference to the SimpleShooterGameModeBase
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (GameMode != nullptr)
		{
			// GameMode will handle events after Character is killed (score, subtract lives, track wave numbers, etc.)
			GameMode->PawnKilled(this);
		}

		
		// Remove controller from Character.  Results in no longer being able to attack, move, etc. 
		DetachFromControllerPendingDestroy(); 
		
		// Turn off capsule collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackRangeSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	}
	else
	{
		Health -= DamageToApply;
	}

	return DamageToApply;
}

bool ABaseAICharacter::IsDead() const
{
	return bIsDead;
}

float ABaseAICharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float ABaseAICharacter::GetCurrentHealth() const
{
	return Health;
}

float ABaseAICharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ABaseAICharacter::OnOverlapBeginAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsWithinAttackRange = true; 
	
	// TODO Function to return AIController
	AController* AIController = GetController();
	ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController); 

	if (BaseAIController)
	{
		BaseAIController->SetIsWithinAttackRange(bIsWithinAttackRange);
	}
}

void ABaseAICharacter::OnOverlapEndAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsWithinAttackRange = false; 

	// TODO Function to return AIController
	AController* AIController = GetController();
	ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);

	if (BaseAIController)
	{
		BaseAIController->SetIsWithinAttackRange(bIsWithinAttackRange);
	}
}

void ABaseAICharacter::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); 

	if (AttackMontage && AnimInstance)
	{
		float MontageDuration = AnimInstance->Montage_Play(AttackMontage, 1.f);

		if (MontageDuration > 0.f)
		{
			int32 Attack = FMath::RandRange(1, NumberOfAttacks);
			
			FString AttackName = FString(TEXT("Attack_"));

			FString AttackSuffex = FString::FromInt(Attack);

			UE_LOG(LogTemp, Warning, TEXT("Attack # from RandRange %s"), *AttackSuffex);

			AttackName.Append(AttackSuffex);

			FName ConverteredAttackName(AttackName);

			AnimInstance->Montage_JumpToSection(ConverteredAttackName, AttackMontage);
		}
	}
}

