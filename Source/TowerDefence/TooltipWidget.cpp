#include "TooltipWidget.h"

#include "Components/TextBlock.h"

void UTooltipWidget::SetTooltipText(const FText& InText)
{
    if (TooltipTextBlock)
    {
        TooltipTextBlock->SetText(InText);
    }
}