// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPSAncientGolem.h"

APTPSAncientGolem::APTPSAncientGolem()
{
	MeshGolem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshGolem"));


	auto InitSM = [this](USkeletalMeshComponent*& SM, const FString& SMPath)
	{
		const ConstructorHelpers::FObjectFinder<USkeletalMesh> SMFinder(*SMPath);
		if (SMFinder.Succeeded())
		{
			SM->SetSkeletalMesh(SMFinder.Object);
		}
	};

	InitSM(MeshGolem, TEXT("SkeletalMesh'/Game/Ancient_Golem/Mesh/SK_Ancient_Golem.SK_Ancient_Golem'"));
	
	MeshGolem->SetupAttachment(GetRootComponent());
}

void APTPSAncientGolem::BeginPlay()
{
	Super::BeginPlay();
}

void APTPSAncientGolem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
