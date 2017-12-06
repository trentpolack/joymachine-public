**NOTE**: This document needs to be updated for all the changes made in the 4.18 asset update. I will hopefully do that soon (trent, 12/5/17).

# Unreal Engine 4 Infostorm
This is a combination of random notes and mini-tutorials as well as any useful assets that we have.

We also have our [**internal team conventions listed in extruciating detail**](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/CONVENTIONS.md) for those that want to read them.

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

#### Procedural Water Material
**Disclaimer**: This is a material and material function set I wrote for a previous project; it hasn't really been used in about a year and a half or so, but it does work! Also, none of the necessary textures for this were included as we don't have the rights to distribute the ones being used.

Aside from diffuse/normal textures and a noise texture -- as noise wasn't available in the material editor "back in the day" -- this is a wholly procedural solution for water and wave generation purely done through the material editor. As such, it's not the most realistic, performant, nor attractive water rendering option in the world. That said, it is pretty customizable and has some neat things going on, so hopefully it's useful for someone.

The gist of the whole system is given a bit of an overview in an article on our site: [https://joy-machine.com/devlog/20151021water-rendering-moby-dick-something-something](https://joy-machine.com/devlog/20151021water-rendering-moby-dick-something-something).

#### Smear Frame
"Smear frame" material effect for (originally created in Unreal Engine 4.12; should be fine still, however). Created by Jan Kaluza of [Broad Strokes](http://www.broad-strokes.com) ([@JKashaar on Twitter](https://twitter.com/JKashaar)). He's a Joy Machine team member, though, so we also put in here.

![Smear Frame effect](http://i.giphy.com/l46CcnrPYQsJ0ejw4.gif "UE4 Smear Frame effect")

##### What is this?
This is an Unreal Engine 4 project containing a simple implementation of a "smear frame" effect, as recently popularized by the game Overwatch.

##### How do I use this?
This effect consists of two parts: a blueprint component you add to an actor you want to be affected by the smear frame effect, and a material function that you add to its material.

##### Limitations
As a quick and dirty proof of concept, this effect uses the actor's velocity to determine smear strength. As such, it does not work with animated skeletal meshes in the way you'd expect. Suggestions or pull requests are very welcome though! :)

## Asset Management

## Development
Most of this section will be updated later, but for now here's a link to a gist about enumerations in UE4 C++ code: [Enumerations in Unreal Engine 4 C++ Code](https://gist.github.com/trentpolack/ad361a4d3d6e46d1a1ff82c03a997f28).

### Object/Component/Actor Initialization Process
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

## Documentation

## Gameplay

## Collision
I generally say that doing anything *unfamiliar* in C++ in Unreal Engine means you should double your time estimate for the given task. When it involves collision and collision response, maybe quadruple your time estimate.
- The Unreal Engine blog has [one of the best breakdowns of collision filtering](https://www.unrealengine.com/en-US/blog/collision-filtering) around. I don't think it's integrated into the proper documentation, which is a source of endless confusion for me. But it exists! And that's what matters.

## Rendering/Shading
- When working within the post-stack, `r.CompositionGraphDebug` is your bestest best friend. Just invoke it from the console, the editor will freeze for a bit, and then you'll see debug output in your console of the stack breakdown.
  - But, beyond that, a handful of `GML` files will be output into your `Saved/Screenshots/*` folder. These, when loaded up in a [program that supports them](http://www.yworks.com), can be super, super helpful in debugging and finessing your composition.
 **NOTE**: Make sure that you have the program organize the nodes hierarchically, so you can see the actual order of operations.
- A [concise engine and shader modification](https://github.com/hallatore/UnrealEngine) that handles sharper/less noisy temporal antiliasing.

## Scripting

## Source Control
Source control is great. And this repo already has [an entire section devoted to it](https://github.com/joymachinegames/joymachine-public/tree/master/git-template). Check it out! We're very git-focused. 

## Tools
