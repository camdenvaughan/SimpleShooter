// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "SimpleShooter/Actors/Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooter/GameModes/SimpleShooterGameModeBase.h"
#include "SimpleShooter/Controllers/ShooterPlayerController.h"
#include "SimpleShooter/Controllers/ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "SimpleShooter/PickUps/PickUpBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Health and spawn guns
	Health = MaxHealth;
	SpawnGuns();
}

void AShooterCharacter::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this , &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("SwitchWeapons"), IE_Pressed, this, &AShooterCharacter::SwitchWeapons);
	FInputActionBinding& Toggle = PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &AShooterCharacter::PausePressed);
	Toggle.bExecuteWhenPaused = true;
}


// Called to bind functionality to input
void AShooterCharacter::SpawnGuns() 
{
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	ActiveGun.SetNum(GunArray.Num());

	for (int32 i = 0; i < ActiveGun.Num(); i++)
	{
		ActiveGun[i] = GetWorld()->SpawnActor<AGun>(GunArray[i]);
		ActiveGun[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		ActiveGun[i]->SetOwner(this);
		ActiveGun[i]->SetActorHiddenInGame(true);
	}

	if (!IsPlayerControlled() && bSpawnWithRandomGun)
	{
		// Create Random int for ActiveGun Array
		int32 RandGun = FMath::RandRange(0, GunArray.Num()- 1);

		if (!ActiveGun[RandGun])
		{
			// If Random Int is outside bounds
			UE_LOG(LogTemp, Warning, TEXT("Rand Int was Outside Bounds of GunArray on %s"), *GetOwner()->GetName());
			return;
		}
			ActiveGunIndex = RandGun;		
	}
	// Show gun at ActiveGunIndex
	ActiveGun[ActiveGunIndex]->SetActorHiddenInGame(false);
}

/* Damage and Health */
//
//
float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Health == 0) return DamageApplied;
	Health = FMath::Clamp(Health - DamageApplied, 0.f, MaxHealth);

	if (IsDead())
	{
		if (!this->IsPlayerControlled())
		{
			TryToSpawnPickup();
			--Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->Enemies;
		}
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageApplied;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

/* Pickups */
//
//
void AShooterCharacter::TryToSpawnPickup() 
{
	if (PickupsArrayClass.Num() < 1) return;
	int32 Max = PickupsArrayClass.Num() - 1;
	int32 RandInt = FMath::RandRange(0, Max);
	UE_LOG(LogTemp, Warning, TEXT("pickup was spawned"));
	Pickup = GetWorld()->SpawnActor<APickUpBase>(PickupsArrayClass[RandInt], this->GetActorLocation(), FRotator::ZeroRotator);

}
void AShooterCharacter::AddHealth(float HealthToAdd) 
{
	Health += HealthToAdd;
}
void AShooterCharacter::AddAmmo(uint32 AmmoToAdd) 
{
	ActiveGun[ActiveGunIndex]->AddAmmo(AmmoToAdd);
}

/* Widget Functions */
//
//
float AShooterCharacter::GetGunCooldown() const
{
	if (ActiveGun[ActiveGunIndex])
	{
		return ActiveGun[ActiveGunIndex]->GetCooldownPercent();
	}
	return 0.f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

FString AShooterCharacter::GetGunAmmo() const
{
	if (ActiveGun[ActiveGunIndex])
	{
		return FString::FromInt(ActiveGun[ActiveGunIndex]->GetAmmo());
	}
	return "";
}

// Called by AIController to increment Enemies
void AShooterCharacter::SetEnemies() 
{
	++Enemies;
}

int32 AShooterCharacter::GetEnemies() const
{
	return Enemies;
}


/* Input Functions */
//
//
void AShooterCharacter::MoveForward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue) 
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot() 
{
	ActiveGun[ActiveGunIndex]->PullTrigger();
}

void AShooterCharacter::SwitchWeapons() 
{
	ActiveGun[ActiveGunIndex]->SetActorHiddenInGame(true);
	++ActiveGunIndex;
	if (ActiveGunIndex == GunArray.Num())
	{
		ActiveGunIndex = 0;
	}
	ActiveGun[ActiveGunIndex]->SetActorHiddenInGame(false);
}


void AShooterCharacter::PausePressed() 
{
	bIsPaused = !bIsPaused;
	UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);
	AShooterPlayerController* ShooterController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ShooterController->ShowPauseUI(bIsPaused);
	ShooterController->bShowMouseCursor = bIsPaused;
	UE_LOG(LogTemp, Warning, TEXT("%d"), bIsPaused);
}