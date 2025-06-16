#include "Enemy.h"
#include "PathSplineActor.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    SetRootComponent(MeshComp);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
    if (APathSplineActor* PathActor = Cast<APathSplineActor>(
        UGameplayStatics::GetActorOfClass(
            GetWorld(), APathSplineActor::StaticClass())))
    {
        PathSpline = PathActor->GetSplineComponent();
    }
    if (!PathSpline)
    {
        UE_LOG(LogTemp, Warning, TEXT("AEnemy::BeginPlay - PathSpline not found!"));
    }

    // 2) 메시 반 높이 계산 (로컬 바운드 × 컴포넌트 Z 스케일)
    if (MeshComp && MeshComp->GetStaticMesh())
    {
        float LocalHalfZ = MeshComp->Bounds.BoxExtent.Z;
        MeshHalfHeight = LocalHalfZ * MeshComp->GetComponentScale().Z;
    }

    // 3) 시작 위치·회전 초기화 (선택 사항)
    if (PathSpline)
    {
        FVector StartPos = PathSpline->GetLocationAtDistanceAlongSpline(
            0.f, ESplineCoordinateSpace::World);
        FRotator StartRot = PathSpline->GetRotationAtDistanceAlongSpline(
            0.f, ESplineCoordinateSpace::World);

        // 높이 오프셋 적용
        StartPos.Z += MeshHalfHeight;

        SetActorLocationAndRotation(StartPos, StartRot);
    }
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!PathSpline) return;

    // 1) 거리 누적
    DistanceAlongSpline += MoveSpeed * DeltaTime;

    // 2) 도착 체크
    float SplineLen = PathSpline->GetSplineLength();
    if (DistanceAlongSpline >= SplineLen)
    {
        Destroy();
        return;
    }

    // 3) 스플라인 위치·회전 샘플링
    FVector NewPos = PathSpline->GetLocationAtDistanceAlongSpline(
        DistanceAlongSpline, ESplineCoordinateSpace::World);
    FRotator NewRot = PathSpline->GetRotationAtDistanceAlongSpline(
        DistanceAlongSpline, ESplineCoordinateSpace::World);

    // 4) Z 오프셋 적용
    NewPos.Z += MeshHalfHeight;

    // 5) 이동 반영
    SetActorLocationAndRotation(NewPos, NewRot);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}