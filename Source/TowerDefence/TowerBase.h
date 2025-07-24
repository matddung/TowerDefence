#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TowerBase.generated.h"

UCLASS()
class TOWERDEFENCE_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:
	ATowerBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void FindTargetEnemy();
	virtual void RotateToTarget(float DeltaTime);
	virtual void Attack();

	virtual int32 GetBuildCost(int32 Level = -1) const { return 0; }
	virtual void ReloadData() {}

	UFUNCTION()
	void SellTower();

	UFUNCTION()
	void UpgradeTower();

	void ShowMenu(bool bShow);

	UFUNCTION()
	void OnMeshClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

	int32 GetCurrentLevel() const { return CurrentLevel; }

protected:
	UPROPERTY()
	class AEnemy* CurrentTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float TargetingRange;

	FTimerHandle SearchTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentLevel = 1;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UNiagaraSystem* AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackInterval = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float Damage = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* MenuWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UTowerMenuWidget> MenuWidgetClass;

	template <typename RowType>
	int32 GetRowCost(const UDataTable* Table, int32 Level) const {
		if (!Table)
			return 0;

		FName Row = FName(*FString::FromInt(Level));
		if (const RowType* Data =
			Table->FindRow<RowType>(Row, TEXT("GetBuildCost"))) {
			return Data->BuildCost;
		}

		return 0;
	}
};