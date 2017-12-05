# find-duplicate-files.ps1
#
# Finds duplicate files in the current directly recursively and then outputs the duplicates.
#	NOTE: I don't automatically siphon the results into an output log, but that's easy enough to do outside the script.
function Find-Duplicate-Files
{
    [CmdletBinding( )]
	#Grab Input Directories
	$WorkingDirectory = Get-Location
	
	if( !(Test-Path -PathType Container $WorkingDirectory ) )
	{
		Write-Error "Invalid path specified."
		Exit
	}
	
	Write-Verbose "Scanning Path: $WorkingDirectory"
	$Files=gci -File -Recurse -path $WorkingDirectory | Select-Object -property FullName,Length
	$Count=1
	$TotalFiles=$Files.Count
	$MatchedSourceFiles=@( )
	
	ForEach( $SourceFile in $Files )
	{
		Write-Progress -Activity "Processing Files" -status "Processing File $Count / $TotalFiles" -PercentComplete( $Count / $TotalFiles * 100 )
		$MatchingFiles=@( )
		$MatchingFiles=$Files |Where-Object {$_.Length -eq $SourceFile.Length}
		Foreach( $TargetFile in $MatchingFiles )
		{
			if( ($SourceFile.FullName -ne $TargetFile.FullName ) -and !(($MatchedSourceFiles | Select-Object -ExpandProperty File ) -contains $TargetFile.FullName ) )
			{
				Write-Verbose "Matching $($SourceFile.FullName) and $($TargetFile.FullName)"
				Write-Verbose "File sizes match."
				if( (fc.exe /A $SourceFile.FullName $TargetFile.FullName )  -contains "FC: no differences encountered" )
				{
					Write-Verbose "Match found."
					$MatchingFiles+=$TargetFile.FullName
				}
			}
		}
		if( $MatchingFiles.Count -gt 0 )
		{
			$NewObject=[pscustomobject][ordered]@{
				File=$SourceFile.FullName
				MatchingFiles=$MatchingFiles
			}
			$MatchedSourceFiles+=$NewObject
		}
		$Count+=1
	}
	
	$MatchedSourceFiles
}
Set-Alias get-duplicates Find-Duplicate-Files