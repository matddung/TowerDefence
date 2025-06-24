#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameModeBase.generated.h"

UCLASS()
class TOWERDEFENCE_API ATowerDefenceGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATowerDefenceGameModeBase();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;
};