// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PTPSWeapon.h"
#include "PTPSWeaponPistol.generated.h"

/**
 * 
 */
UCLASS()
class PTPS_API APTPSWeaponPistol : public APTPSWeapon
{
	GENERATED_BODY()

	APTPSWeaponPistol();

	virtual void BeginPlay() override;

	virtual void Fire() override;

	virtual void Reload() override;
	
};
