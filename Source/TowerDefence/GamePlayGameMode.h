// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API AGamePlayGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGamePlayGameMode();

protected:
	virtual void BeginPlay() override;
};
