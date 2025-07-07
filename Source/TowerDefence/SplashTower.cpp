#include "SplashTower.h"

ASplashTower::ASplashTower()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Game/Data/TowerData/SplashTowerData"));
    if (DataTableObj.Succeeded())
    {
        TowerDataTable = DataTableObj.Object;
    }
}

void ASplashTower::BeginPlay()
{
    Super::BeginPlay();
}