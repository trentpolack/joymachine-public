# Megascan Assembler
Tool for adapting [megascan](https://megascans.se/) source textures into packed-channel textures for decoding using shaders.

## License
GNU GPL 3.0

## Usage
```
python megascan.py dir1 dir2 dir3 .. dirN
```
Run over many directories of unpacked megascan bundles. It will automatically figure out which families are represented by those bundles and put them into appropriate packed-channel textures and zip those up.

## Requirements
Defined in requirements.txt (automatically install the dependencies via `pip install -r requirements.txt`)
 * Pillow library

## Packed-channel Textures
A very simple strategy is used to assemble textures. Right now it's set to:
 * Metallic + Roughness + AO -> `_m_r_ao.png`
   * Metallic (R)
   * Roughness (G)
   * AO (B)
 * Cavity -> `_c.png`
 * Albedo -> `_a.png`
 * Opacity (mask) -> `_o.png`
 * Translucency (typically subsurface scattering coloring on foliage) -> `_t.png`
 * Displacement -> `_d.png`
 * Normal map -> `_n.png`