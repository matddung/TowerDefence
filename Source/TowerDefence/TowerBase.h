#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TowerBase.generated.h"

UENUM(BlueprintType)
enum class ETowerType : uint8
{
	Attack,
	Splash,
	CC
};

USTRUCT(BlueprintType)
struct FTowerData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETowerType TowerType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level;

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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tower")
	int32 TowerCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tower")
	ETowerType TowerType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* TowerDataTable;

	UPROPERTY(VisibleAnywhere)
	FTowerData TowerData;

	UPROPERTY(EditDefaultsOnly)
	FName TowerRowName;
};