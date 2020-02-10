from pathlib import Path
from shutil import copyfile
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
html_web_dir = "../../dreammaker_fx_web/sketches/"
html_html_root = "https://runjumplabs.github.io/dreammaker_fx/sketches"

shutil.rmtree(html_web_dir[:-1])
os.mkdir(html_web_dir)

for root, subdirs, files in os.walk(rootdir):
    for filename in files:
        if filename[-4:] == ".ino" and filename != "dreammaker_fx_template.ino":
            print(filename)

            root_output = root.replace("/sketches/","/autogen/")
            if (not os.path.isdir(root_output)):
                Path(root_output).mkdir(parents=True, exist_ok=True)
            html_output = root_output.replace("../autogen/",html_web_dir)
            if (not os.path.isdir(html_output)):
                Path(html_output).mkdir(parents=True, exist_ok=True)

            this_data ={}
            this_data['fn'] = filename
            file_root = filename
            sketch_path = os.path.join(root, filename)
            this_data['ino_path'] = os.path.join(root, filename).replace("..","")
            this_data['path'] = os.path.join(root_output, filename).replace(filename,"").replace("..","")
            this_data['html_path'] = html_html_root + sketch_path.replace("../sketches","").replace(filename,"autogen_syntax.html")

            if False:
                print(sketch_path)
                print(html_html_root)
                print(filename)
                print(this_data['html_path'])
                exit()


            # Process headers and create json
            with open(sketch_path, 'r') as file:
                source = file.read()
                result = {}                

                # Meta data in comments
                delimiters = ["Effect name\:","Effect description\:","Left pot label\:","Left pot function\:","Center pot label\:","Center pot function\:","Right pot label\:","Right pot function\:","Left footswitch label\:","Left footswitch function\:","Right footswitch label\:","Right footswitch function\:","Youtube Url\:","Soundcloud Url\:","Created by\:","DreamMakerFx package version\:", "Sketch version\:","\*\/"]
                titles = ["name","description","left-pot-label","left-pot-function","center-pot-label","center-pot-function","right-pot-label","right-pot-function","left-switch-label","left-switch-function","right-switch-label","right-switch-function","youtube-url","soundcloud-url","creator","package-version","sketch-version"]                
                
                # Assign category based on directory
                p = this_data['path'].split("/")
                result['category'] = p[2].title()

                print(result['category'])

                # Determine what mods this effect uses
                fx_mods = ["fx_adsr_envelope","fx_biquad_filter","fx_compressor","fx_delay","fx_destructor","fx_envelope_tracker","fx_gain","fx_looper","fx_mixer_2","fx_mixer_3","fx_mixer_4","fx_octave","fx_oscillator","fx_phase_shifter","fx_pitch_shift","fx_ring_mod","fx_slicer","fx_variable_delay"]
                result['modules'] = []
                for fx_mod in fx_mods:
                    a = re.findall(fx_mod, source)
                    if (len(a) > 0):
                        result['modules'].append(fx_mod)


                for i, title in enumerate(titles):
                    delim = delimiters[i]
                    delim_next = delimiters[i+1]

                    regex = "\\n"+delim+"(.*?)\\n"+delim_next
                    # print(regex)
                    a = re.findall(regex, source, flags=re.DOTALL)
                    if (len(a) > 0):
                        result[title] = a[0].replace("\n"," ").replace("\r"," ").strip()
                    else:
                        print("MISSING: "+title)
                        pprint(a)
                        pprint(regex)
                        exit()
                        result[title] = ""

                result.update(this_data)

                with open(os.path.join(root_output,'autogen.json'), 'w') as outfile:
                    json.dump(result, outfile)



            # Create binary
            os.system('/Applications/Arduino.app/Contents/MacOS/Arduino --pref build.path=../temp --verify '+sketch_path)
            # Create UF2 file
            success = False
            if os.path.isfile('../temp/'+file_root+".bin"):
                os.system('python3 uf2conv.py ../temp/'+file_root+".bin -o "+root_output+"/CURRENT.UF2 -f SAMD51 -b 0x4000")
                if os.path.isfile(root_output+"/CURRENT.UF2"):
                    this_data['compiles'] = True
                    success = True
            if not success:
                this_data['compiles'] = False

            if len(result['name']) == 0:
                this_data['compiles'] = False                

            # Create HTML file
            if (os.path.isfile(root_output+"/autogen_syntax.html")):
                os.remove(root_output+"/autogen_syntax.html")
            args = ['node','syntax.js',sketch_path, root_output+"/autogen_syntax.html"]
            if True:
                subprocess.call(args)
            else:
                subprocess.call('node node syntax.js',sketch_path, root_output+"/autogen_syntax.html")
            copyfile(root_output+"/autogen_syntax.html", html_output+"/autogen_syntax.html")


            #success = execute_js('syntax.js '+sketch_path+" "+root+"/autogen_syntax.html")
            # Reset temp directory
            shutil.rmtree("../temp")
            os.mkdir("../temp")

            alldata.append(this_data)

with open('../autogen/all_data_autogen.json', 'w') as outfile:
    json.dump(alldata, outfile)


os.system("git pull")
os.system("git add ../")
os.system("git commit -a -m \"Auto-gen update\"")
os.system("git push")
            
os.chdir('../../dreammaker_fx_web')
os.system("git pull")
os.system("git add .")
os.system("git commit -a -m \"Auto-gen update\"")
os.system("git push")       



