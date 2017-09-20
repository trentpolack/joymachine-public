from PIL import Image
import sys, os
import zipfile

TEXTURE_FILE_PREFIX = "t_"

def get_family_root(directoryPath):
    """ guess the path on disk to the texture family's directory """
    return os.path.basename(os.path.normpath(directoryPath))

def get_filename_root(directoryPath):
    """ guess the prefix of texture filenames """
    # guess the filename root by figuring out the full filename for 'albedo'
    albedo_path = next(f for f in os.listdir(directoryPath) if "albedo" in f.lower())
    if albedo_path == None:
        print('Could not find an albedo file in directory %s. Aborting.' % directoryPath)
    # ...then chop that full filename, to guess the prefix
    albedo_idx = albedo_path.lower().find("albedo")
    return albedo_path[:albedo_idx]

def get_filename_for_channel(directoryPath, filenameRoot, channel_type):
    """ combine the directory path and filename root to guess the filename for a source image """
    # e.g. "pkfg22_4K_" + "Roughness.jpg"
    return os.path.join(directoryPath, filenameRoot + channel_type)

def is_combined_texture_plan(plan):
    """
    If true, this texture is composed of three other single-channel textures,
    which are copied into the R, G, and B channels of the destination image.
    """
    return 'r' in plan and 'g' in plan and 'b' in plan

def do_single_channel(directoryPath, familyRoot, filenameRoot, outputSuffix, single_channel_name):
    """ Take a texture from disk and convert it to a single-channel texture """
    output_file_path = lower(TEXTURE_FILE_PREFIX + familyRoot + outputSuffix)

    # load the channel
    sourceChannelPath = get_filename_for_channel(directoryPath, filenameRoot, single_channel_name)
    try:
        sourceChannel = Image.open(sourceChannelPath)
    except IOError:
        return
    else:
        with Image.new('L', sourceChannel.size) as output:
            output.paste(sourceChannel)
            output.save(output_file_path)
            return output_file_path

    # File doesn't exist.
    return

def do_saveas(directoryPath, familyRoot, filenameRoot, outputSuffix, output_name):
    """ Take an RGB texture from disk and save it as an RGB texture with the name and format we expect """
    output_file_path = lower(TEXTURE_FILE_PREFIX + familyRoot + outputSuffix)

    # Just copy the entire image over.
    source_path = get_filename_for_channel(directoryPath, filenameRoot, output_name)

    try:
        source = Image.open(source_path)
    except IOError:
        return
    else:
        source.save(output_file_path)
        return output_file_path

    return

def do_rgb(directoryPath, familyRoot, filenameRoot, outputSuffix, plan):
    """ Take multiple textures and combine them into a single RGB texture. """
    output_file_path = lower(TEXTURE_FILE_PREFIX + familyRoot + outputSuffix)

    # HACK: since M is optional in M_R_AO, we're going to go backwards
    # and just guess that AO is always here and M isn't. in the future, maybe
    # add some magic to the filename for 'is optional?' but then how do we decide
    # sizes? who cares for now...
    b_path = get_filename_for_channel(directoryPath, filenameRoot, plan['b'])
    g_path = get_filename_for_channel(directoryPath, filenameRoot, plan['g'])
    r_path = get_filename_for_channel(directoryPath, filenameRoot, plan['r'])

    with Image.open(b_path) as bSource:
        with Image.open(g_path) as gSource:
            if os.path.exists(r_path):
                rSource = Image.open(r_path)
            else:
                rSource = Image.new('RGB', bSource.size)

            output = Image.merge('RGB', (rSource.split()[0], gSource.split()[0], bSource.split()[0]))
            output.save(output_file_path)

            rSource.close()

    return output_file_path

def pack_directory(directoryPath, plan):
    if not os.path.isdir(directoryPath):
        print('Directory %s does not exist. Aborting.' % (directoryPath))
        sys.exit(1)
    family_root = get_family_root(directoryPath)
    filename_root = get_filename_root(directoryPath)
    print('Handling family %s at %s*.jpg' % (family_root, filename_root))

    converted = []
    for planned_suffix in plan:
        planned = plan[planned_suffix]
        # determine k or r, g, b
        if 'k' in planned.keys():
            greyscale_result = do_single_channel(directoryPath, family_root, filename_root, planned_suffix, planned['k'])
            if greyscale_result is not None:
                # single channel mode
                converted.append(greyscale_result)
                print 'Rule ' + planned_suffix + ' (1-channel) generated file ' + greyscale_result
        elif 'rgb' in planned.keys():
            # copy this texture directly
            copied_result = do_saveas(directoryPath, family_root, filename_root, planned_suffix, planned['rgb'])
            if copied_result is not None:
                converted.append(copied_result)
                print 'Rule ' + planned_suffix + ' was converted to RGB texture ' + copied_result
        elif is_combined_texture_plan(planned):
            # assume r, g, b => rgb
            colour_result = do_rgb(directoryPath, family_root, filename_root, planned_suffix, planned)
            converted.append(colour_result)
            print 'Rule ' + planned_suffix + ' (RGB) generated file ' + colour_result
        else:
            print 'Error in build plan: Do not know how to build ' + planned_suffix + ', channels provided are ' + ', '.join(planned.keys())
            return

    # all done, bundle them into a zip.
    archive_family(family_root, converted)

def archive_family(family_root, paths):
    zipname = lower(family_root.strip("-_ ") + ".zip")
    with zipfile.ZipFile(zipname, mode = 'w') as z:
        for path in paths:
            z.write(path)
    print(' ~> Generated zip archive of %s called %s' % (family_root, zipname))

def lower(str):
    return str.lower()

def printUsage():
    print('Usage: %s [directories]' % (sys.argv[0]))

def main():
    plan = {
        # r = red, g = green, b = blue, k = grey (single channel), 'rgb' = copy directly
        '_m_r_ao.png': {
            # Metallic, roughness, AO
            'r': 'Metallic.jpg',
            'g': 'Roughness.jpg',
            'b': 'AO.jpg'
        },
        '_n.png': {
            # Normal map
            'rgb': 'Normal.jpg'
        },
        '_c.png': {
            # Just cavity
            'k': 'Cavity.jpg'
        },
        '_o.png': {
            # Opacity (mask).
            'k': 'Opacity.jpg'
        },
        '_t.png': {
            # Translucency (subsurface scattering -- generally reserved only for foliage).
            'rgb': 'Translucency.jpg'
        },
        '_a.png': {
            # Just albedo
            'rgb': 'Albedo.jpg'
        },
        '_d.png': {
            # Displacement map (single channel texture)
            'k': 'Displacement.jpg'
        }
    }

    if len(sys.argv) < 2:
        printUsage()
        sys.exit(1)
    for d in sys.argv[1:]:
        pack_directory(d, plan)

if __name__ == '__main__': main()
