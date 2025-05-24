# Codex Agent Guidelines

This repository contains the **GameplayFramework** plugin for Unreal Engine 5.5.
The plugin provides a data-driven Gameplay Ability System and various utility
classes (inventory, AI, UI widgets, etc.).  The repository does not contain
automated tests.

Refer to `README.md` for detailed setup instructions on integrating the plugin
into your Unreal project.

## Repository Structure

- `GameplayFramework.uplugin` – plugin descriptor enabling required modules.
- `Source/GameplayFramework/`
  - `Public/` – header files. Subdirectories include:
    - `AbilitySystem/` – gameplay abilities, attribute sets, effects,
      modifiers and ability tasks.
    - `AI/` – AI characters, controllers, and behaviour tree helpers.
    - `Inventory/` – inventory components, item base classes, and factories.
    - `UI/` – common UI widgets and widget controller classes.
    - Additional headers for characters, game modes, projectiles and more.
  - `Private/` – implementations mirroring the public layout.
- `Content/` – example assets used by the plugin.
- `Resources/` – plugin icon resources.

## Development Notes

1. Add the plugin to the `Plugins/` folder of an Unreal 5.5 C++ project
   (clone directly or as a git submodule).
2. Run Unreal's `GenerateProjectFiles` script so the new module is detected.
3. Build your project from your IDE or the Unreal Editor. The plugin will be
   compiled along with the rest of the project.

No automated test suite is provided.
