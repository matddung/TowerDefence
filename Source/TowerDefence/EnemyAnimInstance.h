#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed = 0.f;

    UFUNCTION(BlueprintCallable)
    void SetDeadAnim() { bIsDead = true; }

    UFUNCTION(BlueprintCallable)
    bool GetIsDead() const { return bIsDead; }

public:
    UFUNCTION(BlueprintCallable, Category = "Attack")
    float PlayAttackMontage();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    bool bIsDead = false;
};