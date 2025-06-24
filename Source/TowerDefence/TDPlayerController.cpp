#include "TDPlayerController.h"

void ATDPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    FInputModeUIOnly InputMode;
    SetInputMode(InputMode);
}