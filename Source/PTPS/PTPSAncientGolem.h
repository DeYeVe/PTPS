// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PTPSCharacter.h"
#include "PTPSAncientGolem.generated.h"

/**
 * 
 */
UCLASS()
class PTPS_API APTPSAncientGolem : public ACharacter
{
	GENERATED_BODY()

public:
	APTPSAncientGolem();

protected:	
	UPROPERTY(VisibleAnywhere, Category=Mesh)
	USkeletalMeshComponent* MeshGolem;

	USkeletalMeshComponent* GetMesh() const { return MeshGolem; };

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
};
