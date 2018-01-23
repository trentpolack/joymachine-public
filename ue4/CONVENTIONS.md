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
* Meshes: FBX (2018).
 	- We do not rely on the socket manager in UE4 for creating/managing sockets (unless it's purely temporary for testing purposes).
		* As such, all sockets for a mesh/rig _must_ exist within the source file. Sockets are "locators" (in Maya) and "something else" (in Max). 
  		* In order for UE4 to properly import the sockers within an FBX, every socket must be prefixed with: `SOCKET_`.
	- Skeletal Meshes/Animations:
		* When exporting out FBXs for animations, you do not need to include the mesh in the export. The only necessary thing to export is the rig and animation keyframes.
		* The rig structure _must_ be consistent through all of its related animations.
			- This includes everything from bones to sockets. So, before getting too far into everything, make sure that you check with the team that a rig has all of the necessary components. We won't necessairly be able to identify all future needs, but we should be able to get close.
* Textures:
	- In almost all cases, PNG is the best choice.
	- If you need an alpha channel in a texture, the easiest format for reading back into Photoshop is a TGA, as it preserves the alpha channel as-is (rather than how PNG handles alpha channel values by interleaving amongst the RGB channels). But, a PNG's alpha channel is still accessible in Photoshop by Ctrl+Left Clicking the layer image in the layers tab.
	- You should _almost never_ intend for an alpha channel to be a discrete channel in a UE4-imported texture; an alpha channel will roughly double the size of standard compression models.

### File Naming Conventions
All **content** file names in both the source directory and the game `Content/` directroy are exclusively lower-case. The one notable exception to this is the identifier for megascan texture sets (ie, the "random" characters in `mesh_foliage_qmBrl_ms`). Beyond that, all files are also prefixed with their type:
* Texture Prefix: `t_`.
	- For our standard channel-packed texture set, the suffixes are:
		* Albedo: `_a`.
		* Normal: `_n`.
		* Mask (single-channel): `_mask`.
		* Masks (multi-channel RGB): `_masks`.
		* Metallic/Roughness/AO (Surfaces): `_m_r_ao`.
		* Mask/Roughness/AO (Foliage): `_mask_r_ao`.
		* Cavity: `_c`.
		* Displacement (rarely-used): `_d`.
		* Subsuface Scattering (mostly only for foliage): `_transparency`.
* Static Mesh Prefix: `sm_`.
* Skeletal Mesh Prefix: `skm_`.
   - Base Rig Prefix: `rig_`.
   - Animation Sequence Prefix: `anim_`.
   - Animation Montage Prefix: `anim_montage_`.
   - Animation Blueprint Prefix: `animbp_`.
* Blueprints: a bit more complicated to name:
	- Unless a blueprint is a blutility or a prefab for a set of meshes into a single file, blueprints are never used.
	- Blueprints: `bp_`.
	- Blutities: `blutility_`.
* Materials (Base/Shaders): `m_shader`.
* Materials (Instances): `mi_`.
* Material Functions: `mf_`.
* Material Parameter Collections: `mpc_`.
* Particle Systems: `ps_`.
* trueSKY Sequences: `seq_`.
* Curves: `curve_`.
* Damage Types: `dt_`.
* Physical Material Data Object: `PhysicalMaterialData_`.
* Non-Physical Material Data Object: `NonPhysicalMaterialData_`.
* Foliage Type: `ft_`.
* Procedural Foliage Set: `pfs_`.
* Sounds:
	- Cues: `cue_`.
	- Waves: `wav_`.
	- Attenuation: `attenuation_`.
	- Mix: `mix_`.
* Maps: No prefix.
	* Map sub-levels: Prefix with the name of the base map.

#### Optional Conventions.
- Filenames that are fairly close to containing their place in the directory structure are occasionally useful. Such as `t_ui_hud_ammo_bg.tga` being within `Content/textures/ui/hud/`. This is not a *requirement* of a file name by any means, but it does occasionally make searching by text easier.

### File Organization (UE4 Content).
With the exception of miscellaneous packages that have been acquired over time (and have yet to be separated into our component folders), we segregate assets based on what kind of asset they are. We (almost) never have a single directory that contains an asset's mesh, textures, materials, and so on. They're all split up into higher-level folders, such as:
* `audio` - All audio cues, attenuations, concurrencies, etc.
* `effects` - All VFX-related data (with the exception of shaders).
	* The only exception to the standard rule; all effects-related textures, material instances, particle systems, SubUV assets, and meshes all reside within this folder.
	* Specific effects libraries have their own subfolder.
	* Common assets are all in `effects/common`.
* `editor_data`: Editor data (blutilities).
* `game_data` - Data-only blueprints.
* `lighting` - IES profiles.
* `maps` - All map files for the game.
* `material_shaders`: Base materials (shaders) only.
* `material_functions`: One-stop shopping for materials: base material definitions/graphs, material instances, and material functions.
* `materials` - Material instances.
* `mech` - This is a unique folder, but it's basically where all mech parts and animations go.
* `meshes` - Any mesh asset (effects meshes being the exception).
* `Splash` - Splash screen images.
* `textures` - Textures (effects textures being the exception).
	* `textures` - All textures arranged in folders for their theme/material type.
	* `textures/atlases` - Any atlased texture (multiple images in a single file). Similarly arranged into their theme/material type.
