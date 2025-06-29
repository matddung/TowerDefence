#include "EnemyHPBarWidget.h"

#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"

void UEnemyHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!HPBar)
    {
        HPBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("HPBar"));
        WidgetTree->RootWidget = HPBar;
    }

    if (HPBar)
    {
        HPBar->SetPercent(1.0f);
    }
}

void UEnemyHPBarWidget::SetHPPercent(float Percent)
{
    if (HPBar)
    {
        HPBar->SetPercent(Percent);
    }
}