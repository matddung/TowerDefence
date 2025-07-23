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
#include "Components/WidgetComponent.h"

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

        GameHUDWidget = CreateWidget<UGameHUDWidget>(PC, GameHUDWidgetClass);
        if (GameHUDWidget)
        {
            GameHUDWidget->AddToViewport();
            GameHUDWidget->SetGoldAmount(Gold);
            GameHUDWidget->SetTowerHealth(TowerHealth);
        }
    }
}

void AGamePlayGameMode::StartNextWave()
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (ATDPlayerController* TDPC = Cast<ATDPlayerController>(PC))
        {
            TDPC->CancelTowerPlacement();
        }
    }

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
    bWaveInProgress = true;
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGamePlayGameMode::SpawnEnemy, Data->SpawnInterval, true);
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
        bWaveInProgress = false;
        if (GameHUDWidget)
        {
            GameHUDWidget->SetStartButtonEnabled(true);
        }

        AddGold(WaveClearReward);
    }
}

bool AGamePlayGameMode::SpendGold(int32 Amount)
{
    if (Gold < Amount)
    {
        return false;
    }

    Gold -= Amount;

    if (GameHUDWidget)
    {
        GameHUDWidget->SetGoldAmount(Gold);
    }

    return true;
}

void AGamePlayGameMode::AddGold(int32 Amount)
{
    Gold += Amount;

    if (GameHUDWidget)
    {
        GameHUDWidget->SetGoldAmount(Gold);
    }
}

void AGamePlayGameMode::DecreaseTowerHealth(int32 Amount)
{
    TowerHealth -= Amount;
    TowerHealth = FMath::Max(0, TowerHealth);

    if (GameHUDWidget)
    {
        GameHUDWidget->SetTowerHealth(TowerHealth);
    }

    if (TowerHealth <= 0 && !bTowerDestroyed)
    {
        bTowerDestroyed = true;

        if (GameHUDWidget)
        {
            GameHUDWidget->RemoveFromParent();
            GameHUDWidget = nullptr;
        }

        for (TActorIterator<AEnemy> It(GetWorld()); It; ++It)
        {
            if (It->HPBarWidget)
            {
                It->HPBarWidget->SetVisibility(false);
            }
        }


        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            if (PC->PlayerCameraManager)
            {
                PC->PlayerCameraManager->StartCameraFade(0.f, 1.f, 1.f, FLinearColor::Gray, false, true);
            }
        }

        GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &AGamePlayGameMode::HandleGameOver, 3.f, false);
    }
}

void AGamePlayGameMode::HandleGameOver()
{
    const FString Options = TEXT("game=/Script/TowerDefence.GameOverGameMode");
    UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Maps/GameOverMap")), true, Options);
}
