#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameOverGameMode.generated.h"

UCLASS()
class TOWERDEFENCE_API AGameOverGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGameOverGameMode();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UGameOverWidget> GameOverWidgetClass;

    UPROPERTY()
    class UGameOverWidget* GameOverWidget = nullptr;
};