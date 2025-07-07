// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerBase.h"
#include "CCTower.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API ACCTower : public ATowerBase
{
	GENERATED_BODY()

public:
	ACCTower();

protected:
	virtual void BeginPlay() override;
};
