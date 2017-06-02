import os, os.path
import shutil
from datetime import datetime
import sys

def do_folder(src_folder):
    assert os.path.isdir(src_folder)
    dst_folder = os.path.join(src_folder, 'timestamp_prefixed')
    print 'Input dir: %s'%src_folder
    print 'Output dir: %s'%dst_folder
    if not os.path.exists(dst_folder):
        print 'Creating %s' %dst_folder
        os.makedirs(dst_folder)
    for f in os.listdir(src_folder):
        base,ext = os.path.splitext(f)
        src_file = os.path.join(src_folder, f)
        if os.path.isfile(src_file) and ext.lower() == '.png':
            ctime = os.path.getctime(src_file)
            dt = datetime.utcfromtimestamp(ctime)
            dst_file = os.path.join(dst_folder, dt.strftime("%Y_%m_%d_")+f)
            print '%s ---> %s' % (src_file,dst_file)
            shutil.copyfile(src_file,dst_file)

if __name__ == '__main__':
    folders = sys.argv[1:]
    if len(folders) == 0:
        do_folder('.')
    else:
        for f in folders:
            do_folder(f)

