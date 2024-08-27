// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "BaseEnemy.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlayerControllerClass.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	SamuraiMeshComp = FindComponentByClass<UStaticMeshComponent>();
	
	PlayerController = Cast<APlayerControllerClass>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}


// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AMainCharacter::TurnCamera);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMainCharacter::Attack);
	};
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
    
	FRotator Rotation = GetMesh()->GetRelativeRotation();

	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightDirection, MovementVector.Y * 50);
	AddMovementInput(ForwardDirection, MovementVector.X  * 50);
}


void AMainCharacter::TurnCamera(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	FRotator NewRotation = GetMesh()->GetRelativeRotation();

	NewRotation.Yaw += InputValue;

	GetMesh()->SetRelativeRotation(NewRotation);
}

void AMainCharacter::Attack(const FInputActionValue& Value)
{
	RageMeter += RagePerAttack;
	RageMeter = FMath::Clamp(RageMeter, 0.0f, MaxRage);

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRange); // Define your attack range

	// Perform the collision check
	TArray<FHitResult> HitResults;
	FQuat Rotation = GetActorRotation().Quaternion();
	FVector Start = SamuraiMeshComp->GetSocketLocation("AttackPoint");
	FVector End = Start + (Rotation * FVector::ForwardVector * AttackRange); // Adjust as needed

	PlayerController->UpdateRageUI(RageMeter/MaxRage);
	
	DrawDebugSphere(GetWorld(), Start, AttackRange, 12, FColor::Red, false, 2.0f);

	
	// Perform a sphere trace to detect enemies
	if (GetWorld()->SweepMultiByChannel(HitResults, Start, End, Rotation, ECC_GameTraceChannel2, CollisionShape))
	{
		for (FHitResult Hit : HitResults)
		{
			// Assuming the enemy has a method to take damage
			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Hit.GetActor()))
			{
				Enemy->TakeDamage(AttackDamage, FDamageEvent(), GetController(), this);
			}
		}
	}
}

