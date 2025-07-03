#include "Enemy.h"
#include "PathSplineActor.h"
#include "EnemyAnimInstance.h"
#include "EnemyHPBarWidget.h"
#include "FloatingSpawnActor.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Components/WidgetComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
    HPBarWidget->SetupAttachment(RootComponent);
    HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    HPBarWidget->SetDrawSize(FVector2D(100.f, 10.f));
    HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    HPBarWidget->SetWidgetClass(UEnemyHPBarWidget::StaticClass());

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/Blueprints/Enemy/ABP_Enemy"));
    if (AnimBPClass.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
    }

    static ConstructorHelpers::FClassFinder<AFloatingSpawnActor> DamageTextBPClass(TEXT("/Game/UI/BP_FloatingSpawnActor.BP_FloatingSpawnActor_C"));
    if (DamageTextBPClass.Succeeded())
    {
        DamageTextActorClass = DamageTextBPClass.Class;
    }
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (WaveDataTable)
    {
        const FEnemyWaveData* Data = WaveDataTable->FindRow<FEnemyWaveData>(WaveRowName, TEXT("EnemyWaveData Lookup"));
        if (Data)
        {
            WaveData = *Data;
            CurrentHealth = WaveData.Health;
        }
    }

    if (APathSplineActor* PathActor = Cast<APathSplineActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APathSplineActor::StaticClass())))
    {
        PathSpline = PathActor->GetSplineComponent();
    }

    if (auto* BI = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()))
    {
        AnimInst = BI;
    }

    CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    if (PathSpline)
    {
        FVector StartPos = PathSpline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World);
        FRotator StartRot = PathSpline->GetRotationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World);

        StartPos.Z += CapsuleHalfHeight;
        SetActorLocationAndRotation(StartPos, StartRot);
    }

    if (AnimInst)
    {
        AnimInst->Speed = WaveData.MoveSpeed;
    }

    UpdateHPBar();

    GetWorldTimerManager().SetTimer(
        DamageTimerHandle,
        this,
        &AEnemy::ApplyTestDamage,
        1.0f,
        true
    );
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PathSpline) return;

    DistanceAlongSpline += WaveData.MoveSpeed * DeltaTime;
    float SplineLen = PathSpline->GetSplineLength();

    if (DistanceAlongSpline < SplineLen)
    {
        FVector NewPos = PathSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
        NewPos.Z += CapsuleHalfHeight;
        FRotator NewRot = PathSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

        SetActorLocationAndRotation(NewPos, NewRot);

        if (AnimInst)
            AnimInst->Speed = WaveData.MoveSpeed;

        return;
    }

    if (AnimInst)
        AnimInst->Speed = 0.f;

    auto& TM = GetWorldTimerManager();
    if (!TM.IsTimerActive(AttackTimerHandle))
    {
        DoAttack();

        TM.SetTimer(
            AttackTimerHandle,
            this, &AEnemy::DoAttack,
            WaveData.AttackInterval,
            true
        );
    }
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (ActualDamage <= 0.f)
    {
        return 0.f;
    }

    CurrentHealth -= ActualDamage;
    CurrentHealth = FMath::Max(CurrentHealth, 0.f);

    SpawnDamageText(this, ActualDamage);

    UpdateHPBar();

    if (CurrentHealth <= 0.f)
    {
        if (AnimInst)
        {
            AnimInst->SetDeadAnim();
        }

        Destroy();
    }

    return ActualDamage;
}

void AEnemy::DoAttack()
{
    if (AnimInst)
    {
        float Duration = AnimInst->PlayAttackMontage();
    }

    if (AnimInst && AnimInst->AttackMontage)
    {
        AnimInst->PlayAttackMontage();
    }
}

void AEnemy::UpdateHPBar()
{
    if (HPBarWidget)
    {
        if (UEnemyHPBarWidget* Widget = Cast<UEnemyHPBarWidget>(HPBarWidget->GetUserWidgetObject()))
        {
            float MaxHealth = FMath::Max(WaveData.Health, 1.0f);
            Widget->SetHPPercent(CurrentHealth / MaxHealth);
        }
    }
}

void AEnemy::ApplyTestDamage()
{
    FDamageEvent DamageEvent;
    TakeDamage(7.f, DamageEvent, nullptr, nullptr);
}

void AEnemy::SpawnDamageText(AActor* DamagedActor, float Damage)
{
    if (!DamageTextActorClass)
    {
        UE_LOG(LogTemp, Error, TEXT("DamageTextActorClass is nullptr"));
    }
    if (!DamagedActor)
    {
        UE_LOG(LogTemp, Error, TEXT("DamagedActor is nullptr"));
    }
    if (!DamageTextActorClass || !DamagedActor)
    {
        return;
    }

    FVector TargetLocation = DamagedActor->GetActorLocation() + FVector(0.f, 0.f, 100.f);
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AFloatingSpawnActor* DamageText = GetWorld()->SpawnActor<AFloatingSpawnActor>(DamageTextActorClass, TargetLocation, FRotator::ZeroRotator, Params);
    if (DamageText)
    {
        DamageText->SetDamage(Damage);
    }
}