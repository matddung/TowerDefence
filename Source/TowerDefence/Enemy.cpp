#include "Enemy.h"
#include "PathSplineActor.h"
#include "EnemyAnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/Blueprints/Enemy/ABP_Enemy"));
    if (AnimBPClass.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
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

            UE_LOG(LogTemp, Log, TEXT("WaveData loaded: Health=%.1f, Speed=%.1f, AttackInterval=%.1f"),
                WaveData.Health, WaveData.MoveSpeed, WaveData.AttackInterval);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("WaveData row '%s' not found!"), *WaveRowName.ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WaveDataTable not assigned in AEnemy!"));

    }
    if (APathSplineActor* PathActor = Cast<APathSplineActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APathSplineActor::StaticClass())))
    {
        PathSpline = PathActor->GetSplineComponent();
    }

    if (!PathSpline)
    {
        UE_LOG(LogTemp, Warning, TEXT("AEnemy::BeginPlay - PathSpline not found!"));
    }

    if (auto* BI = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()))
    {
        AnimInst = BI;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is not UEnemyAnimInstance"));
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

void AEnemy::DoAttack()
{
    if (AnimInst)
    {
        float Duration = AnimInst->PlayAttackMontage();
        UE_LOG(LogTemp, Log, TEXT("AttackMontage played, duration=%.2f"), Duration);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DoAttack: AnimInst is null"));
    }

    if (AnimInst && AnimInst->AttackMontage)
    {
        AnimInst->PlayAttackMontage();
    }
}