## Generally Helpful Unreal Engine 4 Assets
### Copyright 2015-2017 [Joy Machine](https://joy-machine.com), LLC. All rights reserved.

### Setup
Just toss this folder inside at the root level of your `/Content` directory (you can get rid of this file, though, unless you *really* love it like we do). The references for tehse assets should remain in tact and not require any special treatment so long as the file structure in relation to `/Content/` remains the same. Once they're imported into the project, you can feel free to mvoe them about the cabin.

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
