#pragma once

#include "CoreMinimal.h"
#include "TowerBase.h"
#include "AttackTower.generated.h"

UCLASS()
class TOWERDEFENCE_API AAttackTower : public ATowerBase
{
	GENERATED_BODY()
	
public:
	AAttackTower();

protected:
	virtual void BeginPlay() override;

};