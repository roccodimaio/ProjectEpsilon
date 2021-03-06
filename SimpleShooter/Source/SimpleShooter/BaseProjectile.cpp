// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Engine/World.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
		CollisionComponent->InitSphereRadius(15.f);
		CollisionComponent->bTraceComplexOnMove = true;
		SetRootComponent(CollisionComponent);
		
	}

	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		ProjectileMesh->SetupAttachment(GetRootComponent());
	}

	if (!ProjectileMovement)
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovement->InitialSpeed = ProjectileMovementSpeed;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->ProjectileGravityScale = 0.f;
	}

	if (!ParticleTrail)
	{
		ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticleTrail"));
		ParticleTrail->SetupAttachment(GetRootComponent());
	}

	InitialLifeSpan = 5.f;

}



// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->MaxSpeed = ProjectileMovementSpeed;

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	

	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ParticleTrail->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::FireInDirection(const FVector& FireDirection)
{
	ProjectileMovement->Velocity = FireDirection * ProjectileMovement->InitialSpeed;
	
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit)
{
	//if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());
		}

		//UE_LOG(LogTemp, Warning, TEXT("ProjectileBase->OnHit->If"));
		AActor* MyOwner = GetOwner();

		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation());

		if (MyOwner)
		{
			UE_LOG(LogTemp, Warning, TEXT("ProjectileBase->OnHit->MyOwner TRUE"));
			UGameplayStatics::ApplyDamage(OtherActor, ProjectileDamage, MyOwner->GetInstigatorController(), this, DamageType);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ProjectileBase->OnHit->MyOwner FALSE"));
		}
		
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		// TODO camera shake
		Destroy();
	}
}

bool ABaseProjectile::TracePath(FHitResult& Hit, FVector& ShortDirection)
{
	return false;
}

