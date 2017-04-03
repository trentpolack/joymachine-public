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

## Usage
 * Execute `touch rawr.txt` to:
 	* Create a new file, if it doesn't already exist, modified at the current date and time.
	* Modify the modification date/time of an existing file named `rawr.txt`.
	* NOTE: Useful for simple git status checks.