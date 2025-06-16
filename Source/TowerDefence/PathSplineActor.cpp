// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSplineActor.h"

// Sets default values
APathSplineActor::APathSplineActor()
{
    // 매 프레임 틱은 필요 없으니 끄기
    PrimaryActorTick.bCanEverTick = false;

    // SplineComponent 생성 & 루트로 설정
    PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
    SetRootComponent(PathSpline);

    // 에디터에서 ControlPoint 편집이 OnConstruction 으로 바로 반영되도록
    PathSpline->bInputSplinePointsToConstructionScript = true;

}

// Called when the game starts or when spawned
void APathSplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}