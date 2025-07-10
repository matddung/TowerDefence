#include "TDPlayerController.h"
#include "TowerBase.h"

#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"

void ATDPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
}

void ATDPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ATDPlayerController::HandleRightClick);
    }
}

void ATDPlayerController::HandleRightClick()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    ATowerBase* ClickedTower = Cast<ATowerBase>(Hit.GetActor());

    if (SelectedTower && SelectedTower != ClickedTower)
    {
        SelectedTower->ShowMenu(false);
        SelectedTower = nullptr;
    }

    if (ClickedTower)
    {
        bool bWasSelected = (ClickedTower == SelectedTower);
        ClickedTower->ShowMenu(!bWasSelected);
        SelectedTower = bWasSelected ? nullptr : ClickedTower;
    }
}