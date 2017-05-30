# touch.ps1
# Copyright 2015-2017 Joy Machine, LLC. All rights reserved.
# 
# Create a cmdlet called `Set-FileTime` and an alias for that cmdlet called `touch`. (Emulate linux `touch` behavior).
# Original Source: http://sushihangover.blogspot.com/2012/10/powershell-true-touch-file-routine.html
function Set-FileTime
{
	param( 
		[string[]]$paths,
		[bool]$only_modification = $false,
		[bool]$only_access = $false
	 );

	begin
  {
    # Get updated filesystem information.
		function UpdateFileSystemInfo( [System.IO.FileSystemInfo]$fsInfo )
    {
			$datetime = get-date
			if(	$only_access )
			{
        $fsInfo.LastAccessTime = $datetime
			}
			elseif(	$only_modification )
			{
        $fsInfo.LastWriteTime = $datetime
			}
			else
			{
        $fsInfo.CreationTime = $datetime
        $fsInfo.LastWriteTime = $datetime
			  $fsInfo.LastAccessTime = $datetime
	    }
		}
	 
    # Touch an existing file.
		function TouchExistingFile( $arg )
    {
			if( $arg -is [System.IO.FileSystemInfo] )
      {
				UpdateFileSystemInfo( $arg )
			}
			else
      {
				$resolvedPaths = resolve-path $arg
				foreach( $rpath in $resolvedPaths )
        {
					if( test-path -type Container $rpath )
          {
						$fsInfo = New-Object System.IO.DirectoryInfo( $rpath )
					}
					else
          {
						$fsInfo = New-Object System.IO.FileInfo( $rpath )
					}
	
  				UpdateFileSystemInfo( $fsInfo )
				}
			}
		}

    # Create a new file with null contents.
		function TouchNewFile( [string]$path ) {
			# $null > $path
			Set-Content -Path $path -Value $null;
		}
	}
 
	process
  {
		if( $_ )
    {
			if( Test-Path $_ )
      {
				TouchExistingFile( $_ )
			}
			else
      {
				TouchNewFile( $_ )
			}
		}
	}
 
	end
  {
		if( $paths )
    {
			foreach( $path in $paths )
      {
				if( Test-Path $path )
        {
					TouchExistingFile( $path )
				}
				else
        {
					TouchNewFile( $path )
				}
			}
		}
	}
}

# Set touch as an alias for Set-FileTime.
New-Alias touch Set-FileTime