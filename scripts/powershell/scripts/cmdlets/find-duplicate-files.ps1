# find-duplicate-files.ps1
#
# Logs duplicate files in the current directly recursively (based on file name and file size).
#	NOTE: Results are output into duplicate_files.log in the working directory.
#	NOTE: File Checker can be used too, but that's commented out for now since it was having issues with UASSETs.
function Find-Duplicate-Files
{
	[CmdletBinding( )]
	Param()
	
	# Grab Input Directories
	$WorkingDirectory = Get-Location
	
	Write-Verbose "Scanning Path: $WorkingDirectory"
	$Files = gci -path $WorkingDirectory -File -Recurse | Select-Object -property FullName,Name,Length
	$Count = 1
	$TotalFiles = $Files.Count
	$MatchedSourceFiles = @( )
	
	# Setup the log file.
	$LogOutput = -join( $WorkingDirectory, "\duplicate_files.log" )
	if( Test-Path $LogOutput )
	{
		Clear-Content $LogOutput
	}
	else
	{
		New-Item $LogOutput -ItemType file > $null
	}
	Add-Content $LogOutput "Finding Duplicate Files in: $WorkingDirectory"
	Add-Content $LogOutput ""
	
	# Go through the file list.
	ForEach( $SourceFile in $Files )
	{
		Write-Progress -Activity "Processing Files" -status "Processing File $Count / $TotalFiles" -PercentComplete( $Count / $TotalFiles * 100 )
	
		$MatchingFiles = @( )
		$MatchingFiles = $Files | Where-Object { ( $_.FullName -ne $SourceFile.FullName ) -and ( $_.Name -eq $SourceFile.Name ) -and ( $_.Length -eq $SourceFile.Length ) }
		$Matches = @( )
		
		Foreach( $TargetFile in $MatchingFiles )
		{
			if( !( ( $MatchedSourceFiles | Select-Object -ExpandProperty File ) -contains $TargetFile.FullName ) )
			{
#				if( ( fc.exe /A $SourceFile.FullName $TargetFile.FullName ) -contains "FC: no differences encountered" )
#				{
					Write-Verbose "Matched $($SourceFile.FullName) and $($TargetFile.FullName)"
	
					$Matches+= $TargetFile
#				}
			}
		}
	
		if( $Matches.Count -gt 0 )
		{
			$NewObject=[pscustomobject][ordered]@{
				File=$SourceFile.FullName
				MatchingFiles=$Matches
			}
	
			$MatchedSourceFiles+=$NewObject
	
			Add-Content $LogOutput "Duplicates of: $($SourceFile.FullName)"
			Foreach( $MatchFile in $Matches )
			{
				Add-Content $LogOutput "	$($MatchFile.FullName)"
			}
		}
	
		$Count+= 1
	}
}
Set-Alias get-duplicates Find-Duplicate-Files