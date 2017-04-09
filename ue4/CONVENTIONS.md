## Unreal Engine 4 - Joy Machine Style Guide
### Copyright 2015-2017 [Joy Machine](https://joy-machine.com), LLC. All rights reserved.

### Overview
We have all of our source assets separated in file directory *as well as* repositories. Since art/content folks will be modifying source-side content (such as TGAs/PSDs/FBXs), we don't want to require them to download the entire game project as well.

For *Steel Hunters*, for instance, we have: 
 - `steelhunters` - The entire UE project folder, with file assets consisting solely of UASSETS/UMAPs.
 - `steelhunters-content` - All of our source assets that we use to import into UE as UASSETS.

All **content** file names in both the source directory and the game `Content/` directroy are exclusively lower-case. The one notable exception to this is the identifier for megascan texture sets (ie, the "random" characters in `mesh_foliage_qmBrl_ms`). Beyond that, all files are also prefixed with their type:
* Texture Prefix: `t_`.
* Static Mesh Prefix: `sm_`.
* Skeletal Mesh Prefix: `skm_`.
   - [**Project-Side**] Base Rig Prefix: `rig_`.
   - [**Project-Side**] Animation Sequence Prefix: `anim_`.
   - [**Project-Side**] Animation Montage Prefix: `anim_montage_`.
   - [**Project-Side**] Animation Blueprint Prefix: `animbp_`.
* Blueprints: a bit more complicated to name, general rules TBD.

### Organization
With the exception of miscellaneous packages that have been acquired over time (and have yet to be separated into our component folders), we segregate assets based on what kind of asset they are. We (almost) never have a single directory that contains an asset's mesh, textures, materials, and so on. They're all split up into higher-level folders, such as:
 - `audio` - All audio cues, attenuations, concurrencies, etc.
 - `blueprints` - Any blueprints that define script/functionality. This is a distinction with the "blueprints" that go in `data`.
 - `char` - This is a unique folder, but it's basically where all mech parts and animations go. It will likely be renamed to `mech` at some point.
 - `data` - Generally, this is just game data, child actor component data definitions, and so on. So, while technically a blueprint, they're more like Unity's "prefabs". This will slowly become deprecated as we move to a more data-driven solution.
 - `effects` - All VFX. This includes the 
 - `env` - Environment art. While this folder exists, it will eventually be deprecated in favor of splitting things across disparate folders.
 - `GraniteSDK` - This is a special folder created by Granite for housing any texture tilesets (virtualized textures/megatextures/whatever you want to call them) upon import.
 - `heightmaps` - Any heightmaps for environments in the game. This will change once we move over to VoxelFarm for landscapes.
 - `lut` - A collection of scene color grading look-up tables.
 - `maps` - All map files for the game.
 - `materials` - One-stop shopping for materials: base material definitions/graphs, material instances, and material functions.
 - `meshes` - Eventually where all meshes will end up (and separated out by their type/environment/etc.).
 - `textures` - All the textures. All of them.
   - With a few exception, all textures follow our standard format as [defined in our megascans assembler](https://github.com/joymachinegames/joymachine-public/tree/master/tools/megascan-assembler). This applies to all textures, not just megascans.
 - `ui` - All UI content: textures and fonts. 

### Optional Conventions.
- Filenames that are fairly close to containing their place in the directory structure are occasionally useful. Such as `t_ui_hud_ammo_bg.tga` being within `Content/textures/ui/hud/`. This is not a *require* of a file name by any means, but it does occasionally make searching by text easier.
