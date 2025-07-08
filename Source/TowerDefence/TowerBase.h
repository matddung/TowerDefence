#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TowerBase.generated.h"

USTRUCT(BlueprintType)
struct FTowerData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TowerCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackInterval;
};

UCLASS()
class TOWERDEFENCE_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:
	ATowerBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void FindTargetEnemy();
	virtual void RotateToTarget(float DeltaTime);
	virtual void Attack();

protected:
	UPROPERTY()
	class AEnemy* CurrentTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float TargetingRange;

	FTimerHandle SearchTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tower")
	int32 TowerCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* TowerDataTable;

	UPROPERTY(VisibleAnywhere)
	FTowerData TowerData;

	UPROPERTY(EditDefaultsOnly)
	FName TowerRowName;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UNiagaraSystem* AttackEffect;
};