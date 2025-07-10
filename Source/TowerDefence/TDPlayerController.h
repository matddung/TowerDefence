#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDPlayerController.generated.h"

UCLASS()
class TOWERDEFENCE_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

    void HandleRightClick();

    UPROPERTY()
    class ATowerBase* SelectedTower = nullptr;
};