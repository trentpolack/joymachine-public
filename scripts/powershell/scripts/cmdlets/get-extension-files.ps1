function GetFilesOfExtension
{
    [CmdletBinding(SupportsShouldProcess=$true)]
    param(
        # Specifies a path to one or more locations.
        [Parameter(Mandatory=$true,
                    Position=0,
                    ParameterSetName="Extension",
                    ValueFromPipeline=$true,
                    ValueFromPipelineByPropertyName=$true,
                    HelpMessage="Extension to list files for.")]
        [Alias("PSExtension")]
        [ValidateNotNullOrEmpty()]
        [string[]]
        $Extension
    )

    begin {

    }   
    
    process {
        Get-ChildItem -Filter "*.$($Extension)" -Recurse -ErrorAction SilentlyContinue -Force | Resolve-Path -Relative
    }

    end {

    }
}
Set-Alias get-extension-files GetFilesOfExtension