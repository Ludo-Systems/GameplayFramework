// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DaSaveGameSettings.generated.h"

class UDataTable;
/** UDaSaveGameSettings
 *
 *  Add settings to Game preferences
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Save Game Settings"))
class GAMEPLAYFRAMEWORK_API UDaSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/* Default slot name if UI doesn't specify any */ 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	FString SaveSlotName;

	/* Soft path must be converted to asset before use */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> DummyTablePath;

	UDaSaveGameSettings();
	
};
