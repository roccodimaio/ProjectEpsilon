// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMissile.h"
#include "PlayerCharacter.h"
#include "BaseAICharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "EngineUtils.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseMissile::ABaseMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	SetRootComponent(BoxCollisionComponent);

	MissileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MissileMeshComponent->SetupAttachment(GetRootComponent());

	TrailParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticleSystemComponent"));
	TrailParticleSystemComponent->SetupAttachment(GetRootComponent());
	

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = BoxCollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 200.f;
	ProjectileMovementComponent->MaxSpeed = 300.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->bShouldBounce = false; 
	ProjectileMovementComponent->bIsHomingProjectile = false; 
	ProjectileMovementComponent->HomingAccelerationMagnitude = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->Velocity = FVector(0, 0, 0);

	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseMissile::OnOverlapBegin);

	// Set Default Values for Variables
	bHasTargetPosition = false; 
	bHasNoTarget = false; 
	target = NULL;
	DelayTimer = 0.f;
	bHasFinishedDelay = false;
	LifeTimeCountdown = 15.f;
	bCanBeDestroyed = false; 
	PlayerCharacter = NULL;
	UpwardsLaunchDistance = 300.f;
	MissileHomingTightness = 200.f;
	MissileTurnSensativity = 200.f;
	MissileSpeed = 200.f;
	MissileBoosterDelay = 1.f;
	GravityScale = 0.5f;
	Damage = 20.f;
	ImpluseMultiplier = 2.f;
	ExplosionRadius = 1000.f;
	ExplosionDamage = 15.f; 

}

// Called when the game starts or when spawned
void ABaseMissile::BeginPlay()
{
	Super::BeginPlay();

	TrailParticleSystemComponent->DeactivateSystem();
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	// Before we find our target, launch BaseMissile upwards
	if (!bHasTargetPosition)
	{
		ProjectileMovementComponent->Velocity = GetActorUpVector() * UpwardsLaunchDistance;
		ProjectileMovementComponent->ProjectileGravityScale = 1.f;

		this->SetActorEnableCollision(false);
	}
}

// Called every frame
void ABaseMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasFinishedDelay)
	{
		DelayLogic(DeltaTime);
	}
	else
	{
		if (bHasTargetPosition)
		{
			if (target != NULL)
			{
				if (target->IsValidLowLevel())
				{
					FVector WantedDir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					WantedDir += target->GetVelocity() * WantedDir.Size() / MissileHomingTightness;
					ProjectileMovementComponent->Velocity += WantedDir * MissileTurnSensativity * DeltaTime;
					ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity.GetSafeNormal() * MissileSpeed;
				}
				else
				{
					if (!this->IsPendingKill())
					{
						if (this->IsValidLowLevel())
						{
							K2_DestroyActor();
						}
					}
				}
			}
			else
			{
				// If a target is NOT found, countine to move the missile actor upwards
				if (bHasNoTarget)
				{
					ProjectileMovementComponent->Velocity = GetActorUpVector() * MissileSpeed;
					bHasNoTarget = false; 
				}
			}
			
		}
		
		// Destroy the missile actor after 15 second countdown
		LifeTimeCountdown -= 1 * DeltaTime;

		if (LifeTimeCountdown < 0.f)
		{
			if (!bCanBeDestroyed)
			{
				bCanBeDestroyed = true;
				target = nullptr;
				Explode();
			}
		}
	}

}

void ABaseMissile::DelayLogic(float dTime)
{
	if (!bHasFinishedDelay)
	{
		DelayTimer += 1 * dTime;

		if (DelayTimer > MissileBoosterDelay)
		{
			UpdateTarget();
			BoxCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
			BoxCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
			TrailParticleSystemComponent->ActivateSystem(true);
			this->SetActorEnableCollision(true);
			bHasFinishedDelay = true;
			ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		}
	}
}

void ABaseMissile::FindPlayer()
{
	class UWorld* const World = GetWorld();

	if (World)
	{
		for (TActorIterator<APlayerCharacter>ObstacleItr(World); ObstacleItr; ++ObstacleItr)
		{
			FName PlayerTagName = FName(TEXT("Player"));
			APlayerCharacter* FoundPlayer = *ObstacleItr;

			if (FoundPlayer != nullptr)
			{
				if (FoundPlayer->ActorHasTag(PlayerTagName))
				{
					if (PlayerCharacter != FoundPlayer)
					{
						PlayerCharacter = FoundPlayer;
					}
				}
			}
		}
	}
}

void ABaseMissile::SetTarget(AActor* Actor)
{
	if (Actor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseMissile->SetTarget"));
		target = Actor; 
	}
}

void ABaseMissile::UpdateTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseMissile->UpdateTarget"));
	if (!bHasTargetPosition)
	{
		if (target != NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("BaseMissile->UpdateTarget->Target != null"));
			bHasTargetPosition = true;
			bHasNoTarget = false; 

			// Keep mesh rotation offset
			FRotator rotVal = MissileMeshComponent->GetComponentRotation();
			rotVal.Roll = 0.f;
			rotVal.Pitch = 0.f;
			rotVal.Yaw = 0.f;
			MissileMeshComponent->SetRelativeRotation(rotVal);
		}
		else
		{
			target = nullptr;
			bHasTargetPosition = true;
			bHasNoTarget = true; 
		}
	}
	else
	{
		target = nullptr;
		bHasTargetPosition = true; 
		bHasNoTarget = true;
	}
}

UParticleSystemComponent* ABaseMissile::PlayExplosion(UParticleSystem* ParticleSystem)
{
	class UParticleSystemComponent* retVal = NULL;

	if (ParticleSystem)
	{
		class UWorld* const World = GetWorld();

		if (World)
		{
			FVector MyPos = GetActorLocation();
			FRotator MyRot = GetActorRotation();

			retVal = UGameplayStatics::SpawnEmitterAtLocation(World, ParticleSystem, MyPos, MyRot, true);
		}
	}
	return retVal;
}

UAudioComponent* ABaseMissile::PlayExplosionSound(USoundBase* Sound)
{
	class UAudioComponent* retVal = NULL;

	if (Sound)
	{
		class UWorld* const World = GetWorld();

		if (World)
		{
			FVector MyPos = GetActorLocation();
			FRotator MyRot = GetActorRotation();

			retVal = UGameplayStatics::SpawnSoundAtLocation(World, Sound, MyPos, MyRot, true);
		}

	}
	return nullptr;
}

void ABaseMissile::Explode()
{
	PlayExplosion(ExplosionParticleSystem);
	PlayExplosionSound(ExplosionSound);

	Destroy();
}

void ABaseMissile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * ImpluseMultiplier, GetActorLocation());
		}

		TArray<AActor*> ignoredActors;

		AController* OwnerController = GetOwnerController();

		//UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), ExplosionRadius, UDamageType::StaticClass(), ignoredActors, this, nullptr, false, ECollisionChannel::ECC_Visibility);
		Explode();
	}
	
	/**
	APlayerCharacter* FoundPlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(OtherActor);
	AStaticMeshActor* GroundActor = Cast<AStaticMeshActor>(OtherActor);

	if (FoundPlayerCharacter || AICharacter || GroundActor)
	{
		Explode();
	}
	*/
}

AController* ABaseMissile::GetOwnerController() const
{
	// Find owner of Gun
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	// If owner is nullptr return
	if (OwnerPawn == nullptr) return nullptr;

	// Controller of owner found
	return OwnerPawn->GetController();
}

