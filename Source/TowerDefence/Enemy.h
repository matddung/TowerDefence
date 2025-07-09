#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Enemy.generated.h"

USTRUCT(BlueprintType)
struct FEnemyWaveData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval;
};

UCLASS()
class TOWERDEFENCE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY()
	class USplineComponent* PathSpline = nullptr;

	float DistanceAlongSpline = 0;

	float CapsuleHalfHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth = 0;

	UPROPERTY()
	class UEnemyAnimInstance* AnimInst = nullptr;

	FTimerHandle AttackTimerHandle;

	void DoAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FEnemyWaveData WaveData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName WaveRowName = "Round1";

	UPROPERTY(EditAnywhere, Category = "Wave")
	UDataTable* WaveDataTable = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* HPBarWidget = nullptr;

	void UpdateHPBar();

	UPROPERTY(EditAnywhere, Category = "DamageText")
	TSubclassOf<class AFloatingSpawnActor> DamageTextActorClass;

	void SpawnDamageText(AActor* DamagedActor, float Damage);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSlowed = false;

	float OriginalMoveSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentMoveSpeed = 0.f;

	void ApplySlow(float Percent);
};
