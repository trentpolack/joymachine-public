# [Joy Machine](http://joy-machine.com/) - Git Template Files
These are just some handy references for gitignore/gitattributes for handling a UE4 repo using git-lfs. And then the gitconfig has some of the best aliases ever in the world.

The entirety of this doc is for use with git on the command line (as a note: you do *not* have to use git bash; it can be easily integrated into the standard Powershell and Windows cmd prompt). The only *remotely* decent GUI client for git on Windows is [GitKraken](https://www.gitkraken.com) -- but the number of "quirks" that it has makes it unusable in my opinion. So: command line.

## `difftool` and `mergetool`
Setting up a difftool and mergetool for the git command line is a lovely thing to do, unless you like pain. Or *really* like text editors. Customizing these commands will allow you to use a GUI-based comparison tool to handle merges and differences between files. Up until yesterday, I'd have recommended [`p4merge`](https://www.perforce.com/product/components/perforce-visual-merge-and-diff-tools) (from Perforce). Then I found [Beyond Compare 4](https://www.scootersoftware.com/) -- ignore the site design, it's great software -- which I used at my first professional game development job and had *no* idea it was still around. Unlike `p4merge`, Beyond Compare offers an easy solution to simply compare two folders (whether they're in a repository or not) and git a list of files that are added/removed/changed/unchanged in the file hierarchy. And then you can go through and diff/merge what you want to.

Point being: these are great tools to build into the git command line. This is my personal global gitconfig:

```
[merge]
	tool = bc3

[mergetool "bc3"]
	path = C:\\Program Files\\Beyond Compare 4\\bcomp.exe
	prompt = false

[diff]
	tool = bc3

[difftool "bc3"]
	path = C:\\Program Files\\Beyond Compare 4\\bcomp.exe
	prompt = false
```

This allows me to use Beyond Compare whenever I enter `git difftool <file>` (which will check the remote version of the file); `prompt = false` ensures I don't have to agree to a (Y/N) question for every file I diff. And, alternatively, if you just enter `git difftool` to bring up a diff of all modified files. This is all true of mergetool as well. I tossed the `prompt = false` into the `gitconfig.template` for reference. 

## Colors
Additionally, I added some colors for varying results printed by `git status`, `git branch`, and `git diff`. Here's the list of valid colors:
* `normal`.
* `black`
* `red`.
* `green`.
* `yellow`.
* `blue`.
* `magenta`.
* `cyan`.
* `white`.

### Color Decorators
AND YOU CAN DECORATE THEM!
* `bold`.
* `dim`.
* `ul` (underline).
* `blink` (rock it like an Angelfire website).
* `reverse` (swap foreground/background colors).

## Aliases
* `git s` -- Short format `git status` (no lfs status report like `git state`, below). With colors!
* `git state` -- Does a full evaluation of your current repo state across git and git-lfs, with some decent formatting to separate the two. Does not display git-lfs info if the repo doesn't use lfs.
* `git lard` -- Lists the top 40 heaviest assets in your repository.
* `git last` -- Just print out the last commit to your command line.
* `git uncommit` -- Uncommit whatever you just did.
* `git prettylog` -- Pretty log with some branch graphics.
* `git tree` -- Just prints a pretty-formatted revision history in tree form.
