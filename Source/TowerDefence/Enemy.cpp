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

    // 2) �޽� �� ���� ��� (���� �ٿ�� �� ������Ʈ Z ������)
    if (MeshComp && MeshComp->GetStaticMesh())
    {
        float LocalHalfZ = MeshComp->Bounds.BoxExtent.Z;
        MeshHalfHeight = LocalHalfZ * MeshComp->GetComponentScale().Z;
    }

    // 3) ���� ��ġ��ȸ�� �ʱ�ȭ (���� ����)
    if (PathSpline)
    {
        FVector StartPos = PathSpline->GetLocationAtDistanceAlongSpline(
            0.f, ESplineCoordinateSpace::World);
        FRotator StartRot = PathSpline->GetRotationAtDistanceAlongSpline(
            0.f, ESplineCoordinateSpace::World);

        // ���� ������ ����
        StartPos.Z += MeshHalfHeight;

        SetActorLocationAndRotation(StartPos, StartRot);
    }
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!PathSpline) return;

    // 1) �Ÿ� ����
    DistanceAlongSpline += MoveSpeed * DeltaTime;

    // 2) ���� üũ
    float SplineLen = PathSpline->GetSplineLength();
    if (DistanceAlongSpline >= SplineLen)
    {
        Destroy();
        return;
    }

    // 3) ���ö��� ��ġ��ȸ�� ���ø�
    FVector NewPos = PathSpline->GetLocationAtDistanceAlongSpline(
        DistanceAlongSpline, ESplineCoordinateSpace::World);
    FRotator NewRot = PathSpline->GetRotationAtDistanceAlongSpline(
        DistanceAlongSpline, ESplineCoordinateSpace::World);

    // 4) Z ������ ����
    NewPos.Z += MeshHalfHeight;

    // 5) �̵� �ݿ�
    SetActorLocationAndRotation(NewPos, NewRot);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}