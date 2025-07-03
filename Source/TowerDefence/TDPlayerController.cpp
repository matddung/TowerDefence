#include "TDPlayerController.h"
#include "TowerBase.h"
#include "TowerPlacementIndicator.h"

#include "Kismet/GameplayStatics.h"

void ATDPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    FInputModeUIOnly InputMode;
    SetInputMode(InputMode);
}