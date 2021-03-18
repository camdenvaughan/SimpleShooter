// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUps/PickUpBase.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AAmmoPickup : public APickUpBase
{
	GENERATED_BODY()
private:

	UPROPERTY(EditAnywhere)
	uint32 AmmoAmount = 10;

protected:
	virtual void GiveItemToPlayer() override;
};
