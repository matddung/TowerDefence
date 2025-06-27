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
};
