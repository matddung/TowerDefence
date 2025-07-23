#include "TDPlayerController.h"
#include "TowerBase.h"
#include "GamePlayGameMode.h"
#include "PathSplineActor.h"
#include "FloatingSpawnActor.h"

#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "EngineUtils.h"
#include "Components/SplineComponent.h"

ATDPlayerController::ATDPlayerController()
{
    static ConstructorHelpers::FClassFinder<ATowerBase> AttackBP(TEXT("/Game/Assets/AttackTower/BP_AttackTower"));
    if (AttackBP.Succeeded())
    {
        AttackTowerClass = AttackBP.Class;
    }

    static ConstructorHelpers::FClassFinder<ATowerBase> SplashBP(TEXT("/Game/Assets/SplashTower/BP_SplashTower"));
    if (SplashBP.Succeeded())
    {
        SplashTowerClass = SplashBP.Class;
    }

    static ConstructorHelpers::FClassFinder<ATowerBase> CCBP(TEXT("/Game/Assets/CCTower/BP_CCTower"));
    if (CCBP.Succeeded())
    {
        CCTowerClass = CCBP.Class;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> SuccessMatObj(TEXT("/Game/Assets/Material/M_Success"));
    if (SuccessMatObj.Succeeded())
    {
        SuccessMaterial = SuccessMatObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> FailedMatObj(TEXT("/Game/Assets/Material/M_Failed"));
    if (FailedMatObj.Succeeded())
    {
        FailedMaterial = FailedMatObj.Object;
    }

    static ConstructorHelpers::FClassFinder<AFloatingSpawnActor> FeedbackBP(TEXT("/Game/UI/BP_FloatingSpawnActor.BP_FloatingSpawnActor_C"));
    if (FeedbackBP.Succeeded())
    {
        FeedbackTextActorClass = FeedbackBP.Class;
    }
}

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
        InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ATDPlayerController::HandleLeftClick);
    }
}

void ATDPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bIsPlacingTower && PreviewTower)
    {
        UpdatePreviewLocation();

        if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
        {
            if (GM->IsWaveInProgress())
            {
                CancelPlacingTower();
            }
        }
    }
}

void ATDPlayerController::HandleRightClick()
{
    if (bIsPlacingTower)
    {
        CancelPlacingTower();
        return;
    }

    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, true, Hit);

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

void ATDPlayerController::HandleLeftClick()
{
    if (bIsPlacingTower)
    {
        FinishPlacingTower();
        return;
    }

    if (SelectedTower)
    {
        SelectedTower->ShowMenu(false);
        SelectedTower = nullptr;
    }
}

void ATDPlayerController::StartPlacingTower(TSubclassOf<ATowerBase> TowerClass)
{
    if (!TowerClass)
    {
        return;
    }

    if (PreviewTower)
    {
        PreviewTower->Destroy();
        PreviewTower = nullptr;
    }

    PreviewTower = GetWorld()->SpawnActor<ATowerBase>(TowerClass);
    if (PreviewTower)
    {
        PreviewTower->SetActorEnableCollision(false);
        PreviewTower->SetActorHiddenInGame(true);
    }

    bIsPlacingTower = true;
}

void ATDPlayerController::UpdatePreviewLocation()
{
    if (!PreviewTower)
    {
        return;
    }

    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, true, Hit);
    FVector Loc = Hit.Location;
    PreviewTower->SetActorLocation(Loc);

    bCanPlaceTower = CanPlaceTowerAt(Loc);

    if (SuccessMaterial && FailedMaterial)
    {
        TArray<UStaticMeshComponent*> Meshes;
        PreviewTower->GetComponents<UStaticMeshComponent>(Meshes);
        for (UStaticMeshComponent* Mesh : Meshes)
        {
            int32 NumMats = Mesh->GetNumMaterials();
            for (int32 i = 0; i < NumMats; ++i)
            {
                Mesh->SetMaterial(i, bCanPlaceTower ? SuccessMaterial : FailedMaterial);
            }
        }
    }

    PreviewTower->SetActorHiddenInGame(false);
}

bool ATDPlayerController::CanPlaceTowerAt(const FVector& Loc) const
{
    if (!PreviewTower)
    {
        return false;
    }

    FVector PreviewOrigin, PreviewExtent;
    PreviewTower->GetActorBounds(false, PreviewOrigin, PreviewExtent);
    FBox PreviewBox(PreviewOrigin - PreviewExtent, PreviewOrigin + PreviewExtent);

    for (TActorIterator<ATowerBase> It(GetWorld()); It; ++It)
    {
        if (*It != PreviewTower)
        {
            FVector OtherOrigin, OtherExtent;
            It->GetActorBounds(false, OtherOrigin, OtherExtent);
            FBox OtherBox(OtherOrigin - OtherExtent, OtherOrigin + OtherExtent);
            if (PreviewBox.Intersect(OtherBox))
            {
                return false;
            }
        }
    }

    if (APathSplineActor* PathActor = Cast<APathSplineActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APathSplineActor::StaticClass())))
    {
        if (USplineComponent* PathSpline = PathActor->GetSplineComponent())
        {
            float Key = PathSpline->FindInputKeyClosestToWorldLocation(Loc);
            FVector SplineLoc = PathSpline->GetLocationAtSplineInputKey(Key, ESplineCoordinateSpace::World);
            const float SplineBuffer = 1150.f;
            if (FVector::DistSquared(SplineLoc, Loc) < SplineBuffer * SplineBuffer)
            {
                return false;
            }
        }
    }

    return true;
}

void ATDPlayerController::SpawnFeedbackText(const FText& Text, const FVector& Location)
{
    if (!FeedbackTextActorClass)
    {
        return;
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (AFloatingSpawnActor* Feedback = GetWorld()->SpawnActor<AFloatingSpawnActor>(FeedbackTextActorClass, Location, FRotator::ZeroRotator, Params))
    {
        Feedback->SetText(Text);
    }
}

void ATDPlayerController::FinishPlacingTower()
{
    if (!PreviewTower)
    {
        bIsPlacingTower = false;
        return;
    }

    bool bPlaced = false;
    if (bCanPlaceTower && CanPlaceTowerAt(PreviewTower->GetActorLocation()))
    {
        if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
        {
            int32 Cost = PreviewTower->GetBuildCost();
            if (GM->SpendGold(Cost))
            {
                FVector Loc = PreviewTower->GetActorLocation();
                FRotator Rot = PreviewTower->GetActorRotation();
                GetWorld()->SpawnActor<ATowerBase>(PreviewTower->GetClass(), Loc, Rot);
                bPlaced = true;
            }
        }
    }

    FVector FeedbackLoc = PreviewTower->GetActorLocation() + FVector(0.f, 0.f, 100.f);
    if (bPlaced)
    {
        SpawnFeedbackText(FText::FromString(TEXT("Tower Built")), FeedbackLoc);
    }
    else
    {
        SpawnFeedbackText(FText::FromString(TEXT("Cannot Build")), FeedbackLoc);
    }

    PreviewTower->Destroy();
    PreviewTower = nullptr;
    bIsPlacingTower = false;
}

void ATDPlayerController::CancelPlacingTower()
{
    if (PreviewTower)
    {
        PreviewTower->Destroy();
        PreviewTower = nullptr;
    }

    bIsPlacingTower = false;
}

void ATDPlayerController::CancelTowerPlacement()
{
    CancelPlacingTower();
}

void ATDPlayerController::StartPlacingAttackTower()
{
    StartPlacingTower(AttackTowerClass);
}

void ATDPlayerController::StartPlacingSplashTower()
{
    StartPlacingTower(SplashTowerClass);
}

void ATDPlayerController::StartPlacingCCTower()
{
    StartPlacingTower(CCTowerClass);
}