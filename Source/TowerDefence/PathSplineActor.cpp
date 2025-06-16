// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSplineActor.h"

// Sets default values
APathSplineActor::APathSplineActor()
{
    // �� ������ ƽ�� �ʿ� ������ ����
    PrimaryActorTick.bCanEverTick = false;

    // SplineComponent ���� & ��Ʈ�� ����
    PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
    SetRootComponent(PathSpline);

    // �����Ϳ��� ControlPoint ������ OnConstruction ���� �ٷ� �ݿ��ǵ���
    PathSpline->bInputSplinePointsToConstructionScript = true;

}

// Called when the game starts or when spawned
void APathSplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}