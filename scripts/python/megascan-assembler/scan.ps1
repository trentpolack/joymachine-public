mkdir build
cd build

# Generate all of the various built files using the megascan.py script.
Get-ChildItem "$($args)" | Foreach {python ..\megascan.py $_.FullName}

# Delete all individual TGAs, leaving just the zipped archives remaining.
Get-ChildItem -Include *.tga,*.png,*.jpg,*.exr -Recurse | Foreach {Remove-Item $_.FullName}