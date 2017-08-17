$git_generate_changelog = "$("cl")"
Start-Process -FilePath git -WorkingDirectory "." -Wait -NoNewWindow -ArgumentList "$git_generate_changelog"

# Remove these lines if a HISTORY.md file is not necessary.
Get-Content CHANGELOG.md,HISTORY.md | Set-Content CHANGELOG-tmp.md
Get-Content CHANGELOG-tmp.md | Set-Content CHANGELOG.md
Remove-Item CHANGELOG-tmp.md