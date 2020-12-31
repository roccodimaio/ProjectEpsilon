// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "BaseMissile.h"
#include "Components/BoxComponent.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	SetRootComponent(BoxCollisionComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	TurretMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	InitMissileLaunch();
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::InitMissileLaunch()
{
	GetWorldTimerManager().SetTimer(MissileHandle, this, &ATurret::SpawnMissile, 3.f, true);
}

void ATurret::SpawnMissile()
{
	if (MissileClass != NULL)
	{
		UWorld* const World = GetWorld();

		if (World != NULL)
		{
			FVector CurrentPos = GetActorLocation();
			FRotator CurrentRot = FRotator(0, 0, 0);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			ABaseMissile* FiredMissile = World->SpawnActor<ABaseMissile>(MissileClass, CurrentPos, CurrentRot, SpawnParams);

			if (FiredMissile != nullptr)
			{
				FRotator MeshRot = FiredMissile->MissileMeshComponent->GetComponentRotation();
				MeshRot.Roll = 0.f;
				MeshRot.Pitch = 0.f;
				MeshRot.Yaw = 0.f;
				FiredMissile->MissileMeshComponent->SetRelativeRotation(MeshRot);
			}
		}
	}
}

