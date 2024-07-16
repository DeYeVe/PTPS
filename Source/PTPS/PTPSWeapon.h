// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "PTPSWeapon.generated.h"

UCLASS()
class PTPS_API APTPSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APTPSWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	class UPTPSAnimInstance* AnimInstance;
	
	UPROPERTY(EditAnywhere, Category=Mesh)
	USkeletalMeshComponent* MeshWeapon;
	
	UPROPERTY(VisibleAnywhere)
	APTPSCharacter* Character;
	
	UPROPERTY(VisibleAnywhere)
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere)
	int32 MaxAmmo;
	
	UPROPERTY(VisibleAnywhere)
	int32 RemainedAmmo;
	
	UPROPERTY(VisibleAnywhere)
	int32 Damage;
	
	bool bCanFire;
	bool bCanReload;

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag WeaponTypeTag;
	
	UFUNCTION(BlueprintCallable)
	int GetCurAmmo() { return CurrentAmmo; };
	
	UFUNCTION(BlueprintCallable)
	int GetMaxAmmo() { return MaxAmmo; };
	
	UFUNCTION(BlueprintCallable)
	int GetTotalAmmo() { return RemainedAmmo; };
	
	UFUNCTION(BlueprintCallable)
	bool GetCanFire() { return bCanFire; };
	
	UFUNCTION(BlueprintCallable)
	bool GetCanReload() { return bCanReload; };

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(APTPSCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Reload();
};
