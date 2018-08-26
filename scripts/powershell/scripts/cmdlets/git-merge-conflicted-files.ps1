function Git-Mergetool-Conflicted
{
    param(
        [string]$root="."
    )

    Get-ChildItem -path $root -include *.cpp, *.c, *.h, *.hpp, *.cs, *.ini, *.py, *.bat -recurse | Select-String ">>>>>>>" -list |
    Foreach-Object {
        $src = $_.FullName
        echo "$src"
        $cmd = "git mergetool $src"
        git mergetool $src
    }
}
Set-Alias git-merge-conflicts Git-Mergetool-Conflicted