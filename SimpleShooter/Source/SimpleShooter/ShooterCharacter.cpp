// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GunBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGunBase>(GunClass);

	// Hide bone from mesh (rifle on Wraith character)
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	// Attached Gun to the RightHandWeaponSocket socket on the ShooterCharacter
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandWeaponSocket"));
	Gun->SetOwner(this); 
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump); 
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot); 

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageToApply, 0.f, MaxHealth);
		
	UE_LOG(LogTemp, Warning, TEXT("Take Damage; Remaining health at %f!"), Health);

	return DamageToApply;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	// Call built in function AddMovementInput and use the Forward vector to move Forward / Backward
	AddMovementInput(GetActorForwardVector() * AxisValue); 
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	// Call built in function AddMovementInput and use the Right vector to move Right / Left
	AddMovementInput(GetActorRightVector() * AxisValue); 
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	// Identify the direction and distance to rotate per frame
	AddControllerPitchInput(AxisValue * RotationRateUp * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	// Identify the direction and distance to rotate per frame
	AddControllerYawInput(AxisValue * RotationRateRight * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	if (Gun)
	{
		Gun->PullTrigger();
	}
}



