// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPSAnimInstance.h"

#include "PTPSCharacter.h"
#include "PTPSWeapon.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPTPSAnimInstance::UPTPSAnimInstance()
{

	SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	
	auto InitMontage = [this](UAnimMontage*& Montage, const FString& MontagePath)
	{
		const ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(*MontagePath);
		if (MontageFinder.Succeeded())
		{
			Montage = MontageFinder.Object;
		}
	};
	
	// Player
	InitMontage(PlayerPistolFireMontage, TEXT("AnimMontage'/Game/Player/Animation/AM_PistolFire.AM_PistolFire'"));
	InitMontage(PlayerRifleFireMontage, TEXT("AnimMontage'/Game/Player/Animation/AM_RifleFire.AM_RifleFire'"));
	InitMontage(PlayerRollMontage, TEXT("AnimMontage'/Game/Player/Animation/AM_Roll.AM_Roll'"));

	InitMontage(PlayerRifleReloadMontage, TEXT("AnimMontage'/Game/Player/Animation/AM_RifleReload.AM_RifleReload'"));

	
	// Weapons
	InitMontage(WeaponPistolFireMontage, TEXT("AnimMontage'/Game/Weapons/Animation/AM_PistolFire.AM_PistolFire'"));
	InitMontage(WeaponRifleFireMontage, TEXT("AnimMontage'/Game/Weapons/Animation/AM_RifleFire.AM_RifleFire'"));
	
	InitMontage(WeaponPistolReloadMontage, TEXT("AnimMontage'/Game/Weapons/Animation/AM_PistolReload.AM_PistolReload'"));
	InitMontage(WeaponRifleReloadMontage, TEXT("AnimMontage'/Game/Weapons/Animation/AM_RifleReload.AM_RifleReload'"));
	

}

void UPTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto Pawn = TryGetPawnOwner();
	
	if (IsValid(Pawn))
	{
		if (const auto Character = Cast<APTPSCharacter>(Pawn))
		{
			Speed = Character->GetVelocity().Size();
			bIsFalling = Character->GetMovementComponent()->IsFalling();
			bIsZooming = Character->GetIsZooming();
			bIsSprinting = Character->GetIsSprinting();
			MovementVector = Character->GetMovementVector();

			// aim offset
			FRotator ControlRotator = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetBaseAimRotation(), Character->GetActorRotation());
			Pitch = UKismetMathLibrary::ClampAngle(ControlRotator.Pitch, -90.f, 90.f);
			Yaw = ControlRotator.Yaw;

			//UE_LOG(LogTemp, Warning, TEXT("%f Pitch"), Pitch);
			//UE_LOG(LogTemp, Warning, TEXT("%f Yaw"), Yaw);
		}		
	}
}

void UPTPSAnimInstance::PlayPlayerRollMontage()
{
	Montage_Play(PlayerRollMontage);
}

void UPTPSAnimInstance::PlayPlayerPistolFireMontage()
{
	Montage_Play(PlayerPistolFireMontage);
}

void UPTPSAnimInstance::PlayPlayerRifleFireMontage()
{
	Montage_Play(PlayerRifleFireMontage);
}

void UPTPSAnimInstance::PlayPlayerPistolReloadMontage()
{
	Montage_Play(PlayerPistolReloadMontage);
}

void UPTPSAnimInstance::PlayPlayerRifleReloadMontage()
{
	Montage_Play(PlayerRifleReloadMontage);
}

void UPTPSAnimInstance::PlayWeaponPistolFireMontage()
{
	Montage_Play(WeaponRifleFireMontage);
}

void UPTPSAnimInstance::PlayWeaponRifleFireMontage()
{
	Montage_Play(WeaponRifleFireMontage);
}

void UPTPSAnimInstance::PlayWeaponPistolReloadMontage()
{
	Montage_Play(WeaponRifleReloadMontage);
}

void UPTPSAnimInstance::PlayWeaponRifleReloadMontage()
{
	Montage_Play(WeaponRifleReloadMontage);
}

void UPTPSAnimInstance::AnimNotify_CanFire() const
{
	CanFire.Broadcast();
}
