## Generally Helpful Unreal Engine 4 Assets
### Copyright 2015-2017 [Joy Machine](https://joy-machine.com), LLC. All rights reserved.

### Setup
Just toss this folder inside at the root level of your `/Content` directory (you can get rid of this file, though, unless you *really* love it like we do). The references for tehse assets should remain in tact and not require any special treatment so long as the file structure in relation to `/Content/` remains the same. Once they're imported into the project, you can feel free to mvoe them about the cabin.

#### Standard Shader (Material) Library
The standard material library was developed to help us maintain physically-based property consistency throughout our entire scene (which helps *tremendously* when balancing scene luminance values). They're kind of... The master materials are, well, terrifying to look at, but there's a reason for that: since we use these master materials everywhere we don't want to ever have unnecessary instructions. 

In short, these materials are not intended to be "uber shaders" per se, but rather just the most-frequently used aspects of materials that we want whenever customing our scene.

These materials all adhere pretty strictly to both UE4's PBR practices as well as general PBR practices:
 * Metallic-based.
 * Specular is configurable, but should almost always be set to `0.5`.
 * Please for the love of god don't make non-metallic materials have a metallic value of anything but 0.

There are six shaders that this library includes:
 * `m_foliage_master` -- General-purpose foliage (non-speedtree) shader.
 * `m_foliage_speedtree_master` -- Foliage (speedtree) shader.
 * `m_standard` -- The most-frequently used of the bunch; general-purpose shader with all the functionality listed below.
 * `m_standard_metallic` -- A customized `m_standard` shader that also takes advantage of clear-coat parameters and exaggerated metallic shading (NOTE: this does *not* change the metallic map whatsoever, it just alters the shading a bit based on the camera view).
 * `m_standard_tessellated` -- Hardware tessellation version of `m_standard`. This is probably the least-complete of the bunch because hardware tessellation is, quite simply, not practical.
 * `m_standard_triplanar` -- A triplanar version of `m_standard`.
 
##### Using the Standard Shader Library
Ideally, I recommend following our typical texture format convention to save samplers/space. Basically, we just pack as much as we can into three textures:
 * Texture One: Albedo (RGB)/Displacement (A).
  * Texture One (Alternative): Albedo (RGB)/Alpha Mask (A).
 * Texture Two: Metallic (R)/Roughness (G)/Cavity (B)/AO (A).
 * Texture Three: Normal map.
This is by no means a requirement, though, as the materials support individual textures for each of those components, it's just not ideal.

Each material also supports additional functionality (which, due to extreme static switch use, will all increase the instruction count):
 * Height Blended Textures: Specifical a texture -- using the same conventions as the base material -- to help better blend an object with the world.
 * Color manipulation parameters.
 * Other stuff I may be forgetting since I'm running a full engine rebuild right now.
 
If you have any questions, feel free to email [joy@joy-machine.com](mailto:joy@joy-machine.com).

## General Tips & Tricks

### Asset Management

### Coding

### Documentation

### Gameplay

### Rendering/Shading
- When working within the post-stack, `r.CompositionGraphDebug` is your bestest best friend. Just invoke it from the console, the editor will freeze for a bit, and then you'll see debug output in your console of the stack breakdown.
  - But, beyond that, a handful of `GML` files will be output into your `Saved/Screenshots/*` folder. These, when loaded up in a [program that supports them](http://www.yworks.com), look like this:

![Post-Processor Stack Breakdown](https://joy-machine.com/wp-content/uploads/2017/04/ue4_poststack_debug_graph.png "Post-Processor Stack Breakdown")

**NOTE**: Make sure that you have the program organize the nodes hierarchically, so you can see the actual order of operations.

### Scripting

### Source Control

### Tools
