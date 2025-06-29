// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    void SetHPPercent(float Percent);

protected:
    UPROPERTY()
    class UProgressBar* HPBar = nullptr;
};
