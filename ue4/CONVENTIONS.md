# Unreal Engine 4 - Joy Machine Style Guide

## Overview
We have all of our source assets separated in file directory *as well as* repositories. Since art/content folks will be modifying source-side content (such as TGAs/PSDs/FBXs), we don't want to require them to download the entire game project as well.

For *Steel Hunters*, for instance, we have: 
* `steelhunters` - The entire UE project folder, with file assets consisting solely of UASSETS/UMAPs.
* `steelhunters-content` - All of our source assets that we use to import into UE as UASSETS.
* `joyengine` - Our fork of Unreal Engine 4, which has an extensive amount of changes and additions from vanilla UE4. As such, you cannot use any UE4 binaries downloaded from the Epic Game Launcher.

### Version Control
* Most day-to-day, non-feature-sized work is done in `develop`.
* For major features, work within a feature branch (`feature/my-branch-name`). There are also branch specifiers for:
	- Bug-Fix (non-trivial fix to a non-release build): `bugfix/`.
	- Hot-fix (non-trivial fix to a release build): `hotfix/`.
	- Release (I honestly don't know why this exists): `release/`.
	- Support (Also: no idea): `support/`.
* We will generally make tags of each repository's state whenever it's in a generally stable state. 
	- This is Trent's responsibility.
* We will push to master when a tagged repository is confirmed to be in a stable state.
	- Also Trent's responsibility.
 
* **IMPORTANT NOTE**: If at any point you're in a weird local state, don't feel comfortable merging something into `develop`, or any other questions: ask. Ask. ASK. ask ask ask. 

### Code-Side Conventions.

#### Game Code Conventions.
* Follow standard UE4 practices: 
	- (Most) headers go into the `Public` directory.
	- All source (C/CPP) files go into the `Private` directory.
* Any third-party code-only libraries go into a `ThirdParty` folder (in `Public` and `Private`).
* All C/C++ files must first include `steelhunters.h`. You'll know if they don't because the project won't build.
* With the rare (or particularly small) exception, stick to one class declaration/definition per file set. 
* I can't recall if this is enforced by the UE4 build tool, but if you're making an actor or component (`AWhatever`/`UWhateverComponent`), the initial letter designation does not get used in the filename. Instead, append the written-out form to the end of the filename (`WhateverActor`/`WhateverComponent`). 
	- Actors do not require `Actor` in the name of the class (they are prefixed with `A`).
	- Interfaces do not require `Interface` in the name of the class (they are prefixed with `I`).
	- Components _do_ require `Component` in the name of the class (they are prefixed with `U` as well).
	- General structures/classes that don't fit into the aforementioned types are prefixed with `F`.
	- Enumerations are prefixed with `E`.
		* The contents of an enumeration are, if possible, prefixed with the acronym for the name of the enumeration (minus the `E` prefix).
  		* All enumerations end with a `_MAX` entry.
* All class members and function arguments are camel case.
 	- If the argument can potentially be a conflict with a class member, append `In` to it.
 	- If an argument is intended to be filled by a method to use outside of it, append `Out` to it.
* All UE-related (i.e. not _purely_ internal code) classes/structures/enumerations/methods/properties are preceded by `UCLASS`/`USTRUCT`/`UENUM`/`UFUNCTION`/`UPROPERTY`.
 	- Additionally, all classes (and most structures) first in-declaration line is `GENERATED_BODY`.
 	- Apply the appropriate properties and metadata designations for each of these.
* Function/method local variables are lower camel case.
* Use `uint32` with a bitmask specifier (i.e. `uint32 MyFlag : 1`) in most cases. 
 	- Exception to this rule is is a boolean value is required for an `EditCondition` property, in which case use `bool`. 
 	- For the love of all that is holy, do not prefix boolean values of any kind with `b`. 
* For `UPROPERTY` members with the `Transient` property, put them in their appropriate category, but under a nested `Runtime`:
 	- For members that are in the `Core` group, a transient member would be in `Category = "Core|Runtime"`.
* Variables rarely should require a comment to explain what they are.
	- This doesn't mean you have to be verbose for the sake of explicitness, of course, just organize a class/function and then read the contained data and see if everything makes sense given the context.
* _Don't_ add comments for the sake of adding comments.
	- If a variable is named `IgnoreIncomingDamage`, don't comment it with `// When enabled, ignore all incoming damage.`.
* _Do_ add comments to demarcate sections of code, especially in class/structure declarations. 
* Typos in variable names happen (but fix them); consistent misspellings do not.
* Use constants for any known strings. Do not just type them in the middle of a function.
* When dealing with In/Out arguments or Start/End/Current/Previous variable names, always append them to the _end_ of the variable (i.e. `VelocityIn` and `VelocityOut` or `SpeedStart` and `SpeedCurrent`.
	- If you prepend `Start` to a set of variables intended to initialize values, typing `Start` will yield a variety of unrelated variable types as opposed to typing `Speed` and getting its various states. Intelli-gent intelli-sense! MAGICK.
* If you use a singleton, you need to have a five-paragraph essay prepared as to why you're using a singleton.
* **THIS IS VERY IMPORTANT**: Don't shorten parts of variable/function names just to save four-five letters. I will hunt you down, and my vengeance will be the things urban legends are born from.

#### Engine Code Conventions
* Don't touch unless explicitly told to by Trent. He's very particular of his baby.

### File Formats and Conventions
* Meshes: FBX (2016).
 	- We do not rely on the socket manager in UE4 for creating/managing sockets (unless it's purely temporary for testing purposes).
		* As such, all sockets for a mesh/rig _must_ exist within the source file. Sockets are "locators" (in Maya) and "something else" (in Max). 
  		* In order for UE4 to properly import the sockers within an FBX, every socket must be prefixed with: `SOCKET_`.
	- Skeletal Meshes/Animations:
		* When exporting out FBXs for animations, you do not need to include the mesh in the export. The only necessary thing to export is the rig and animation keyframes.
		* The rig structure _must_ be consistent through all of its related animations.
			- This includes everything from bones to sockets. So, before getting too far into everything, make sure that you check with the team that a rig has all of the necessary components. We won't necessairly be able to identify all future needs, but we should be able to get close.
* Textures:
	- No alpha channel nor any future need for an alpha channel: PNG.
	- Alpha channel: TGA.

### File Naming Conventions
All **content** file names in both the source directory and the game `Content/` directroy are exclusively lower-case. The one notable exception to this is the identifier for megascan texture sets (ie, the "random" characters in `mesh_foliage_qmBrl_ms`). Beyond that, all files are also prefixed with their type:
* Texture Prefix: `t_`.
	- For our standard channel-packed texture set, the suffixes are:
		* Albedo/Displacement: `_a_d`.
		* Albedo/Mask: `a_mask`.
		* Metallic/Roughness/Cavity/AO: `_m_r_c_ao`.
		* Normal: `_n`.
		* Subsuface Scattering (mostly only for foliage): `_transparency`.
* Static Mesh Prefix: `sm_`.
* Skeletal Mesh Prefix: `skm_`.
   - [**Project-Side**] Base Rig Prefix: `rig_`.
   - [**Project-Side**] Animation Sequence Prefix: `anim_`.
   - [**Project-Side**] Animation Montage Prefix: `anim_montage_`.
   - [**Project-Side**] Animation Blueprint Prefix: `animbp_`.
* Blueprints: a bit more complicated to name:
	- If a blueprint is a first-tier derivation of a C++ class, it's one of the _only_ cases camel case is used (since it's basically a class). Examples: `WorldSimulationActor` or `MechWeaponBaseComponent`.
	- If a blueprint is a data-only blueprint (which are essentially the equivalent to a Unity prefab), there is no mandatory prefix; just name them somewhat intelligent to identify what their parent class/blueprint is named. Examples (to follow the previous ones): `world_simulation_desert` or `mech_weapon_component_barrel`.
* Materials (Base): `m_`
* Materials (Instances): `mi_`
* Material Parameter Collections: `mpc_`
* Material Functions: `mf_`

#### Optional Conventions.
- Filenames that are fairly close to containing their place in the directory structure are occasionally useful. Such as `t_ui_hud_ammo_bg.tga` being within `Content/textures/ui/hud/`. This is not a *requirement* of a file name by any means, but it does occasionally make searching by text easier.

### File Organization
With the exception of miscellaneous packages that have been acquired over time (and have yet to be separated into our component folders), we segregate assets based on what kind of asset they are. We (almost) never have a single directory that contains an asset's mesh, textures, materials, and so on. They're all split up into higher-level folders, such as:
* `audio` - All audio cues, attenuations, concurrencies, etc.
* `data` - Generally, this is just game data, child actor component data definitions, and so on. So, while technically a blueprint, they're more like Unity's "prefabs". This will slowly become deprecated as we move to a more data-driven solution.
* `effects` - All VFX. This includes the 
* `env` - Environment art. While this folder exists, it will eventually be deprecated in favor of splitting things across disparate folders.
* `game` - Any blueprints representing a first-tier derivation of a C++ class.
* `game_data` - Data-only blueprints.
* `GraniteSDK` - This is a special folder created by Granite for housing any texture tilesets (virtualized textures/megatextures/whatever you want to call them) upon import.
* `heightmaps` - Any heightmaps for environments in the game. This will change once we move over to VoxelFarm for landscapes.
* `lut` - A collection of scene color grading look-up tables.
* `maps` - All map files for the game.
* `materials` - One-stop shopping for materials: base material definitions/graphs, material instances, and material functions.
* `mech` - This is a unique folder, but it's basically where all mech parts and animations go.
* `meshes` - Eventually where all meshes will end up (and separated out by their type/environment/etc.).
* `textures` - All the textures. All of them.
 * With a few exception, all textures follow our standard format as [defined in our megascans assembler](https://github.com/joymachinegames/joymachine-public/tree/master/tools/megascan-assembler). This applies to all textures, not just megascans.
* `ui` - All UI content: textures and fonts. 
* `VoxelProjects` - VoxelFarm-exported datasets.
* `water` - Water-specific materials, material instances, meshes, and textures.
