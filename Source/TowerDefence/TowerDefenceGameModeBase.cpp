#include "TowerDefenceGameModeBase.h"
#include "TDPlayerController.h"
#include "MainMenuWidget.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ATowerDefenceGameModeBase::ATowerDefenceGameModeBase()
{
    PlayerControllerClass = ATDPlayerController::StaticClass();

    static ConstructorHelpers::FClassFinder<UMainMenuWidget> MenuWidgetBPClass(TEXT("/Game/UI/WBP_MainMenu"));
    if (MenuWidgetBPClass.Succeeded())
    {
        MainMenuWidgetClass = MenuWidgetBPClass.Class;
    }
}

void ATowerDefenceGameModeBase::BeginPlay()
{
    Super::BeginPlay();
        
    if (MainMenuWidgetClass)
    {
        UMainMenuWidget* Menu = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
        if (Menu)
        {
            Menu->AddToViewport();

            APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(Menu->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
            }
        }
    }
}