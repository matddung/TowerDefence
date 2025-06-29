#include "FloatingSpawnActor.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AFloatingSpawnActor::AFloatingSpawnActor()
{
    PrimaryActorTick.bCanEverTick = true;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawSize(FVector2D(150, 50));
    RootComponent = WidgetComponent;

    static ConstructorHelpers::FClassFinder<UUserWidget> DamageTextWidgetClass(TEXT("/Game/UI/WBP_DamageText.WBP_DamageText_C"));
    if (DamageTextWidgetClass.Succeeded())
    {
        WidgetComponent->SetWidgetClass(DamageTextWidgetClass.Class);
    }
}

void AFloatingSpawnActor::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(LifeTime);
    ElapsedTime = 0.0f;
}

void AFloatingSpawnActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();
    NewLocation.Z += RiseSpeed * DeltaTime;
    SetActorLocation(NewLocation);

    ElapsedTime += DeltaTime;
    if (ElapsedTime >= LifeTime)
    {
        Destroy();
    }
}

void AFloatingSpawnActor::SetDamage(float Damage)
{
    if (UUserWidget* Widget = WidgetComponent->GetWidget())
    {
        if (UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("DamageText"))))
        {
            Text->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
        }
    }
}