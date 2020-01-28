import csv
from os import listdir
from os.path import isfile, join
from pprint import pprint
import re
import os
import shutil


rootdir = '../sketches/'
for root, subdirs, files in os.walk(rootdir):
    for filename in files:
        if filename[-4:] == ".ino" and filename != "dreammaker_fx_template.ino":
            file_root = filename
            sketch_path = os.path.join(root, filename)
            print(sketch_path)
            os.system('/Applications/Arduino.app/Contents/MacOS/Arduino --pref build.path=../temp --verify '+sketch_path)
            os.system('python3 uf2conv.py ../temp/'+file_root+".bin -o "+root+"/CURRENT.UF2 -f SAMD51 -b 0x4000")
            shutil.rmtree("../temp")
            os.mkdir("../temp")
            



