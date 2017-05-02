# [Joy Machine](http://joy-machine.com/) - Git Template Files
These are just some handy references for gitignore/gitattributes for handling a UE4 repo using git-lfs. And then the gitconfig has some of the best aliases ever in the world.

## Aliases
* `git state` -- Does a full evaluation of your current repo state across git and git-lfs, with some decent formatting to separate the two. Does not display git-lfs info if the repo doesn't use lfs.
* `git last` -- Just print out the last commit to your command line.
* `git tree` -- Just prints a pretty-formatted revision history in tree form.
* `git cl` -- Generated a simple CHANGELOG.md based on commit log.
	* NOTE: This requires [git-extras](https://github.com/tj/git-extras) installed. For now, anyway. We're working on our own little solution right now.