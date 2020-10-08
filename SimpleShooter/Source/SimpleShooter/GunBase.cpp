// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root); 

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(GetRootComponent());

}



// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunBase::PullTrigger()
{
	// Spawn emitter at the location when PullTrigger() is called
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystem, Mesh, TEXT("MuzzleFlashSocket"));

	// Identify owner of GunBase
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn == nullptr) return; 

	AController* OwnerController = OwnerPawn->GetController();

	if (OwnerController == nullptr) return;
	
	FVector Location; 
	FRotator Rotation; 

	// Get location and rotation of the Owner ViewPoint
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;

	//TODO: LineTrace
	
	FHitResult HitResult;

	// Call LineTrace function utlizing the line trace setup in Unreal Engine whish as been assigned to GameTraceChannel1
	// @ ECC_GameTraceChannel1 - Setup by going to Project Settings -> Collision -> Line Trace - Open DefaultEngine file 
	// under Config folder to identify the assigned channel
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
	}


	//DrawDebugCamera(GetWorld(), Location, Rotation, 90.f, 2.f, FColor::Red, true);
}

