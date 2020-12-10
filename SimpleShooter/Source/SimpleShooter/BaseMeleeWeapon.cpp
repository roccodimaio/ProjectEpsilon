// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseMeleeWeapon::ABaseMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABaseMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

