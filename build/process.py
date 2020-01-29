import json
from os import listdir
from os.path import isfile, join
from pprint import pprint
import re
import os
import shutil
import subprocess

alldata = []
rootdir = '../sketches/'
for root, subdirs, files in os.walk(rootdir):
    for filename in files:
        if filename[-4:] == ".ino" and filename != "dreammaker_fx_template.ino":
            this_data ={}
            this_data['fn'] = filename
            file_root = filename
            sketch_path = os.path.join(root, filename)
            this_data['ino_path'] = sketch_path.replace("../sketches/","")
            this_data['path'] = sketch_path.replace("../sketches/","").replace(filename,"")

            # Process headers and create json
            with open(sketch_path, 'r') as file:
                source = file.read()
                result = {}

                delimiters = ["Effect name\:","Effect description\:","Left pot label\:","Left pot function\:","Center pot label\:","Center pot function\:","Right pot label\:","Right pot function\:","Left footswitch label\:","Left footswitch function\:","Right footswitch label\:","Right footswitch function\:","Youtube Url\:","Soundcloud Url\:","Created by\:","DreamMakerFx package version\:", "Sketch version\:","\*\/"]
                titles = ["name","description","left-pot-label","left-pot-function","center-pot-label","center-pot-function","right-pot-label","right-pot-function","left-switch-label","left-switch-function","right-switch-label","right-switch-function","youtube-url","soundcloud-url","creator","package-version","sketch-version"]
                
                for i, title in enumerate(titles):
                    delim = delimiters[i]
                    delim_next = delimiters[i+1]

                    regex = "\\n"+delim+"(.*?)\\n"+delim_next
                    # print(regex)
                    a = re.findall(regex, source, flags=re.DOTALL)
                    if (len(a) > 0):
                        result[title] = a[0].replace("\n"," ").replace("\r"," ").strip()

                    with open(os.path.join(root,'autogen.json'), 'w') as outfile:
                        json.dump(result, outfile)


            # Create binary
            os.system('/Applications/Arduino.app/Contents/MacOS/Arduino --pref build.path=../temp --verify '+sketch_path)
            # Create UF2 file
            os.system('python3 uf2conv.py ../temp/'+file_root+".bin -o "+root+"/CURRENT.UF2 -f SAMD51 -b 0x4000")
            # Create HTML file
            args = ['node','syntax.js',sketch_path, root+"/autogen_syntax.html"]
            if True:
                subprocess.call(args)
            else:
                subprocess.call('node node syntax.js',sketch_path, root+"/autogen_syntax.html")

            #success = execute_js('syntax.js '+sketch_path+" "+root+"/autogen_syntax.html")
            # Reset temp directory
            shutil.rmtree("../temp")
            os.mkdir("../temp")

            alldata.append(this_data)

with open(os.path.join(root,'../all_data_autogen.json'), 'w') as outfile:
    json.dump(alldata, outfile)
            



