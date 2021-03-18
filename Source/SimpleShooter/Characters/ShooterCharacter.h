// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AShooterCharacter();

	virtual void Tick(float DeltaSeconds);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SetEnemies();
	void Shoot();
	void AddAmmo(uint32 AmmoToAdd);
	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	float GetGunCooldown() const;
	
	UFUNCTION(BlueprintPure)
	FString GetGunAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetEnemies() const;

	UFUNCTION(BlueprintCallable)
	void PausePressed();

private:

	// Variables
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health = 1.f;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickUpBase>> PickupsArrayClass;

	UPROPERTY(EditAnywhere)
	int32 ActiveGunIndex = 0;

	UPROPERTY()
	TArray<AGun*> ActiveGun;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGun>> GunArray;

	UPROPERTY(EditAnywhere)
	bool bSpawnWithRandomGun = false;

	bool bIsPaused = false;
	class APickUpBase* Pickup;
	int32 Enemies;

	// Functions
	void TryToSpawnPickup();
	void SpawnGuns();
	void SwitchWeapons();
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	
};
