#include "TowerBase.h"

#include "Components/StaticMeshComponent.h"

ATowerBase::ATowerBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	RootComponent = Mesh;


}

void ATowerBase::BeginPlay()
{
	Super::BeginPlay();


}