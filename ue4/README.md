# Unreal Engine 4 Infostorm
This page is going to be filled with as much UE4/development information, links, and resources as I can find time to wrangle together. _This folder_, however, contains a whole lot of [Unreal Engine 4](https://unrealengine.com) assets that you can use as well. For more information on the assets, check out [**this page**](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/ASSET_LIBRARY.md) as it has the whole skinny on the asset library.

The rest of this will be general-purpose art, development, programming, and so on work with Unreal Engine 4. 

## Index
* [**Conventions**](#conventions)
* [**Asset Management**](#asset-management)
* [**Content Creation**](#content-creation)
  * [**Meshes**](#meshes)
  * [**Materials**](#materials)
	* [**Using Materials**](#using-materials)
	* [**Shaders**](#shaders)
	* [**Particle Shaders**](#particle-shaders)
	* [**Material Functions**](#material-functions)
* [**C++ Development**](#c-development)
  * [**Object/Component/Actor Initialization Process**](#objectcomponentactor-initialization-process)
  * [**Miscellaneous Code Snippets**](#miscellaneous-code-snippets)
* [**Documentation**](#documentation)
* [**Gameplay**](#gameplay)
* [**Collision**](#collision)
* [**Rendering/Shading**](#renderingshading)
  * [**Alternate Shading Models**](#alternate-shading-models)
	* [**Microfacet BRDF Terms**](#microfacet-brdf-terms)
	  * [**Resources**](#resources)
  * [**Debugging**](#debugging)
	* [**RenderDoc**](#renderdoc)
* [**Scripting**](#scripting)
* [**Source Control**](#source-control)
* [**Tools**](#tools)
	* [**Joy Machine**](#joy-machine)
	* [**Third-Party**](#third-party)

## Conventions
Establishing a set, hard set of conventions for development across all departments/disciplines is absolutely essential — for projects of any size, not just larger teams. I've seen so much time wasted as a result of poor organization and fast-and-loose conventions that I can't recommend strongly enough how important it is to think through your team's development practices early. And then, once established, make changes along the way as necessary (with the caveat that doing so generally entails a non-trivial amount of work).

But, in general, a strong and adhered-to set of conventions can be hugely beneficial to limit confusion, bugs, and general human error.

While not every one of the conventions I established for Joy Machine are useful universally, I feel fairly good about the ones that we have in place; I [**tossed up a document showing our standard practices across development, content, and design**](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/CONVENTIONS.md).

## Asset Management

## Content Creation

### Meshes
Topics TODO: Adjacency index generation, lightmap UV generation.

### Materials
I tend to call master materials "shaders" (which is how I'll refer to them from now on unless I specifically talk about USF shaders) as, at least for me, they tend to be "uber-shaders" done with the material editor. Which I do primarily to differentiate between special-case materials and my general-purpose shaders.

#### Using Materials
And I can't recommend strongly enough how Materials — as opposed to Material Instances — should be used as sparingly as possible. The more spread-out your materials are, the more involved a process it becomes to make changes after you realize something is wrong or additional features are needed. Not to mention that once a material is used for a particle or mesh, whenever you delete a material and attempt to replace its references, you cannot replace it with a material instance; they can only be replaced by other materials. And this... is a real annoying and time-consuming issue to remedy.

If you absolutely must use materials over material instances, then your next best bet is to consolidate as much common logic into a [material function](#material-functions) as you possibly can. Do anything you can to consolidate logic whenever possible.

#### Shaders
The shaders I create (which you can see in the [material shader library in this repo](https://github.com/joymachinegames/joymachine-public/tree/master/ue4/material_shaders)) contain a whole lot of logic, customizable features, and complexity. Here's an example of an early version of my standard shader:
![](https://joy-machine.com/wp-content/gallery/steel-hunters-dev-shots/2016_11_03_really_whats_so_complex.PNG "The Totally-Comprehendable Node Graph for my Standard Shader")
While it's can easily become a gargantuan node graph, the resulting material instances created from it are _not_ high-complexity shaders (always remember you can view the resulting HLSL shader code in UE). Here's a comparison between a simple material instance:
![](https://joy-machine.com/wp-content/gallery/steel-hunters-dev-shots/dev_shot_example_random_material_instance.PNG "A Random Material Instance")
And a material instance created from my standard shader:
 ![](https://joy-machine.com/wp-content/gallery/steel-hunters-dev-shots/dev_shot_example_shader_material_instance.PNG "A material instance of my standard shader")
I have all possible instance properties displayed (irrelevant ones are hidden by default) for both instance editors; the standard shader screen shot wasn't able to fit in the remaining texture customization properties as well as detail texturing properties. The only difference in the resulting complexity was a single instruction count increase on the random example material instance, despite the enormous difference in configuration possibilities.

Which brings me to the material parameter node that is the most important thing in the entire world: the [Static Switch Parameter](https://docs.unrealengine.com/latest/INT/Engine/Rendering/Materials/ExpressionReference/Parameters/#staticswitchparameter). This parameter node exposes checkboxes to the material editor that completely discard any logic leading up to the chosen flag state (true/false). This means that, despite the extensive amount of _possible_ functionality in a shader, that possible functionality is completely irrelevant to the generated material instance's generated HLSL code. So, basically, I have every material in my game derived from a core shader library (most of which is in this repo, save for some more proprietary ones); any changes/fixes/optimizations that I realize need to be made can be made solely on the parent shader and it will propagate to any instance derived from it — saving me an extraordinary amount of hassle. And if I need more features on a given instance, I just check/uncheck the relevant switch property, and a new tree of parameters become available and the material instance's complexity is increased/decreased accordingly.

#### Particle Shaders
Particle materials can be a bit more of a pain in the ass, given how specific they can be to a given effect. I'm still in the process of refining how I handle these, but so far I've found, still, a set of shaders for particle materials — a different shader per lighting model, basically — in tandem with a variety of material functions the most useful workflow while still consolidating as much logic as possible.

The only really drastic change between particle material instances and standard surface material instances is that particle material instances rely far more heavily on overrides for blending, lighting type, and two-sidedness. Since these can all be handled on the instance level, however, there's no reason to create separate master materials for variations on all of these. Just ensure that your basic shaders connect to as many output material pins as possible, as some will be grayed out in your master material but still used if an instance makes any overrides.

#### Material Functions
Even with my hard adherence to consolidating as much shading/texturing logic as I possibly can into shaders, that's _still_ not enough to really consolidate as much as I'd really like to be doing. Material functions are the material editor's form of logic encapsulation: individual functions with inputs and outputs  that can be used and reused to your heart's content across any number of materials.
I maintain a [constantly-updating library of material functions](https://github.com/joymachinegames/joymachine-public/tree/master/ue4/material_functions) as I work that I've found invaluable whenever I work. As an example of some of my most-used materials:
* [`mf_normal_lerp`](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/material_functions/mf_normal_lerp.uasset) — A function to properly blend together two normal maps that maintains the proper normal data.
* [`mf_metallic_shading`](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/material_functions/mf_metallic_shading.uasset) — A function to exaggerate the metallic shading on a material surface; it bases the material's roughness based on the vector between the camera and the pixel being evaluated. 
	* Aspects of this functionality is now integrated into the material editor (as of UE 4.17 or 4.18) with the curvature-based roughness parameter in the material properties (which [I also have a material function](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/material_functions/mf_angle_based_roughness.uasset) for, as it's more useful than making it a property of a master material).
* [`mf_luminance`](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/material_functions/mf_luminance.uasset) — Properly evaluate the luminance value of the given pixel (best used in postprocessor materials).
* [`mf_component_max_v3`](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/material_functions/mf_component_max_v3.uasset) — A stupid simple function that just grabs the maximum value from a three-component vector. It sounds dumb. It's amazing.
And as far as feature availability goes, material functions have all of the same node graph functionality as any material. You can even create and expose parameters to the material instance editor — though, without exception, I recommend not creating customizable parameters within material functions (just make them inputs, then create the parameters in the material using the material function). This is mostly a convention, but it's one that has helped prevent a whole lot of confusion when looking back on materials I haven't touched in a while.


## Development

### C++ Development

### Object/Component/Actor Initialization Process
There's backstory for this one.

For the most part, the creation/spawning of objects, components, actors, etc. (henceforth called "entities") is actually pretty manageable and sometimes aren't even a real concern. There are certain situations where you have a set of dependencies that is particularly rough to ensure are setup properly, but that shouldn't be the case terribly often. If it is, reevaluate your approach.

That said, there is an aspect to this entire process that is a living nightmare from which there is not only no escape, but in this nightmare you also have nightmares. And that aspect is: **actors that must exist in a fully-initialized and functional form in the editor outside of gameplay**. With maybe one or two exceptions, everything in our workflow, game architecture, and general practices is completely dynamic. And we like to have as much of our worlds active and functioning like normal while we're in the editor. This is partially because it's just a fantastic way to sanity check the health of every system in the game and engine. The other reason is that we're a small team making a game whose events and flow are very difficult to every predict. If we can't build systems that are bulletproof during development, we're going to be constantly revisiting old work to bring it up to snuff throughout production (and, likely, beyond). And that's no bueno. Plus, it's nice to just see everything working in harmony while you're working in the editor. It's just a good feeling.

Eventually, I'd like to be able to say: here's a C++ template for dealing with actors/components that must be routinely setup and torn down due to frequent editor manipulation. I thought I had a pretty good idea of the ins-and-outs of that whole *thing* when I wrapped up our `AMechController` which is the core pawn derivative that defines the logic for every mech in the world. And that meant destroying and reassembling the entire mech whenever that actor was affected by an editor change (at the time, we had an idle mech in the scene, so we had to deal with that often). Then when I started working on our primary world simulation mega-system — which operates at an even higher level than a single mech — I realized that I was so young and naive to think I had *things* down.

Oh, and once I had the world simulation core in and working, I then decided it would be neat to have it run its `::Tick` method while the editor was just running (not in PIE nor in simulation, just while working in the editor).

**ANYWAY**. This led to a lot of reading and experimenting and researching and mostly digging through file after file in the Unreal Engine codebase. That all helped a lot, but the following documents are really (almost) one-stop shopping for everything you need to know (the one with all caps and bolded letters is the main one):
* [Actors Overview](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/index.html)
* [ACTOR LIFECYCLE](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/ActorLifecycle/index.html)
* [Components Overview](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/Components/index.html)
* [Actor Spawning](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/Spawning/index.html)
* [Actor Ticking](https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/Actors/Ticking/index.html)

### Miscellaneous Code Snippets
* [**Houdini Engine Plugin for UE4**](https://gist.github.com/trentpolack/203806b53fe06ee46bc5f24a41467211) — Windows-only improvements I made to the Houdini Engine UE4 plugin for better Houdini path determination.
* [**Set/Get Accessor Creation Macros**](https://gist.github.com/trentpolack/a1bdcd3002d0eda2bf17a46b5c292bf7) — I've been working with a lot of serialization data structures which, generally, are just mirrored data to deserialize into an object instance with similar data. So, I created some macros to generate Set/Get accessors with one line of code (which also eliminates set/get methods from accidentally setting/getting the wrong variable amidst mass copy-and-pasting).
* [**A More Useful UE4 Enumeration Structure**](https://gist.github.com/trentpolack/ad361a4d3d6e46d1a1ff82c03a997f28) — Rather than relying on a straight-up `UENUM` enumeration, it's far, far easier to create a namespace with your enumeration's name and embed the enumeration values in an `enum Type`. It makes life _so much simpler_.
* [**Reminders via Visual Studio Build Warnings**](https://gist.github.com/trentpolack/957daf8f0189e8d0ef5da4e75ac68d04) — Add `JM_TODO( "05/05/18", "HEY REMEMBER TO ADD THE GREAT."` once you toss something like in that gist somewhere, and you'll get nice little TODO messages as non-build-breaking warnings (even for warnings-treated-as-errors).

Read. Read so hard.

## Documentation

## Gameplay

## Collision
I generally say that doing anything *unfamiliar* in C++ in Unreal Engine means you should double your time estimate for the given task. When it involves collision and collision response, maybe quadruple your time estimate.
- The Unreal Engine blog has [one of the best breakdowns of collision filtering](https://www.unrealengine.com/en-US/blog/collision-filtering) around. I don't think it's integrated into the proper documentation, which is a source of endless confusion for me. But it exists! And that's what matters.

## Rendering/Shading

### Alternate Shading Models
Topic TODO: lambertian vs oren-nayar

#### Microfacet BRDF Terms

##### Resources
* [_Microfacet BRDF_](http://simonstechblog.blogspot.com/2011/12/microfacet-brdf.html) — A great article (with a wonderful embedded WebGL demo for term comparisons) covering various BRDF distribution and geometry term models. 
* [_Optimizing GGX Shaders_](http://filmicworlds.com/blog/optimizing-ggx-shaders-with-dotlh) — A solid piece on optimizing the GGX model for calculating the fresnel term.

### Debugging
- When working within the post-stack, `r.CompositionGraphDebug` is your bestest best friend. Just invoke it from the console, the editor will freeze for a bit, and then you'll see debug output in your console of the stack breakdown.
  - But, beyond that, a handful of `GML` files will be output into your `Saved/Screenshots/*` folder. These, when loaded up in a [program that supports them](http://www.yworks.com), can be super, super helpful in debugging and finessing your composition.
 **NOTE**: Make sure that you have the program organize the nodes hierarchically, so you can see the actual order of operations.
- A [concise engine and shader modification](https://github.com/hallatore/UnrealEngine) that handles sharper/less noisy temporal antialiasing.

#### RenderDoc
As of UE 4.17 (or 4.18), the [RenderDoc](https://renderdoc.org) plugin ([originally from Temaran](https://github.com/Temaran/UE4RenderDocPlugin)) is part of the Unreal Engine 4 source plugins, making it easier to get working than setting it up yourself. And it is _wonderful_.

This plugin adds an icon for RenderDoc in the upper-right of the primary editor viewport and clicking it (so long as you've set it up to point to your RenderDoc installation) will do a full frame capture of all API calls and let you see the entire frame's rendering progress. You can look at any given call and see what its input/output render targets are to see how the render progression is working, analyze the input/output of all postprocessor passes, as well as geometric/shader data for any given call.

[Pix](https://blogs.msdn.microsoft.com/pix) is an alternative (potentially complementary) DirectX debugging tool as well, though I haven't found it as useful as RenderDoc. 

## Scripting

## Source Control
Source control is great. And this repo already has [an entire section devoted to it](https://github.com/joymachinegames/joymachine-public/tree/master/git-template). Check it out! We're very git-focused. 

## Tools

### Joy Machine
- [**Joy Create (wip)**](https://github.com/joymachinegames/joymachine-public/tree/feature/joy-create-v0.1/joy-create) — This is a work-in-progress Electron application (eventually a general web app as well) for using [l-systems/turtle instantiation](https://cgjennings.ca/articles/l-systems.html) of shapes/meshes to create exportable OBJ/FBXs out of the results. As noted: *very work-in-progress*.

### Third-Party
- [**Procedural street and intersection plugin for UE4**](https://github.com/ydrive/StreetArch) — Procedural street and intersection building tool and plugin for Unreal Engine 4.
