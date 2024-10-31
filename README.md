# ModularGameplayFramework

The modular gameplay framework is an Unreal 5 plugin that utilizes various features of Unreal to simplify creating a full featured game rapidly. Most everything is written as optimally as possible in C++, but all classes expect blueprint subclasses or properties for setting content.

It utilizes the the Gameplay Ability System (GAS) and is heavily influenced by various features from Epic Games Lyra example such as data driven ability set creation and an attribute component that is responsible for handling all the various attributes such as Health, Mana, etc.

It includes various prebuilt character controllers such as a third person character controller, a top down character controller, a modular UI system that responds to a given controller connected to the GAS system, a projectile system, an item pickup system to create abilities on the fly, an data driven enhanced input system that can either be set up using GAS abilities or directly in a player character or controller.

There are a few assets in the content folder that can be used for things like AI or UI, but they are not required and can be replaced with custom assets.
