# Unreal Engine 4 Infostorm
This page is going to be filled with as much UE4/development information, links, and resources as I can find time to wrangle together. _This folder_, however, contains a whole lot of [Unreal Engine 4](https://unrealengine.com) assets that you can use as well. For more information on the assets, check out [**this page**](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/ASSET_LIBRARY.md) as it has the whole skinny on the asset library.

The rest of this will be general-purpose art, development, programming, and so on work with Unreal Engine 4. 

## Index
* [**Conventions**](#conventions)
* [**C++ Development**](#c-development)
  * [**Object/Component/Actor Initialization Process**](#objectcomponentactor-initialization-process)
* [**Documentation**](#documentation)
* [**Gameplay**](#gameplay)
* [**Collision**](#collision)
* [**Rendering/Shading**](#renderingshading)
* [**Scripting**](#scripting)
* [**Source Control**](#source-control)
* [**Tools**](#tools)

## Conventions
Establishing a set, hard set of conventions for development across all departments/disciplines is absolutely essential -- for projects of any size, not just larger teams. I've seen so much time wasted as a result of poor organization and fast-and-loose conventions that I can't recommend strongly enough how important it is to think through your team's development practices early. And then, once established, make changes along the way as necessary (with the caveat that doing so generally entails a non-trivial amount of work).

But, in general, a strong and adhered-to set of conventions can be hugely beneficial to limit confusion, bugs, and general human error.

While not every one of the conventions I established for Joy Machine are useful universally, I feel fairly good about the ones that we have in place; I [**tossed up a document showing our standard practices across development, content, and design**](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/CONVENTIONS.md).

## Asset Management

## Development

### C++ Development
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
