from PIL import Image
import sys, os
import zipfile

TEXTURE_FILE_PREFIX = "t_"

def get_family_root(directoryPath):
    return os.path.basename(os.path.normpath(directoryPath))

def get_filename_root(directoryPath):
    # figure out who the albedo is
    albedo_path = next(f for f in os.listdir(directoryPath) if "albedo" in f.lower())
    if albedo_path == None:
        print('Could not find an albedo file in directory %s. Aborting.' % directoryPath)
    albedo_idx = albedo_path.lower().find("albedo")
    return albedo_path[:albedo_idx]

def get_filename_for_channel(directoryPath, filenameRoot, channel_type):
    # e.g. "pkfg22_4K_" + "Roughness.jpg"
    return os.path.join(directoryPath, filenameRoot + channel_type)

def is_rgb_plan(plan):
    return 'r' in plan and 'g' in plan and 'b' in plan

def do_single_channel(directoryPath, familyRoot, filenameRoot, outputSuffix, single_channel_name):
    output_file_path = lower(TEXTURE_FILE_PREFIX + familyRoot + outputSuffix)

    # load the channel
    sourceChannelPath = get_filename_for_channel(directoryPath, filenameRoot, single_channel_name)
    with Image.open(sourceChannelPath) as sourceChannel:
        with Image.new('L', sourceChannel.size) as output:
            output.paste(sourceChannel)
            output.save(output_file_path)
    return output_file_path

def do_rgb(directoryPath, familyRoot, filenameRoot, outputSuffix, plan):
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
            # single channel mode
            greyscale_result = do_single_channel(directoryPath, family_root, filename_root, planned_suffix, planned['k'])
            converted.append(greyscale_result)
            print 'Rule ' + planned_suffix + ' (1-channel) generated file ' + greyscale_result
        elif is_rgb_plan(planned):
            # assume r, g, b
            colour_result = do_rgb(directoryPath, family_root, filename_root, planned_suffix, planned)
            converted.append(colour_result)
            print 'Rule ' + planned_suffix + ' (RGB) generated file ' + colour_result
        else:
            print 'Error in build plan: Do not know how to handle ' + planned_suffix + ', channels provided are ' + ', '.join(planned.keys())
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
        # r = red, g = green, b = blue, k = grey (single channel)
        '_m_r_ao.png': {
            # Metallic, roughness, AO
            'r': 'Metallic.jpg',
            'g': 'Roughness.jpg',
            'b': 'AO.jpg'
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
            'k': 'Translucency.jpg'
        },
        '_a.png': {
            # Just albedo
            'k': 'Albedo.jpg'
        },
        '_d.png': {
            # Displacement map (single channel texture)
            'k': 'Displacement.jpg'
        },
        '_n.png': {
            # Normal map
            'k': 'Normal.jpg'
        }
    }

    if len(sys.argv) < 2:
        printUsage()
        sys.exit(1)
    for d in sys.argv[1:]:
        pack_directory(d, plan)

if __name__ == '__main__': main()
