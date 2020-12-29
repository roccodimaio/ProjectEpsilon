// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon_Melee.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "BaseAICharacter.h"

ABaseWeapon_Melee::ABaseWeapon_Melee()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(Mesh, "CollisionSocket");
}

void ABaseWeapon_Melee::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon_Melee::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon_Melee::OnOverlapEnd);
}

void ABaseWeapon_Melee::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector HitDirection;
	HitDirection = -ViewPointRotation.Vector();
	
	const USkeletalMeshSocket* TipSocket = Mesh->GetSocketByName("TipSocket");
	
	AController* OwnerController = GetOwnerController();
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	
	if (ImpactParticleSystem != nullptr && TipSocket)
	{
		FVector SocketLocation = TipSocket->GetSocketLocation(Mesh);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticleSystem, SocketLocation, FRotator(0.f), false);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, SocketLocation); 
	}
	
	if (OtherActor != nullptr)
	{
		ABaseAICharacter* EnemyCharacter = Cast<ABaseAICharacter>(OtherActor);

		if (EnemyCharacter)
		{
			FPointDamageEvent DamageEvent(Damage, SweepResult, -ViewPointRotation.Vector(), nullptr);

			EnemyCharacter->TakeDamage(Damage, DamageEvent, GetOwnerController(), GetOwner());
		}
	}

}

void ABaseWeapon_Melee::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABaseWeapon_Melee::EnableCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseWeapon_Melee::DisableCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
