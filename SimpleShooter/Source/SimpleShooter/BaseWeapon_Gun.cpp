// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon_Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacter.h"
#include "BaseAICharacter.h"

ABaseWeapon_Gun::ABaseWeapon_Gun()
{

}

void ABaseWeapon_Gun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->PullTrigger()"));
	
	// Spawn particle effect for muzzle flash,  Use SpawnEmitterAttached so particle stays attached to Gun
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystem, Mesh, TEXT("MuzzleFlashSocket"));

	// Play sound at location of the Gun
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	// Assign values to Hit and ShotDirection by calling the function GunTrace
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->PullTrigger()->bSuccess"));
		// Direction of shot
		ShotDirection = -ViewPointRotation.Vector();

		// Spawn particle system at location bullet impacts an object
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticleSystem, Hit.Location, ShotDirection.Rotation());

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* ActorHit = Hit.GetActor();

		if (ActorHit != nullptr)
		{
			// Create DamageEvent
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

			AController* OwnerController = GetOwnerController();

			// Call TakeDamage on Actor that was hit
			ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);

		}

		//Draw debut point to show point of bullet impact (location of object hit)
		//DrawDebugPoint(GetWorld(), OutHit.Location, 20, FColor::Red, true);
	}

	// Draw debug view port
	//DrawDebugCamera(GetWorld(), ViewPointLocation, ViewPointRotation, 90.0f, 2, FColor::Red, true);

}

bool ABaseWeapon_Gun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->GunTrace()"));
	
	AActor* HitActor = Hit.GetActor();

	ABaseAICharacter* BaseAICharacter = Cast<ABaseAICharacter>(HitActor);

	AController* OwnerController = GetOwnerController();

	if (OwnerController == nullptr) return false;

	UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->GunTrace()->OwnerController found"));

	// Call GetplayerViewPoint and store the Viewpoint location and rotation into variables
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	// Direction of shot
	ShotDirection = -ViewPointRotation.Vector();

	// Calculate end point of line trace (distance bullet can travel from the camera)
	FVector End = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;

	// Ignore the Gun class when line tracing
	Params.AddIgnoredActor(this);

	// Ignore the Owner of the Gun class when line tracing
	Params.AddIgnoredActor(GetOwner());

	if (BaseAICharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->GunTrace()->BaseAICharacter True"));
		
		USphereComponent* SphereComponent;

		SphereComponent = BaseAICharacter->AttackRangeSphereComponent;

		Params.AddIgnoredComponent(SphereComponent);
	}

	// LineTrace on custom LineTrace (Bullet = ECC_GameTraceChanel1) and store object hit into OutHit  
	// If successfully hit something return true
	return GetWorld()->LineTraceSingleByChannel(Hit, ViewPointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

/**
AController* ABaseWeapon_Gun::GetOwnerController() const
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->GunTrace()->GetOwnerController()"));

	// Find owner of Gun
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	// If owner is nullptr return
	if (OwnerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->GunTrace()->GetOwnerController()->OwnerPawn = nullptr"));
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon_Gun->GunTrace()->GetOwnerController()->OwnerPawn found"));

	// Controller of owner found
	return OwnerPawn->GetController();
}
*/
