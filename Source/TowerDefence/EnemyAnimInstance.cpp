#include "EnemyAnimInstance.h"

float UEnemyAnimInstance::PlayAttackMontage()
{
    if (AttackMontage)
    {
        return Montage_Play(AttackMontage);
    }
    return 0.f;
}