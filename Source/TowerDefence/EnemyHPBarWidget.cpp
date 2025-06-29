#include "EnemyHPBarWidget.h"

#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"

void UEnemyHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!PB_HPBar)
    {
        PB_HPBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("HPBar"));
        WidgetTree->RootWidget = PB_HPBar;
    }

    if (PB_HPBar)
    {
        PB_HPBar->SetPercent(1.0f);
    }
}

void UEnemyHPBarWidget::SetHPPercent(float Percent)
{
    if (PB_HPBar)
    {
        PB_HPBar->SetPercent(Percent);
    }
}