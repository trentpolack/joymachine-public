# [Joy Machine](http://joy-machine.com/) - Repo of *Things*
This is Joy Machine's public repository of everything that we can easily separate from our main engine and game source base that we think is useful for people. There are some proprietary things we'd adore to include as well, but alas, we have to have some secrets.

We're adding to this pretty regularly, so check in every now and then to see what's new. And, of course, if there is anything in particular that you're looking for, just put up an Issue in the repo and we'll see what we can do. Or we'll shoot your idea down instantly and viciously 

Most recent change log (which is poorly-updated until we finally find a good generator): [CHANGELOG.md](https://github.com/joymachinegames/joymachine-public/blob/master/CHANGELOG.md).

* `git` - Some git configurations for ignoring files, LFS-managed files, and then detailed config settings for everyday interaction with git on the command line.
* `joy-create` - This is mega early in development (like, two days old), but it's eventually going to be a standalone L-system turtle-based procedural mesh generation system. You'll be able to instantiate any mesh you want using a series of grammars to dictate the progression of the generation, then export the resulting mesh.
* `powershell` - I (trent) use Powershell almost exclusively, so I made some littles scripts as well as some shortcuts I dig.
* `tools` - At the moment, this just contains our batch script for taking megascans output and assembling it into our texture use conventions (three channel-packed textures: albedo (RGB)/displacement (A), metal (R)/roughness (G)/cavity (B)/AO (A), and normal map).
* `ue4` - Our general-purpose Unreal Engine 4 stockpile. Contains our team production conventions and, more interestingly at the moment, a variety of material functions and our "standard shader set" (they're just materials, but I call them shaders because they're pretty... intense). 

More will be added soon!

hugs,
Joy Machine
