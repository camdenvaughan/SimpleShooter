// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUps/PickUpBase.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AHealthPickup : public APickUpBase
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float HealthAmount = 20.f;

protected:

	virtual void GiveItemToPlayer() override;
};
