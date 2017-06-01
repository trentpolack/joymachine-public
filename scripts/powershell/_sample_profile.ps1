Echo "Joy Machine wishes you a lovely day, $env:Username."

$JoyShellDirectory = [Environment]::GetEnvironmentVariable("POWERSHELL_MODULES_DIR","User")
$JoyShellScripts = "$JoyShellDirectory\scripts"
Remove-Variable JoyShellDirectory

Foreach( $PS1File in Get-ChildItem $JoyShellScripts *.ps1 -Recurse ) {
	Invoke-Expression $PS1File.FullName
}

Remove-Variable JoyShellScripts

# I suck at wrapping this command in an actual alias/function, so just send it to the clipboard. Also add alises for the clipboard cmdlets.
function ClipLOC { Set-Clipboard 'Get-ChildItem -Include *.cs,*.h,*.cpp -Recurse | Get-Content | Measure-Object -line -word -character' }
Set-Alias loc 'ClipLOC'
Set-Alias setclip Set-Clipboard
Set-Alias getclip Get-Clipboard