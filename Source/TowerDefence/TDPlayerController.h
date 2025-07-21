#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"

class ATowerBase;

UCLASS()
class TOWERDEFENCE_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    ATDPlayerController();

protected:
    virtual void PlayerTick(float DeltaTime) override;
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

    void HandleRightClick();
    void HandleLeftClick();

    void StartPlacingTower(TSubclassOf<ATowerBase> TowerClass);
    void FinishPlacingTower();
    void CancelPlacingTower();

    void UpdatePreviewLocation();
    bool CanPlaceTowerAt(const FVector& Loc) const;

    bool bIsPlacingTower = false;
    bool bCanPlaceTower = false;
    UPROPERTY()
    ATowerBase* PreviewTower = nullptr;

public:
    void StartPlacingAttackTower();
    void StartPlacingSplashTower();
    void StartPlacingCCTower();

protected:
    TSubclassOf<ATowerBase> AttackTowerClass;
    TSubclassOf<ATowerBase> SplashTowerClass;
    TSubclassOf<ATowerBase> CCTowerClass;

    UPROPERTY()
    UMaterialInterface* SuccessMaterial = nullptr;

    UPROPERTY()
    UMaterialInterface* FailedMaterial = nullptr;

    UPROPERTY()
    ATowerBase* SelectedTower = nullptr;
};