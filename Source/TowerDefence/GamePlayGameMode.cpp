#include "GamePlayGameMode.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"

AGamePlayGameMode::AGamePlayGameMode()
{

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

        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeGameOnly());
    }
}