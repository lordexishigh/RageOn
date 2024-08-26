// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerClass.generated.h"

/**
 * 
 */
UCLASS()
class RAGEON_API APlayerControllerClass : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRageUI(float RagePercent);
};
