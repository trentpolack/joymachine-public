mkdir build
cd build

# Generate all of the various built files using the megascan.py script.
Get-ChildItem E:\megascans\surfaces | Foreach {python ..\megascan.py $_.FullName}

# Delete all individual TGAs, leaving just the zipped archives remaining.
Get-ChildItem -Include *.tga -Recurse | Foreach {Remove-Item $_.FullName}