#include "TowerBase.h"
#include "Enemy.h"
#include "GamePlayGameMode.h"
#include "TowerMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "InputCoreTypes.h"

ATowerBase::ATowerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	RootComponent = Mesh;

	MenuWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MenuWidget"));
	MenuWidgetComponent->SetupAttachment(RootComponent);
	MenuWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	MenuWidgetComponent->SetDrawSize(FVector2D(120.f, 60.f));
	MenuWidgetComponent->SetVisibility(false);
}

void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		MenuWidgetComponent->SetWidgetClass(MenuWidgetClass);
		if (UTowerMenuWidget* Menu = Cast<UTowerMenuWidget>(MenuWidgetComponent->GetUserWidgetObject()))
		{
			Menu->Init(this);
		}
	}

	if (Mesh)
	{
		Mesh->OnClicked.AddDynamic(this, &ATowerBase::OnMeshClicked);
	}

	GetWorldTimerManager().SetTimer(SearchTimerHandle, this, &ATowerBase::FindTargetEnemy, 1.0f, true);

	if (AttackInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATowerBase::Attack, AttackInterval, true);
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

	UGameplayStatics::ApplyDamage(CurrentTarget, Damage, nullptr, this, nullptr);
}

void ATowerBase::ShowMenu(bool bShow)
{
	if (MenuWidgetComponent)
	{
		if (bShow)
		{
			if (UTowerMenuWidget* Menu = Cast<UTowerMenuWidget>(MenuWidgetComponent->GetUserWidgetObject()))
			{
				Menu->UpdateInfo();
			}
		}

		MenuWidgetComponent->SetVisibility(bShow);
	}
}

void ATowerBase::OnMeshClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::RightMouseButton)
	{
		bool bShow = !MenuWidgetComponent->IsVisible();
		ShowMenu(bShow);
	}
}

void ATowerBase::SellTower()
{
	if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
	{
		if (GM->IsWaveInProgress())
		{
			return;
		}

		int32 Refund = FMath::RoundToInt(GetBuildCost(CurrentLevel) * 0.7f);
		GM->AddGold(Refund);
	}

	Destroy();
}

void ATowerBase::UpgradeTower()
{
	int32 NextLevel = CurrentLevel + 1;
	int32 Cost = GetBuildCost(NextLevel);
	if (Cost <= 0)
	{
		return;
	}

	if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
	{
		if (GM->IsWaveInProgress())
		{
			return;
		}

		if (!GM->SpendGold(Cost))
		{
			return;
		}
	}

	CurrentLevel = NextLevel;
	ReloadData();
	ShowMenu(false);
}