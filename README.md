# ModularGameplayFramework

The modular gameplay framework is an Unreal 5 C++ plugin that simplifies creating a fully replicated data-driven Gameplay Ability System (GAS) based game rapidly. Classes can be overriden in C++ or Blueprints but often will expect Data Tables or Data Assets to set them up.

## Overview

The plugin utilizes the Gameplay Ability System (GAS) and is heavily influenced by the Epic Games Lyra example such as data driven ability set creation and an attribute component that is responsible for handling vital attributes like Health and Death of an actor. 

It provides core base character classes and player state set up with an ability system component and an attribute component, and subclasses for players and NPCs. Characters are granted attribute sets, gameplay effects, and gameplay abilities set up with AbilitySets defined as DataAssets. 

For player stats or attributes (strength, intelligence, armor, etc), define a subclass of DaAttributeSet, define each attribute using the parent GameplayTag: Attributes.Stat, and setup event handlers in blueprints. Attributes.Stat.Primary are meant to be set directly while those with Attributes.Stat.Secondary are derived from Primary (through a gameplay effect).

The UI is based on an event driven MVC design pattern with AttributeSet subclasses (model), WidgetControllers listening and broadcasting changes to UserWidgets (controller->view), all driven via GameplayTags. 

The game using this plugin is responsible for creating its own DaAttributeSet subclass for things like Stats and DaAttributeInfo DataAsset to define things associated (like its widgets), defines images and text for each widget type (like for buttons: borders, button background images and text) and connects attributes to widgets via its gameplay tag.  

Some Gameplay effects are required by default (and are available in the plugin's content folder) as they are responsible for important things like handling death or setting an attribute set's default values. 

There is a GAS based projectile system, an item pickup system to create abilities on the fly, a data driven enhanced input system that can either be set up using GAS abilities or directly in a player character or controller.

Replicated Actors can be continuously spawned on the server via game mode by setting a Spawner subclass in its blueprint which will spawn actors defined via data asset. Currently there is an Item spawner and an AI Character spawner which can be activated via game mode.

The AICharacters predefined in the engine can run around, find a TargetActor using pawn sensing and try to attack it (via gameplay ability), and run away and heal (via gameplay ability) if damaged. 

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

