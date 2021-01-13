// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkillBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Engine/World.h"
//#include "NiagaraComponent.h"
//#include "NiagaraFunctionLibrary.h"
//#include "NiagaraSystem.h"

// Sets default values
AProjectileSkillBase::AProjectileSkillBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(15.f);
		CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");

		// Players cannot walk on ProjectileSkillBase
		CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
		CollisionComponent->CanCharacterStepUpOn = ECB_No;

		RootComponent = CollisionComponent;

	}
	
	if (!ProjectileSkillMesh)
	{
		ProjectileSkillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileSkillMesh"));
		ProjectileSkillMesh->SetupAttachment(GetRootComponent());
		
	}
	
	if (!ProjectileSkillMovement)
	{
		ProjectileSkillMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileSkillMovement"));
		ProjectileSkillMovement->InitialSpeed = ProjectileSkillMovementSpeed;
		ProjectileSkillMovement->MaxSpeed = ProjectileSkillMaxMovementSpeed;
		ProjectileSkillMovement->bRotationFollowsVelocity;
		ProjectileSkillMovement->bShouldBounce = true; 
	}
	
	if (!ParticleSkillTrail)
	{
		ParticleSkillTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleTrail"));
		ParticleSkillTrail->SetupAttachment(GetRootComponent());
	}
	
	InitialLifeSpan = 5.f;

}

// Called when the game starts or when spawned
void AProjectileSkillBase::BeginPlay()
{
	Super::BeginPlay();
	
	//UE_LOG(LogTemp, Warning, TEXT("ProjectileSkillBase->BeginPlay"));

	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileSkillMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ParticleSkillTrail->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 

	//this->OnActorHit.AddDynamic(this, &AProjectileSkillBase::OnHit);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileSkillBase::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

	//ProjectileSkillMesh->OnComponentHit.AddDynamic(this, &AProjectileSkillBase::OnHit);
	
	//if (NiagaraEffect != nullptr)
	//{
		
	//}

}

// Called every frame
void AProjectileSkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileSkillBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ProjectileSkillBase->OnHit"));


	//if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());
		}
		
		UE_LOG(LogTemp, Warning, TEXT("ProjectileSkillBase->OnHit->If"));
		AActor* MyOwner = GetOwner();
		
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, ProjectileSkillDamage, MyOwner->GetInstigatorController(), this, DamageType);
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		// TODO camera shake
		Destroy();
	}
}

/**
void AProjectileSkillBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ProjectileSkillBase->OnHit"));
	AActor* MyOwner = GetOwner();

	if (MyOwner == nullptr)
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ProjectileSkillBase->OnHit->If statement"));

		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, ProjectileSkillDamage, MyOwner->GetInstigatorController(), this, DamageType);
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		// TODO camera shake
		Destroy(); 
	}
}

*/
bool AProjectileSkillBase::TracePath(FHitResult& Hit, FVector& ShortDirection)
{
	return false;
}

void AProjectileSkillBase::FireInDirection(const FVector& FireDirection)
{
	ProjectileSkillMovement->Velocity = FireDirection * ProjectileSkillMovement->InitialSpeed;
}

