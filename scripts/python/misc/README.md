## rename_images_by_timestamp.py ##

For a given directory (or list of directories), all png, jpg and gif files are copied to a new subdirectory `timestamp_prefixed`, with their UTC modification date prefixed to the new filename.

For example, if you have a directory named `foo` that contains `bar.png` that was modified on 2017-01-02 UTC, it will create a new file `foo/timestamp_prefixed/2017_01_02_bar.png`.

### Usage ###

`python rename_images_by_timestamp.py [SRCDIR ...]`

If no directories are specified, it operates on the current directory (`.`)
Multiple directories can be specified.

### Requirements ###
* Python 2.7