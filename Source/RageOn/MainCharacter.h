// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MainCharacter.generated.h"

UCLASS()
class RAGEON_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:
	AMainCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RollAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MeditateAction;

	UPROPERTY()
	UStaticMeshComponent* SamuraiMeshComp;
	
	UPROPERTY()
	class APlayerControllerClass* PlayerController;
	
	float RageMeter = 0;
	float RagePerAttack = 10;
	float RageDecay = 1;
	float MaxRage = 100;
	
	float AttackRange = 100;
	float AttackDamage = 20;
	float AttackRate = 0.5f;
	float AttackTimer = 0;
	
	bool IsAttacking = false;
	bool IsMeditating = false;

	FTimerHandle MeditationTimerHandle;
	
	void Move(const struct FInputActionValue& Value);

	void TurnCamera(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);
	
	void Roll(const FInputActionValue& Value);

	void Meditate(const FInputActionValue& Value);

	void MedicationActivated();
	
	void DecreaseRage();
};
