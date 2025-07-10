#pragma once

#include "CoreMinimal.h"
#include "TowerBase.h"
#include "CCTower.generated.h"

USTRUCT(BlueprintType)
struct FCCTowerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BuildCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SlowlyPercent;
};

UCLASS()
class TOWERDEFENCE_API ACCTower : public ATowerBase
{
	GENERATED_BODY()

public:
	ACCTower();

	virtual int32 GetBuildCost(int32 Level = -1) const override;
	virtual void ReloadData() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void FindTargetEnemy() override;

	virtual void Attack() override;

public:
	UPROPERTY(EditAnywhere)
	UDataTable* TowerDataTable;

	UPROPERTY(VisibleAnywhere)
	FCCTowerData TowerData;

	FName TowerRowName = "1";
};
