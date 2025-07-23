#include "GameOverGameMode.h"
#include "TDPlayerController.h"
#include "GameOverWidget.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AGameOverGameMode::AGameOverGameMode()
{
    PlayerControllerClass = ATDPlayerController::StaticClass();

    static ConstructorHelpers::FClassFinder<UGameOverWidget> WidgetBPClass(TEXT("/Game/UI/WBP_GameOver"));
    if (WidgetBPClass.Succeeded())
    {
        GameOverWidgetClass = WidgetBPClass.Class;
    }
}

void AGameOverGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GameOverWidgetClass)
    {
        GameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidgetClass);
        if (GameOverWidget)
        {
            GameOverWidget->AddToViewport();
        }
    }

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
}