// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(GetRootComponent());

}

void AGun::PullTrigger()
{
	//UE_LOG(LogTemp, Warning, TEXT("Gun->PullTrigger"));

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

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	
	AController* OwnerController = GetOwnerController();

	if (OwnerController == nullptr) return false; 

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

	// LineTrace on custom LineTrace (Bullet = ECC_GameTraceChanel1) and store object hit into OutHit  
	// If successfully hit something return true
	return GetWorld()->LineTraceSingleByChannel(Hit, ViewPointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	// Find owner of Gun
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	// If owner is nullptr return
	if (OwnerPawn == nullptr) return nullptr;

	// Controller of owner found
	return OwnerPawn->GetController();
}

