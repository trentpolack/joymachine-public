Echo "Joy Machine wishes you a lovely day, $env:Username."

$JoyShellDirectory = [Environment]::GetEnvironmentVariable("POWERSHELL_MODULES_DIR","User")
$JoyShellScripts = "$JoyShellDirectory\scripts"
Remove-Variable JoyShellDirectory

Foreach( $PS1File in Get-ChildItem $JoyShellScripts *.ps1 -Recurse ) {
	Invoke-Expression $PS1File.FullName
}

Remove-Variable JoyShellScripts

# Count lines of code in CS, H, CPP, JS, and HTML files. Functions/aliases for copying the command to the clipboard or executing it out-right.
function ClipLOC { Set-Clipboard 'Get-ChildItem -Include *.cs,*.h,*.cpp,*.js,*.html -Recurse | Get-Content | Measure-Object -line -word -character' }
function GetLOC { Get-ChildItem -Include *.cs,*.h,*.cpp,*.js,*.html -Recurse | Get-Content | Measure-Object -line -word -character }
Set-Alias get-loc 'GetLOC'
Set-Alias clip-loc 'ClipLOC'
Set-Alias setclip Set-Clipboard
Set-Alias getclip Get-Clipboard

# Superfetch management.
function SuperfetchEnable { net start superfetch }
function SuperfetchDisable { net stop superfetch }
Set-Alias superfetch_on SuperfetchEnable
Set-Alias superfetch_off SuperfetchDisable

# System/File System commands.
function GetEnv { gci env: | Format-Table -Wrap -AutoSize }
Set-Alias get-env GetEnv
function GetExtensionsRecursively { dir -recurse | Select-Object extension -Unique }
Set-Alias get-extensions GetExtensionsRecursively

function TopMemoryProcesses { Get-Process | Sort-Object -Property WS | Select-Object -Last 5 }
Set-Alias check-processmemory TopMemoryProcesses

# Pretty file directory printout (I don't feel like handling an unspecified filename atm).
function GetDirTree { Tree . /F | Select-Object -Skip 2 | Set-Content $args }
Set-Alias get-dirtree GetDirTree
