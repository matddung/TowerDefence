#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingSpawnActor.generated.h"

UCLASS()
class TOWERDEFENCE_API AFloatingSpawnActor : public AActor
{
    GENERATED_BODY()

public:
    AFloatingSpawnActor();

    void SetDamage(float Damage);

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    class UWidgetComponent* WidgetComponent;

    float LifeTime = 0.5f;
    
    float RiseSpeed = 50.0f;

private:
    float ElapsedTime = 0.0f;
};