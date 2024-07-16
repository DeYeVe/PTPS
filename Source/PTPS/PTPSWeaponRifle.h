// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PTPSWeapon.h"
#include "PTPSWeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class PTPS_API APTPSWeaponRifle : public APTPSWeapon
{
	GENERATED_BODY()

	APTPSWeaponRifle();

	virtual void BeginPlay() override;

	virtual void Fire() override;

	virtual void Reload() override;
};
