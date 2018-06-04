# Commit information JSON format converter

## Requirements
 * Python 3.x
 
## Usage
 * `python commit_counter.py <input file name>`
 * By default, assumes UTF-16 encoding for input file if BOM is present, otherwise assumes input is UTF-8
 	* Can override with `--encoding <encodingname>` e.g. `--encoding utf-8` 
 * By default, writes output to stdout. Can specify a specific output file with `-o`/`--output`.
