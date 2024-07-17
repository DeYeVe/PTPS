// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPSWeaponRifle.h"

#include "PTPSAnimInstance.h"

APTPSWeaponRifle::APTPSWeaponRifle()
{
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	RemainedAmmo = 30;
}

void APTPSWeaponRifle::BeginPlay()
{
	Super::BeginPlay();
}

void APTPSWeaponRifle::Fire()
{
	Super::Fire();
	
	AnimInstance->PlayWeaponRifleFireMontage();
	UE_LOG(LogTemp, Warning, TEXT("Weapon Rifle Fire"));	
}

void APTPSWeaponRifle::Reload()
{
	Super::Reload();
	
	AnimInstance->PlayWeaponRifleReloadMontage();
}
