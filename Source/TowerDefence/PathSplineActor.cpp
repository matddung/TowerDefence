#include "PathSplineActor.h"

APathSplineActor::APathSplineActor()
{
    PrimaryActorTick.bCanEverTick = false;

    PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
    SetRootComponent(PathSpline);

    PathSpline->bInputSplinePointsToConstructionScript = true;

}

void APathSplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}