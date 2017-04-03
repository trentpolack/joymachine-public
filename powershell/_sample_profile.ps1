Echo "Hello, $env:Username"

$JoyShellDirectory = [Environment]::GetEnvironmentVariable("POWERSHELL_MODULES_DIR","User")
$JoyShellScripts = "$JoyShellDirectory\scripts"
Remove-Variable JoyShellDirectory

Foreach( $PS1File in Get-ChildItem $JoyShellScripts *.ps1 -Recurse ) {
	Invoke-Expression $PS1File.FullName
}

Remove-Variable JoyShellScripts