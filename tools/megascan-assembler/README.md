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
Right now the only textures it generates are:
 * Normal map (`n.tga`) - Direct copy of the Normal.jpg source texture
 * Albedo/displacement (`a_d.tga`) - Albedo.jpg packed into RGB, displacement packed into the alpha channel
 * Metallic/Roughness/Cavity/AO (`m_r_c_ao.tga`) - Metallic packed into R, Roughness packed into G, Cavity packed into B, AO packed into the alpha channel.
  * If Metallic does not exist, will just render 0 into R.
  * TODO: Implement Specular/Roughness/Cavity/AO
