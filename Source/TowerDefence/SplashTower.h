#pragma once

#include "CoreMinimal.h"
#include "TowerBase.h"
#include "SplashTower.generated.h"

USTRUCT(BlueprintType)
struct FSplashTowerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BuildCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackInterval;
};

UCLASS()
class TOWERDEFENCE_API ASplashTower : public ATowerBase
{
	GENERATED_BODY()
	
public:
	ASplashTower();

	virtual int32 GetBuildCost(int32 Level = -1) const override;
	virtual void ReloadData() override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	UDataTable* TowerDataTable;

	UPROPERTY(VisibleAnywhere)
	FSplashTowerData TowerData;

	FName TowerRowName = "1";
};
