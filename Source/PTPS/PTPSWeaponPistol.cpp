// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPSWeaponPistol.h"

#include "PTPSAnimInstance.h"

APTPSWeaponPistol::APTPSWeaponPistol()
{
	MaxAmmo = 10;
	CurrentAmmo = MaxAmmo;
	RemainedAmmo = 30;
}

void APTPSWeaponPistol::BeginPlay()
{
	Super::BeginPlay();
}

void APTPSWeaponPistol::Fire()
{
	Super::Fire();

	AnimInstance->PlayWeaponPistolFireMontage();
}

void APTPSWeaponPistol::Reload()
{
	Super::Reload();
	
	AnimInstance->PlayWeaponPistolReloadMontage();
}
