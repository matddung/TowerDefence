#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

UCLASS()
class TOWERDEFENCE_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

protected:
    UFUNCTION()
    void OnMainMenuClicked();

    UPROPERTY(meta = (BindWidget))
    class UButton* MainMenuButton = nullptr;
};
