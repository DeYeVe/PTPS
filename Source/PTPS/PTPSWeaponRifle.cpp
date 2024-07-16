// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPSWeaponRifle.h"

APTPSWeaponRifle::APTPSWeaponRifle()
{
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	RemainedAmmo = 0;	
}

void APTPSWeaponRifle::BeginPlay()
{
	Super::BeginPlay();
}

void APTPSWeaponRifle::Fire()
{
	Super::Fire();
}

void APTPSWeaponRifle::Reload()
{
	Super::Reload();
}
