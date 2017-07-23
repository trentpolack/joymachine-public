function Compress-PNGs
{
    param(
        [string]$root="."
    )

    Get-ChildItem -path $root -filter *.png -recurse |
    Foreach-Object {
        $src = $_.FullName
        echo "$src"
        $dest = "$src.compress-temp"
        magick convert $src $dest
        if($LastExitCode -ne 0)
        {
            echo "--> ImageMagick failed somehow"
            Remove-Item $dest
        }
        else
        {
            $srcInfo = Get-Item $src
            $destInfo = Get-Item $dest
            if ($destInfo.Length -lt ($srcInfo.Length * 0.98))
            {
                echo "--> Compressed"
                Move-Item $dest $src -force
            }
            else
            {
                echo "--> Failed to compress better, skipping"
                Remove-Item $dest
            }
        }
    }
}
