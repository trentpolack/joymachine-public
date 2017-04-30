echo off
set existingBranchName=%1
set newBranchName=%2
shift
shift

git branch -m %existingBranchName% %newBranchName%
git push origin :%existingBranchName%  
git push --set-upstream origin %newBranchName%