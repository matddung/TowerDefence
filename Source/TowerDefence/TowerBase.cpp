#include "TowerBase.h"
#include "Enemy.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"

ATowerBase::ATowerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	RootComponent = Mesh;
}

void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SearchTimerHandle, this, &ATowerBase::FindTargetEnemy, 1.0f, true);

	if (TowerData.AttackInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATowerBase::Attack, TowerData.AttackInterval, true);
	}
}

void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget)
	{
		RotateToTarget(DeltaTime);
	}
}

void ATowerBase::FindTargetEnemy()
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

	AEnemy* ClosestEnemy = nullptr;
	float ClosestDistanceSq = TargetingRange * TargetingRange;

	for (AActor* Actor : FoundEnemies)
	{
		float DistSq = FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation());
		if (DistSq < ClosestDistanceSq)
		{
			ClosestEnemy = Cast<AEnemy>(Actor);
			ClosestDistanceSq = DistSq;
		}
	}

	CurrentTarget = ClosestEnemy;
}

void ATowerBase::RotateToTarget(float DeltaTime)
{
	if (!CurrentTarget) return;

	FVector Direction = CurrentTarget->GetActorLocation() - GetActorLocation();
	FRotator TargetRotation = Direction.Rotation();
	FRotator CurrentRotation = GetActorRotation();

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10);
	SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
}

void ATowerBase::Attack()
{
	if (!CurrentTarget) return;

	if (AttackEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackEffect, CurrentTarget->GetActorLocation());
	}

	UGameplayStatics::ApplyDamage(CurrentTarget, TowerData.Damage, nullptr, this, nullptr);
}