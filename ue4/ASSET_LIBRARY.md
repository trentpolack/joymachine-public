# Asset Library
This folder contains a bunch of assets from development during [**_Steel Hunters_**](http://steel-hunters.com) and miscellaneous other projects. The library isn't exactly "super well documented" since I tend to just take working assets from _Steel Hunters_ and toss them in here to keep everything updated with changes I've made... And that isn't very conducive to keeping documentation up-to-date. I try to write as descriptive of changelogs as I can, but that's the best I can promise. If you ever have a question, feel free to [post an issue](https://github.com/joymachinegames/joymachine-public/issues) or [e-mail me](mailto:trent@joy-machine.com) or  just [@mittense me on twitter](https://twitter.com/mittense).

These assets are all entirely free for you to use, so you can take them into your own projects, modify them, as well as distribute them elsewhere (with no requirement to keep everything open source). The only stipulation is that [this copyright/license is included along with everything](https://github.com/joymachinegames/joymachine-public/blob/master/LICENSE).

## Setup
Just toss this folder inside at the root level of your `/Content` directory (you can get rid of this file, though, unless you *really* love it like we do). The references for tehse assets should remain in tact and not require any special treatment so long as the file structure in relation to `/Content/` remains the same. Once they're imported into the project, you can feel free to mvoe them about the cabin.

## Standard Shader (Material) Library
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
	 
### Using the Standard Shader Library
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
	 
If you have a Directional Wind Component in your scene, that is neat. It generates wind with varying gusts/turbulence that do a very adequate job of affecting SpeedTree assets. And that's about it. However, if you'd like those forces to also affect non-SpeedTree foliage, you can pipe the values from the Directional Wind Component into the material parameter collection that we have setup in `game_data/env`: `mpc_world.uasset`. It's a silly simple little structure, but it has just the data you need to help simulate wind on standard foliage. If you fill it with data every frame and then use the non-SpeedTree master foliage material, the results should be obvious. We use a simple vertical gradient to help give the lower parts of foliage more "weight" so they don't waver as much in the wind, but this still needs some work to look even a little bit realistic. Also note: if you have your foliage weighted through vertex colors, the master foliage shader supports an optional parameter to read a weight value from the red vertex color channel. 

## Procedural Water Material
**Disclaimer**: This is a material and material function set I wrote for a previous project; it hasn't really been used in about a year and a half or so, but it does work! Also, none of the necessary textures for this were included as we don't have the rights to distribute the ones being used.

Aside from diffuse/normal textures and a noise texture -- as noise wasn't available in the material editor "back in the day" -- this is a wholly procedural solution for water and wave generation purely done through the material editor. As such, it's not the most realistic, performant, nor attractive water rendering option in the world. That said, it is pretty customizable and has some neat things going on, so hopefully it's useful for someone.

The gist of the whole system is given a bit of an overview in an article on our site: [https://joy-machine.com/devlog/20151021water-rendering-moby-dick-something-something](#)(https://joy-machine.com/devlog/20151021water-rendering-moby-dick-something-something).

## Smear Frame
"Smear frame" material effect for (originally created in Unreal Engine 4.12; should be fine still, however). Created by Jan Kaluza of [Broad Strokes](#)(http://www.broad-strokes.com) ([@JKashaar on Twitter](#)(https://twitter.com/JKashaar)). He's a Joy Machine team member, though, so we also put in here.

![Smear Frame effect](http://i.giphy.com/l46CcnrPYQsJ0ejw4.gif "UE4 Smear Frame effect")

### What is this?
This is an Unreal Engine 4 project containing a simple implementation of a "smear frame" effect, as recently popularized by the game Overwatch.

### How do I use this?
This effect consists of two parts: a blueprint component you add to an actor you want to be affected by the smear frame effect, and a material function that you add to its material.

### Limitations
As a quick and dirty proof of concept, this effect uses the actor's velocity to determine smear strength. As such, it does not work with animated skeletal meshes in the way you'd expect. Suggestions or pull requests are very welcome though! :)
