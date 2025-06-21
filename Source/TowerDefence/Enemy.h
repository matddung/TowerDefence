#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

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
