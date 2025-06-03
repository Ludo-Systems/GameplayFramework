#include "Collectibles.h"

#include "CEInventoryItemFactory.h"
#include "Inventory/DaInventoryItemBase.h"

#define LOCTEXT_NAMESPACE "FCollectiblesModule"

void FCollectiblesModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("Collectibles submodule Loaded."));
	
	UDaInventoryItemBase::Factories.Empty();
	UCEInventoryItemFactory* CustomFactory = NewObject<UCEInventoryItemFactory>();
	UDaInventoryItemBase::Factories.Add(CustomFactory);
}

void FCollectiblesModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCollectiblesModule, Collectibles)