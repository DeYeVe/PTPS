// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "PTPSAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FDelegate);
/**
 * 
 */
UCLASS()
class PTPS_API UPTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPTPSAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	
public:
	void PlayPlayerRollMontage();
	void PlayPlayerPistolFireMontage();
	void PlayPlayerRifleFireMontage();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Player, Meta=(AllowPrivateAccess=true)) 
	UAnimMontage* PlayerRollMontage;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Player, Meta=(AllowPrivateAccess=true)) 
	UAnimMontage* PlayerPistolFireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Player, Meta=(AllowPrivateAccess=true)) 
	UAnimMontage* PlayerRifleFireMontage;
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Pitch;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Yaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector2D MovementVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsZooming;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsDashing;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FGameplayTag WeaponTypeTag;

	FDelegate CanFire;

private:
	UFUNCTION()
	void AnimNotify_CanFire() const;
};
