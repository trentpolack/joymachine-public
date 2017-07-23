# [Joy Machine](https://joy-machine.com/) - Repo of *Things*
### Currently Being Developed Alongside [*Steel Hunters*](https://joy-machine.com/steel-hunters)
This is Joy Machine's public repository of everything that we can easily separate from our main engine and game source base that we think is useful for people. There are some proprietary things we'd adore to include as well, but alas, we have to have some secrets.

We're adding to this pretty regularly, so check in every now and then to see what's new. And, of course, if there is anything in particular that you're looking for, just put up an Issue in the repo and we'll see what we can do. Or we'll shoot your idea down instantly and viciously 

Most recent change log (which is poorly-updated until we finally find a good generator): [CHANGELOG.md](https://github.com/joymachinegames/joymachine-public/blob/master/CHANGELOG.md).

* `git` - Some git configurations for ignoring files, LFS-managed files, and then detailed config settings for everyday interaction with git on the command line.
* `tools` - Tools for content generation (`joy-create` -- nowhere near useful yet and `jmkernels` for generating an HDR/EXE file for use as an FFT bloom convolution image in UE 4.16+ (can be customized further for stylization).
* `scripts` - Miscellaneous scripts (tools, powershell, visual studio extensions, batch files, etc.) 
* `ue4-source` - Whatever C++ actors/components/utilities/etc. we end up being able to easily divorce from our projects. Or, if not easily divorced, whatever seems worth divorcing so others never have to know the pain that we did. This will likely be the least frequently-updated part of the repo.
* `ue4` - Our general-purpose Unreal Engine 4 stockpile. Contains our team production conventions and, more interestingly at the moment, a variety of material functions and our "standard shader set" (they're just materials, but I call them shaders because they're pretty... intense). There is also an old procedural water and wave generation/rendering asset set (all done through the material editor) if, you know, you like water.

### Other Things of Note:
* [AWESOME_SOFTWARE.md](https://github.com/joymachinegames/joymachine-public/blob/master/AWESOME_SOFTWARE.md) -- Has a list of all of the software that I (trent) have found to be the best solution within its given context.
* [AWESOME_GAMEDEV_MENTORS](https://ishansharma.github.io/twitter-mentors/) -- Technically, it's "twitter-mentors", but I like my name better. Anyway, this is [@ishansharma](https://github.com/ishansharma)'s compilation of people ([@trentpolack](https://github.com/trentpolack) included) who offer game development mentoring to anyone that seeks it.

### More to Come!
hugs,
Joy Machine

If you have any questions, feel free to email us: [joy@joy-machine.com](mailto:joy@joy-machine.com).
