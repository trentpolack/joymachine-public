# Windows PowerShell 5 Scripts
## Copyright 2015-2017 [Joy Machine](https://joy-machine.com), LLC. All rights reserved.

## Setup
 * Create a user-level environment variable for your system:
 	* Name: `POWERSHELL_MODULES_DIR`
	* Value: <Insert Path to .\powershell>
 * From a PowerShell instance:
 	* Execute: `type .\_sample_profile.ps1 | clip`
	 	* NOTE: This will overwrite the contents of your clipboard.
	* Execute `notepad $Profile`
		* NOTE: You can replace `notepad` with the text editor of your choice (we use VS Code).
	* In the newly-opened text file either simply paste the contents of your clipboard (Ctrl+V, right-click "Paste", etc.) into the empty file, or append it to a new line in your existing PowerShell profile.
	* Save the file. Close your text editor.
	* Back in your PowerShell instance, execute: `.$Profile`
		* This will reload your profile with the new contents.
		* If SUCCESSFUL (!), you should see a "Hello, <your username>" upon execution.

## General Scripts
 * `generate-changelog.ps1`:
	* Execute this script from a powershell session and it will look for (and update) `CHANGELOG.md` and append the contents of `HISTORY.md` (in case you have a changelog from a separate repo with more history) to it.
		* If the history isn't needed, just remove everything below "Remove these lines if a HISTORY.md file is not necessary.".
	* Changelog details will be split up by tags.

## Usage
 * Execute `touch rawr.txt` to:
 	* Create a new file, if it doesn't already exist, modified at the current date and time.
	* Modify the modification date/time of an existing file named `rawr.txt`.
	* NOTE: Useful for simple git status checks.
		* ISSUE: Upon testing this, git does not actually respect this. Will investigate. I'm on a roll today [Trent, 4/3/17].
 * Execute `Compress-PNGS -root <directory>` to:
    * Iterate through all PNGs in that directory recursively. Compress them to a temp file. If temp file is significantly smaller than
      the original file, replace original file with temp file.
    * Requires ImageMagick's `magick` to be in your `PATH`.

## One-Liners
 * Replace filename text within a directory (X being source substring, Y being target substring):
	* `Get-ChildItem *.h -Recurse | Rename-Item -NewName {$_.name -replace '^X','Y'}`