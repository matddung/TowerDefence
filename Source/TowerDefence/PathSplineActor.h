#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PathSplineActor.generated.h"

UCLASS()
class TOWERDEFENCE_API APathSplineActor : public AActor
{
	GENERATED_BODY()
	
public:
	APathSplineActor();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE USplineComponent* GetSplineComponent() const { return PathSpline; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* PathSpline;
};
