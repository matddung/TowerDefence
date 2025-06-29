#include "GamePlayGameMode.h"
#include "TDPlayerController.h"
#include "GameHUDWidget.h"
#include "Enemy.h"
#include "Portal.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

AGamePlayGameMode::AGamePlayGameMode()
{
    PlayerControllerClass = ATDPlayerController::StaticClass();

    static ConstructorHelpers::FClassFinder<UGameHUDWidget> HUDWidgetBPClass(TEXT("/Game/UI/WBP_GameHUD"));
    if (HUDWidgetBPClass.Succeeded())
    {
        GameHUDWidgetClass = HUDWidgetBPClass.Class;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> WaveDataObj(TEXT("/Game/Data/EnemyWaveData"));
    if (WaveDataObj.Succeeded())
    {
        WaveDataTable = WaveDataObj.Object;
    }

    static ConstructorHelpers::FClassFinder<AEnemy> EnemyBPClass(TEXT("/Game/Blueprints/Enemy/BP_Enemy"));
    if (EnemyBPClass.Succeeded())
    {
        EnemyClass = EnemyBPClass.Class;
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

void AGamePlayGameMode::StartNextWave()
{
    if (AliveEnemyCount > 0)
    {
        if (GameHUDWidget)
        {
            GameHUDWidget->SetStartButtonEnabled(false);
        }
        return;
    }

    ++CurrentWave;

    if (!WaveDataTable)
    {
        return;
    }

    FName RowName = FName(*FString::Printf(TEXT("Round%d"), CurrentWave));
    const FEnemyWaveData* Data = WaveDataTable->FindRow<FEnemyWaveData>(RowName, TEXT("StartNextWave Lookup"));
    if (!Data)
    {
        return;
    }

    SpawnPortal = Cast<APortal>(UGameplayStatics::GetActorOfClass(this, APortal::StaticClass()));
    if (!SpawnPortal)
    {
        return;
    }

    RemainingSpawnCount = Data->SpawnCount;
    CurrentWaveRowName = RowName;

    if (GameHUDWidget)
    {
        GameHUDWidget->SetStartButtonEnabled(false);
    }

    SpawnEnemy();
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGamePlayGameMode::SpawnEnemy, 2.0f, true);
}

void AGamePlayGameMode::SpawnEnemy()
{
    if (RemainingSpawnCount <= 0)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    if (!SpawnPortal)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    FVector SpawnLoc = SpawnPortal->GetActorLocation();
    FRotator SpawnRot = SpawnPortal->GetActorRotation();
    if (AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLoc, SpawnRot))
    {
        NewEnemy->WaveDataTable = WaveDataTable;
        NewEnemy->WaveRowName = CurrentWaveRowName;
        NewEnemy->OnDestroyed.AddDynamic(this, &AGamePlayGameMode::HandleEnemyDestroyed);
        ++AliveEnemyCount;
    }

    --RemainingSpawnCount;
    if (RemainingSpawnCount <= 0)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }
}

void AGamePlayGameMode::HandleEnemyDestroyed(AActor* DestroyedActor)
{
    --AliveEnemyCount;
    if (AliveEnemyCount <= 0)
    {
        AliveEnemyCount = 0;
        if (GameHUDWidget)
        {
            GameHUDWidget->SetStartButtonEnabled(true);
        }
    }
}