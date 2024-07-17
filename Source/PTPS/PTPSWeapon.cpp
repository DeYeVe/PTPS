// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPSWeapon.h"

#include "PTPSAnimInstance.h"
#include "PTPSCharacter.h"

// Sets default values
APTPSWeapon::APTPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshWeapon"));
	MeshWeapon->SetupAttachment(RootComponent);

	bCanFire = true;
	bCanReload = false;
}

// Called when the game starts or when spawned
void APTPSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<UPTPSAnimInstance>(MeshWeapon->GetAnimInstance());
}

// Called every frame
void APTPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentAmmo == 0)
		bCanFire = false;

	UE_LOG(LogTemp, Warning, TEXT("RemainedAmmo = %d"), RemainedAmmo);
	UE_LOG(LogTemp, Warning, TEXT("CurrentAmmo = %d"), CurrentAmmo);
	UE_LOG(LogTemp, Warning, TEXT("MaxAmmo = %d"), MaxAmmo);
	
	if (RemainedAmmo > 0 && CurrentAmmo < MaxAmmo)
		bCanReload = true;
	else
		bCanReload = false;

}

void APTPSWeapon::AttachWeapon(APTPSCharacter* TargetCharacter)
{
	Character = TargetCharacter;
}

void APTPSWeapon::Fire()
{
	CurrentAmmo--;
}

void APTPSWeapon::Reload()
{
}

void APTPSWeapon::ReloadEnd()
{
	int32 RequiredAmmo = MaxAmmo - CurrentAmmo;
    int32 ReloadedAmmo = (RemainedAmmo > RequiredAmmo) ? RequiredAmmo : RemainedAmmo;
    CurrentAmmo += ReloadedAmmo;
    RemainedAmmo -= ReloadedAmmo;	
}

