# Unreal Engine 4 Infostorm
### Copyright 2015-2017 [Joy Machine](https://joy-machine.com), LLC. All rights reserved.

## Assets
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
 
If you have a Directional Wind Component in your scene, that is neat. It generates wind with varying gusts/turbulence that do a very adequate job of affecting SpeedTree assets. And that's about it. However, if you'd like those forces to also affect non-SpeedTree foliage, you can pipe the values from the Directional Wind Component into the material parameter collection that we have setup in `game_data/env`: `mpc_world.uasset`. It's a silly simple little structure, but it has just the data you need to help simulate wind on standard foliage. If you fill it with data every frame and then use the non-SpeedTree master foliage material, the results should be obvious. We use a simple vertical gradient to help give the lower parts of foliage more "weight" so they don't waver as much in the wind, but this still needs some work to look even a little bit realistic. Also note: if you have your foliage weighted through vertex colors, the master foliage shader supports an optional parameter to read a weight value from the red vertex color channel. 

If you have any questions, feel free to email [joy@joy-machine.com](mailto:joy@joy-machine.com).

## Practice, Convention, and Neat Things

### Asset Management

### Development
#### Object/Component/Actor Initialization Process
There's backstory for this one.

For the most part, the creation/spawning of objects, components, actors, etc. (henceforth called "entities") is actually pretty manageable and sometimes aren't even a real concern. There are certain situations where you have a set of dependencies that is particularly rough to ensure are setup properly, but that shouldn't be the case terribly often. If it is, reevaluate your approach.

That said, there is an aspect to this entire process that is a living nightmare from which there is not only no escape, but in this nightmare you also have nightmares. And that aspect is: **actors that must exist in a fully-initialized and functional form in the editor outside of gameplay**. With maybe one or two exceptions, everything in our workflow, game architecture, and general practices is completely dynamic. And we like to have as much of our worlds active and functioning like normal while we're in the editor. This is partially because it's just a fantastic way to sanity check the health of every system in the game and engine. The other reason is that we're a small team making a game whose events and flow are very difficult to every predict. If we can't build systems that are bulletproof during development, we're going to be constantly revisiting old work to bring it up to snuff throughout production (and, likely, beyond). And that's no bueno. Plus, it's nice to just see everything working in harmony while you're working in the editor. It's just a good feeling.

Eventually, I'd like to be able to say: here's a C++ template for dealing with actors/components that must be routinely setup and torn down due to frequent editor manipulation. I thought I had a pretty good idea of the ins-and-outs of that whole *thing* when I wrapped up our `AMechController` which is the core pawn derivative that defines the logic for every mech in the world. And that meant destroying and reassembling the entire mech whenever that actor was affected by an editor change (at the time, we had an idle mech in the scene, so we had to deal with that often). Then when I started working on our primary world simulation mega-system -- which operates at an even higher level than a single mech -- I realized that I was so young and naive to think I had *things* down.

Oh, and once I had the world simulation core in and working, I then decided it would be neat to have it run its `::Tick` method while the editor was just running (not in PIE nor in simulation, just while working in the editor).

**ANYWAY**. This led to a lot of reading and experimenting and researching and mostly digging through file after file in the Unreal Engine codebase. That all helped a lot, but the following documents are really (almost) one-stop shopping for everything you need to know (the one with all caps and bolded letters is the main one):
* [Actors Overview](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/index.html)
* [**ACTOR LIFECYCLE**](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/ActorLifecycle/index.html)
* [Components Overview](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/Components/index.html)
* [Actor Spawning](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/Spawning/index.html)
* [Actor Ticking](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/Ticking/index.html)

Read. Read so hard.

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
