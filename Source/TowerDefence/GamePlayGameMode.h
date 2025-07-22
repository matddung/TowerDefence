#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayGameMode.generated.h"

UCLASS()
class TOWERDEFENCE_API AGamePlayGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGamePlayGameMode();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartNextWave();

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	void HandleEnemyDestroyed(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable, Category = "Economy")
	bool SpendGold(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Economy")
	void AddGold(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Economy")
	int32 GetGold() const { return Gold; }

	bool IsWaveInProgress() const { return bWaveInProgress; }

	UFUNCTION(BlueprintCallable, Category = "Tower")
	void DecreaseTowerHealth(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Tower")
	int32 GetTowerHealth() const { return TowerHealth; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UGameHUDWidget> GameHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	class UDataTable* WaveDataTable = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Wave")
	int32 CurrentWave = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave")
	TSubclassOf<class AEnemy> EnemyClass;

	UPROPERTY()
	class APortal* SpawnPortal = nullptr;

	FTimerHandle SpawnTimerHandle;
	int32 RemainingSpawnCount = 0;
	FName CurrentWaveRowName;

	UPROPERTY()
	class UGameHUDWidget* GameHUDWidget = nullptr;

	int32 AliveEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	int32 Gold = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	int32 TowerHealth = 20;

	bool bWaveInProgress = false;
};
