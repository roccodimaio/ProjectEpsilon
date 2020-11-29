// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAICharacter.h"
#include "Gun.h"
#include "Components/BoxComponent.h"

AShooterAICharacter::AShooterAICharacter()
{

}

void AShooterAICharacter::BeginPlay()
{
	Super::BeginPlay();

	// Hide bone associated with Gun included with the character mesh.  This will hide the gun
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	// Spawn blueprint child Gun.h
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	// Attach Gun to the specified socket on the character mesh
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));

	// Set ShooterCharacter as the GunOwner
	Gun->SetOwner(this);
}

void AShooterAICharacter::PullTrigger()
{
	if (Gun)
	{
		Gun->PullTrigger();
	}
}

float AShooterAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bIsDead == true)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApply;
}
