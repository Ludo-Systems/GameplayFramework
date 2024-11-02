# ModularGameplayFramework

The modular gameplay framework is an Unreal 5 C++ plugin that simplifies creating a fully replicated data-driven Gameplay Ability System (GAS) based game rapidly. Classes can be overriden in C++ or Blueprints but often will expect Data Tables or Data Assets to set them up.

## Features

The plugin utilizes the Gameplay Ability System (GAS) and is heavily influenced by the Epic Games Lyra example such as data driven ability set creation and an attribute component that is responsible for handling all the various attributes such as Health, Mana, etc.

It provides core base player character classes set up with an ability system component and an attribute component. Game mode and Game state are also set up for use with GAS as well as a modular UI system that responds to a given controller connected to the GAS system. 

There is a GAS based projectile system, an item pickup system to create abilities on the fly, a data driven enhanced input system that can either be set up using GAS abilities or directly in a player character or controller.

There are a few assets in the content folder that can be used for things like AI or UI, but they are not required and can be replaced with custom assets.

## Setup

1. Create an Unreal C++ project and either clone the plugin to Project Folder/Plugins/GameplayFramework as a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) or download the zip and place in an Unreal Plugin folder.
2. Enable the Plugin in the project editor Plugins, or in C++, add "GameplayFramework" to the list of required public submodules in your project's Build.cs file. 
3. Create subclasses in either C++ or Blueprints for the core classes: DaGameStateBase, DaGameModeBase, DaCharacter, DaPlayerState, DaPlayerController, DaHud.
4. Set your Game mode subclass up to use those and set it as the map's game mode or default game mode in settings.
5. Create a DaAbilitySet Data Asset where you can assign GameplayAbilities to Input Tags, setup any non-default Attribute Sets (DaPlayerState and DaAICharacter both creates a default Character set for Health and Mana and a combat set to handle damage and healing), GameplayEffects. Create a DaPlayerPawn Data Asset and set the AbilitySet you previously created and then make a DataTable of PlayerInfoRow and set the PlayerPawnDataAsset here.
6. Set the DataTable on the PlayerState as well as a GameplayEffect to reset Health. There is one pre-made in the plugin's Content folder that can be used for this.
7. In Project settings->Input Set the Default Input Component Class to DaInputComponent.
8. Create your EnhancedInput InputActions and Mapping Contexts then create a DaInputConfig DataAsset and map InputTags to InputComponents as needed. Provide this file to your DaPlayerController subclass to dynamcially connect Input to GameplayAbilities previously defined in the character ability set.
9. HUD: Create an OverlayWidgetController and Overlay UserWidget class and set them on the HUD's properties. 
10. In project Settings->Game->Asset Manager: Create 3 new entries in the Primary Data Types to Scan array:
    1. PrimaryAssetType: MinionPawnData, AssetBaseClass: DaMinionPawnData, directory where these data assets can be found. Set rules to always cook. (If not using DaAICharacter for AI this can be skipped).
    2. PrimaryAssetType: AbilitySetData, AssetBaseClass: DaAbilitySet, directory where these data assets can be found. Set rules to always cook.
    3. PrimaryAssetType: PlayerPawnData, AssetBaseClass: DaPlayerPawnData, directory where these data assets can be found. Set rules to always cook.
11. Developer tab can change SaveGame name.

## Configuration

There are lots of ways to configure the project. UI, AI, Camera modes, various gameplay abilities, effects, etc. 
TODO: More on this...

## Debug 

### CVARS
Using da. in UE's editor console will show lots of useful CVars available for debugging.
TODO: More on this....

