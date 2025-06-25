#include "GamePlayGameMode.h"
#include "TDPlayerController.h"
#include "GameHUDWidget.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Blueprint/UserWidget.h"

AGamePlayGameMode::AGamePlayGameMode()
{
    PlayerControllerClass = ATDPlayerController::StaticClass();

    static ConstructorHelpers::FClassFinder<UGameHUDWidget> HUDWidgetBPClass(TEXT("/Game/UI/WBP_GameHUD"));
    if (HUDWidgetBPClass.Succeeded())
    {
        GameHUDWidgetClass = HUDWidgetBPClass.Class;
    }
}

void AGamePlayGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
        {
            if (It->GetName().Contains("FixedViewCamera"))
            {
                PC->SetViewTarget(*It);
                break;
            }
        }

        UGameHUDWidget* HUD = CreateWidget<UGameHUDWidget>(PC, GameHUDWidgetClass);
        if (HUD)
        {
            HUD->AddToViewport();
        }
    }
}