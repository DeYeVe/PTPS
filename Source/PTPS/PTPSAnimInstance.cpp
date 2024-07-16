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
	
	InitMontage(PlayerPistolFireMontage, TEXT("AnimMontage'/Game/Player/Animation/AM_PistolFire.AM_PistolFire'"));
	InitMontage(PlayerRifleFireMontage, TEXT("AnimMontage'/Game/Player/Animation/AM_RifleFire.AM_RifleFire'"));
	InitMontage(PlayerRollMontage, TEXT("AnimMontage'/Game/Player/Animation/AM_Roll.AM_Roll'"));
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

void UPTPSAnimInstance::AnimNotify_CanFire() const
{
	CanFire.Broadcast();
}