// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPSWeapon.h"

#include "PTPSCharacter.h"

// Sets default values
APTPSWeapon::APTPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshWeapon"));
	MeshWeapon->SetupAttachment(RootComponent);

	bCanFire = true;
	bCanReload = true;
}

// Called when the game starts or when spawned
void APTPSWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (MaxAmmo == CurrentAmmo)
		bCanReload = false;
	
}

// Called every frame
void APTPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APTPSWeapon::AttachWeapon(APTPSCharacter* TargetCharacter)
{
	Character = TargetCharacter;
}

void APTPSWeapon::Fire()
{

	CurrentAmmo--;
		//animinst->playanim
		
	if (CurrentAmmo == 0)
		bCanFire = false;

	if (RemainedAmmo != 0)
		bCanReload = true;
}

void APTPSWeapon::Reload()
{
	if (RemainedAmmo > 0)
	{
		int32 RequiredAmmo = MaxAmmo - CurrentAmmo;
		int32 ReloadedAmmo = (RemainedAmmo > RequiredAmmo) ? RequiredAmmo : RemainedAmmo;
		CurrentAmmo += ReloadedAmmo;
		RemainedAmmo -= ReloadedAmmo;		
		
		bCanReload = false;		
	}
}

