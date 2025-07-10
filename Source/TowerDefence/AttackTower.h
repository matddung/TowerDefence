#pragma once

#include "CoreMinimal.h"
#include "TowerBase.h"
#include "AttackTower.generated.h"

USTRUCT(BlueprintType)
struct FAttackTowerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BuildCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackInterval;
};

UCLASS()
class TOWERDEFENCE_API AAttackTower : public ATowerBase
{
	GENERATED_BODY()
	
public:
	AAttackTower();

	virtual int32 GetBuildCost(int32 Level = -1) const override;
	virtual void ReloadData() override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	UDataTable* TowerDataTable;

	UPROPERTY(VisibleAnywhere)
	FAttackTowerData TowerData;

	FName TowerRowName = "1";
};