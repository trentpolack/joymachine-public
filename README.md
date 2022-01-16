# [Joy Machine](https://joy-machine.com/) - Repo of *Things*
## Maintenance Status
**NOTE**: This repo is no longer being maintained — hence the most recent UE4 version being 4.17 — but there are still some resources/links in here which may prove useful.

## Overview
Most recent change log (which is poorly-updated until we finally find a good generator): [CHANGELOG.md](https://github.com/joymachinegames/joymachine-public/blob/master/CHANGELOG.md).

* [**`assets`**](https://github.com/joymachinegames/joymachine-public/tree/master/assets) — General-purpose game assets to do with as you please.
    * [**`terrain`**](https://github.com/joymachinegames/joymachine-public/tree/master/assets/terrain) — An older iteration of the landscape that I was using for the Nevada environment (the current one is much better, but I need to keep some things internal for obvious reasons). The main attraction here is the node graph for the entire landscape build... but it requires both World Machine 3 and GeoGlyph 2. So, in the spirit of things, I included the output of a previous build I did with that graph. *However*: the output is not as good as the actual node graph will export (I made did cleanup and made some improvements before putting it in this repo); I'll try and update these files with shiny, new data if I get a spare eight hours to build the thing.
* [**`git`**](https://github.com/joymachinegames/joymachine-public/tree/master/git) — Some git configurations for ignoring files, LFS-managed files, and then detailed config settings for everyday interaction with git on the command line.
* [**`joy-create` (wip)**](https://github.com/joymachinegames/joymachine-public/tree/feature/joy-create-v0.1/joy-create) — This is a work-in-progress Electron application (eventually a general web app as well) for using [l-systems/turtle instantiation](https://cgjennings.ca/articles/l-systems.html) of shapes/meshes to create exportable OBJ/FBXs out of the results. As noted: *very work-in-progress*.
* [**`scripts`**](https://github.com/joymachinegames/joymachine-public/tree/master/scripts) — Miscellaneous scripts (tools, powershell, visual studio extensions, batch files, etc.) 
* [**`ue4-source`**](https://github.com/joymachinegames/joymachine-public/tree/master/ue4-source) — Whatever C++ actors/components/utilities/etc. we end up being able to easily divorce from our projects. Or, if not easily divorced, whatever seems worth divorcing so others never have to know the pain that we did. This will likely be the least frequently-updated part of the repo.
   * **NOTE**: The source is kind of a free-for-all; I infrequently add some of the more generic/widely-useful stuff I can, but I can't include the full source integrations (such as, in the case of the anamorphic lens flares, the updated `Scene.h`|`.cpp` nor `PostProcessing.cpp`) due to a whole lot of custom work done.
   * The anamorphic lens flare code is also a very rough first-draft.
   * The object pooling code is _not_ ready for production use by any means at the moment; it looked like it would be lovely and fine, but it is not. It is a wolf dressed up as a friendly grandmother and it will eat all joy from you.
* [**`ue4`**](https://github.com/joymachinegames/joymachine-public/tree/master/ue4) — Our general-purpose Unreal Engine 4 stockpile. Contains our team production conventions and, more interestingly at the moment, a variety of material functions and our "standard shader set" (they're just materials, but I call them shaders because they're pretty... intense).
   * **NOTE**: These assets are output from Unreal Engine 4.19; there are archived versions of them (without the changes) for 4.17 in the `4.17` subfolder.

### Other Things of Note:
* [**RESEARCH PAPERS**](https://github.com/joymachinegames/joymachine-public/blob/master/RESEARCH_PAPERS.md) - For the nerdier among us (like me), here are some of the better research papers/presentations I've come across over the years. That have survived over time.
* [**RESOURCES**](https://github.com/joymachinegames/joymachine-public/blob/master/RESOURCES.md) — A list of general resources for game development across a number of disciplines. (Work-in-Progress at the moment)
* [**AWESOME_SOFTWARE**](https://github.com/joymachinegames/joymachine-public/blob/master/AWESOME_SOFTWARE.md) — Has a list of all of the software that I (trent) have found to be the best solution within its given context.
* [**AWESOME_GAMEDEV_MENTORS**](https://ishansharma.github.io/twitter-mentors/) — Technically, it's "twitter-mentors", but I like my name better. Anyway, this is [@ishansharma](https://github.com/ishansharma)'s compilation of people ([@trentpolack](https://github.com/trentpolack) included) who offer game development mentoring to anyone that seeks it.
* [**Unreal Engine 4 - General Information**](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/README.md) — A collection of information, links, and whatever else seems helpful to include. It also contains a summary of the UE4 content contained therein.
* [**Unreal Engine 4 - Joy Machine Conventions**](https://github.com/joymachinegames/joymachine-public/blob/master/ue4/CONVENTIONS.md) — Our internal development conventions for working with UE4.

### More to Come!
hugs,
Joy Machine

If you have any questions, feel free to email us: [joy@joy-machine.com](mailto:joy@joy-machine.com) (or [trent@joy-machine.com](mailto:trent@joy-machine.com) if you prefer that).
