#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Enemy.generated.h"

USTRUCT(BlueprintType)
struct FEnemyWaveData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float AttackInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 SpawnCount;
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

	UPROPERTY()
	class USplineComponent* PathSpline = nullptr;

	float DistanceAlongSpline = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 3000;

	float CapsuleHalfHeight = 0.f;

	UPROPERTY()
	class UEnemyAnimInstance* AnimInst = nullptr;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackInterval = 2;

	void DoAttack();
};
