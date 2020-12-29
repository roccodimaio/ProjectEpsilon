// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkillBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Engine/World.h"

// Sets default values
AProjectileSkillBase::AProjectileSkillBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSkillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileSkillMesh"));
	RootComponent = ProjectileSkillMesh;

	ProjectileSkillMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileSkillMovement"));
	ProjectileSkillMovement->InitialSpeed = ProjectileSkillMovementSpeed;

	ParticleSkillTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleTrail"));
	ParticleSkillTrail->SetupAttachment(GetRootComponent());

	InitialLifeSpan = 5.f;

}

// Called when the game starts or when spawned
void AProjectileSkillBase::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileSkillMesh->OnComponentHit.AddDynamic(this, &AProjectileSkillBase::OnHit);
}

// Called every frame
void AProjectileSkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileSkillBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (MyOwner == nullptr)
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, ProjectileSkillDamage, MyOwner->GetInstigatorController(), this, DamageType);
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		// TODO camera shake
		Destroy(); 
	}
}

