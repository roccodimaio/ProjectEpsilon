// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "PlayerCharacter.h"
#include "Components/SceneComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root); 

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(GetRootComponent()); 

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::SetWeaponType(EWeaponType Type)
{
	WeaponType = Type; 
}

EWeaponType ABaseWeapon::GetWeaponType()
{
	return WeaponType;
}

void ABaseWeapon::EquipWeapon(ACharacter* Character)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);

	if (PlayerCharacter != nullptr)
	{
		AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RightHandSocket");
		
		switch (WeaponType)
		{
		case EWeaponType::EPS_Katana:
			PlayerCharacter->SetPlayerStance(EPlayerStance::EPS_Katana);
			break;
		case EWeaponType::EPS_Rifle:
			PlayerCharacter->SetPlayerStance(EPlayerStance::EPS_Rifle);
			break;
		case EWeaponType::EPS_Pistol:
			PlayerCharacter->SetPlayerStance(EPlayerStance::EPS_Pistol); 
			break;
		case EWeaponType::EPS_MAX:
			break;
		}
	}
}

void ABaseWeapon::UnequipWeapon(ACharacter* Character)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);

	if (PlayerCharacter != nullptr)
	{
		switch (WeaponType)
		{
		case EWeaponType::EPS_Katana:
			AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "SwordSocket");
			break;
		case EWeaponType::EPS_Rifle:
			AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RifleSocket");
			break;
		case EWeaponType::EPS_Pistol:
			AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "PistolSocket");
			break;
		case EWeaponType::EPS_MAX:
			break;
		}
	}
}

void ABaseWeapon::SwapWeapon(ACharacter* Character)
{
}

