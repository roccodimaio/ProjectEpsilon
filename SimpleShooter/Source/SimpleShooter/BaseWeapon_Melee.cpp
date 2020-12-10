// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon_Melee.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"

ABaseWeapon_Melee::ABaseWeapon_Melee()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(Mesh, "CollisionSocket");

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}