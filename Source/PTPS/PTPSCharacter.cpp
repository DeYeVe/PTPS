// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTPSCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PTPSAnimInstance.h"
#include "PTPSWeapon.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APTPSCharacter

APTPSCharacter::APTPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 470.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Skins/HeavyArmor/Meshes/Belica_HeavyArmor.Belica_HeavyArmor'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	
	GetMesh()->SetupAttachment(RootComponent);
	
	// Zoom
	DefaultCameraLocation = FVector(0.0f, 80.0f, 70.0f);
	ZoomedCameraLocation = FVector(0.0f, 60.0f, 70.0f);
	DefaultCameraBoomLength = 290;
	ZoomedCameraBoomLength = 150;
	FollowCamera->SetRelativeLocation(DefaultCameraLocation);
	bIsZooming = false;
	bCanFire = true;
	bCanAct = true;
}

void APTPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	AnimInstance = Cast<UPTPSAnimInstance>(GetMesh()->GetAnimInstance());

	// init weapon
	if (PistolClass)
	{
		APTPSWeapon* NewWeapon = GetWorld()->SpawnActor<APTPSWeapon>(PistolClass);
		if (NewWeapon)
		{
			NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			NewWeapon->SetOwner(this);
			NewWeapon->SetActorHiddenInGame(true);
			WeaponSlots.Add(NewWeapon);
		}
	}
	SelectWeapon(0);
	UE_LOG(LogTemp, Warning, TEXT("%d num of weapons"),WeaponSlots.Num());

	// AnimNotify
	AnimInstance->CanFire.AddLambda([this](){ bCanFire = true; });
	AnimInstance->OnMontageEnded.AddDynamic(this,&APTPSCharacter::OnMontageEnded);
}

void APTPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->GetMaxSpeed());	

	// camera control with zooming
	FVector ZoomLocation = bIsZooming && bCanAct ? ZoomedCameraLocation : DefaultCameraLocation;
	float ZoomLength = bIsZooming && bCanAct ? ZoomedCameraBoomLength : DefaultCameraBoomLength;
	FollowCamera->SetRelativeLocation(FMath::VInterpTo(FollowCamera->GetRelativeLocation(), ZoomLocation, DeltaSeconds, 10.0f));
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ZoomLength, DeltaSeconds, 10.0f);

	// speed
	if (bIsZooming)
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
	else
	{
		float MoveSpeed = bIsSprinting ? 800.f : 500.f;
		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, MoveSpeed, DeltaSeconds, 15.0f);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APTPSCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APTPSCharacter::Look);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APTPSCharacter::Fire);

		// ReloadAction
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APTPSCharacter::Reload);

		// SprintAction
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APTPSCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APTPSCharacter::SprintEnd);

		// Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &APTPSCharacter::ZoomIn);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &APTPSCharacter::ZoomOut);

		// Roll		
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APTPSCharacter::Roll);
		
		// Select Weapon
		EnhancedInputComponent->BindAction(SelectWeapon1, ETriggerEvent::Triggered, this, &APTPSCharacter::SelectWeapon, 0);
		EnhancedInputComponent->BindAction(SelectWeapon2, ETriggerEvent::Triggered, this, &APTPSCharacter::SelectWeapon, 1);
		EnhancedInputComponent->BindAction(SelectWeapon3, ETriggerEvent::Triggered, this, &APTPSCharacter::SelectWeapon, 2);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APTPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APTPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APTPSCharacter::Fire()
{
	if (!bCanAct)
		return;
	
	if (!bCanFire)
		return;
		
	if (!bIsZooming)
		return;
	
	if (!(CurrentWeapon->GetCanFire()))
		return;
	
	bCanFire = false;
	
	if (CurrentWeapon->WeaponTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Weapons.Rifle"))))
	{
		AnimInstance->PlayPlayerRifleFireMontage();
		UE_LOG(LogTemp, Warning, TEXT("Rifle Fire"));	
	}
	else if (CurrentWeapon->WeaponTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Weapons.Pistol"))))
	{
		AnimInstance->PlayPlayerPistolFireMontage();
	}
	//else if weapon3
	
	CurrentWeapon->Fire();
	
}

void APTPSCharacter::Reload()
{
	if (!bCanAct)
		return;
	
	if (!(CurrentWeapon->GetCanReload()))
		return;
	
	bCanAct = false;
	
	if (CurrentWeapon->WeaponTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Weapons.Rifle"))))
	{
		AnimInstance->PlayPlayerRifleReloadMontage();
		UE_LOG(LogTemp, Warning, TEXT("Rifle Reload"));	
	}
	else if (CurrentWeapon->WeaponTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Weapons.Pistol"))))
	{
		AnimInstance->PlayPlayerPistolReloadMontage();
	}
	//else if weapon3
	
	CurrentWeapon->Reload();
}

void APTPSCharacter::ReloadEnd()
{
	if (CurrentWeapon->WeaponTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Weapons.Rifle"))))
	{		
		CurrentWeapon->ReloadEnd();
	}
	else if (CurrentWeapon->WeaponTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Weapons.Pistol"))))
	{
		CurrentWeapon->ReloadEnd();
	}
	//else if weapon3

	bCanAct = true;
}

void APTPSCharacter::SprintStart()
{
	bIsSprinting = true;
}

void APTPSCharacter::SprintEnd()
{
	bIsSprinting = false;
}

void APTPSCharacter::Roll()
{
	if (!bCanAct)
		return;
	
	bCanAct = false;
	GetCharacterMovement()->SetJumpAllowed(false);
	AnimInstance->PlayPlayerRollMontage();
}

void APTPSCharacter::RollEnd()
{
	bCanAct = true;
	GetCharacterMovement()->SetJumpAllowed(true);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void APTPSCharacter::ZoomIn()
{
	if (!bCanAct)
		return;
	
	bIsZooming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationYaw = true;
}

void APTPSCharacter::ZoomOut()
{
	bIsZooming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}

void APTPSCharacter::SelectWeapon(int32 WeaponIndex)
{
	if (!bCanAct)
		return;
	
	if (WeaponSlots.IsValidIndex(WeaponIndex) && WeaponSlots[WeaponIndex] != nullptr)
	{
		HideAllWeapons();
		ShowWeapon(WeaponIndex);
		CurrentWeapon = WeaponSlots[WeaponIndex];
		if (AnimInstance)
			AnimInstance->WeaponTypeTag = CurrentWeapon->WeaponTypeTag;
	}
}

void APTPSCharacter::HideAllWeapons()
{
	for (APTPSWeapon* Weapon : WeaponSlots)
	{
		if (Weapon)
		{
			Weapon-> SetActorHiddenInGame(true);
		}
	}
}

void APTPSCharacter::ShowWeapon(int32 WeaponIndex)
{
	if (WeaponSlots.IsValidIndex(WeaponIndex) && WeaponSlots[WeaponIndex] != nullptr)
	{
		WeaponSlots[WeaponIndex]->SetActorHiddenInGame(false);
	}
}

void APTPSCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimInstance->PlayerRollMontage)
	{
		RollEnd();
	}
	else if (Montage == AnimInstance->PlayerPistolReloadMontage
		|| Montage == AnimInstance->PlayerRifleReloadMontage )
	{
		ReloadEnd();
	}
}
