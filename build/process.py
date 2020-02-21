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

print("Pulling latest")
os.system("git pull")
cur_dir = os.getcwd()
os.chdir('../../dreammaker_fx_web')
os.system("git pull")

os.chdir(cur_dir)

shutil.rmtree(html_web_dir[:-1])
os.mkdir(html_web_dir)

for root, subdirs, files in os.walk(rootdir):
    for filename in files:
        if filename[-4:] == ".ino" and filename != "dreammaker_fx_template.ino":
            root_output = root.replace("/sketches/","/autogen/")
            if (not os.path.isdir(root_output)):
                Path(root_output).mkdir(parents=True, exist_ok=True)
            html_output = root_output.replace("../autogen/",html_web_dir)
            if (not os.path.isdir(html_output)):
                Path(html_output).mkdir(parents=True, exist_ok=True)

            # Create various paths
            paths = {}
            paths['hosted_root_html'] = "https://runjumplabs.github.io/dreammaker_fx/sketches/"
            paths['hosted_root'] = "https://raw.githubusercontent.com/runjumplabs/dreammaker_fx_sketches/master/"
            paths['hosted_root_sketch'] = paths['hosted_root'] + "sketches/"
            paths['hosted_root_autogen'] = paths['hosted_root'] + "autogen/"
            paths['local_sketch'] = os.path.join(root, filename)
            paths['sketch_path'] = root.replace("../sketches","")[1:]+"/"
            paths['filename_full'] = filename
            paths['filename_bare'] = filename.replace(".ino","")
            paths['local_autogen_path'] = os.path.join(root, filename).replace("/sketches","/autogen").replace(filename,"")
            paths['local_sketch_path'] = os.path.join(root, filename).replace(filename,"")
            paths['local_hosted_html'] = paths['local_autogen_path'] .replace("../autogen/",html_web_dir)


            # Create data object for this sketch
            this_file_data ={}
            this_file_data['fn'] = filename


            sketch_path = os.path.join(root, filename)
            autogen_path = paths['hosted_root'] + sketch_path.replace("../sketches","/autogen").replace(filename,"")
            html_folder_path = html_html_root + sketch_path.replace("../sketches","").replace(filename,"")

            this_file_data['path'] = paths['sketch_path']
            this_file_data['ino_path'] = paths['hosted_root_sketch'] + paths['sketch_path'] + paths['filename_full']
            this_file_data['html_path'] = paths['hosted_root_html'] + paths['sketch_path'] + "autogen_syntax.html"
            this_file_data['uf2_path'] = paths['hosted_root_autogen'] + paths['sketch_path'] + "CURRENT.UF2"

            # Generate zip file and add path
            zip_path = paths['local_sketch_path']
            zip_fn = paths['local_autogen_path']+paths['filename_bare']
            shutil.make_archive(zip_fn, 'zip', zip_path)
            this_file_data['zip_path'] = paths['hosted_root_autogen'] + paths['sketch_path'] + paths['filename_bare'] + ".zip" 

            # Debug
            if True:
                pprint(paths)
             


            # Process headers and create json
            result = {}  
            with open(sketch_path, 'r') as file:
                source = file.read()
                              

                # Meta data in comments
                delimiters = ["Effect name\:","Effect description\:","Left pot label\:","Left pot function\:","Center pot label\:","Center pot function\:","Right pot label\:","Right pot function\:","Left footswitch label\:","Left footswitch function\:","Right footswitch label\:","Right footswitch function\:","Youtube Url\:","Soundcloud Url\:","Created by\:","DreamMakerFx package version\:", "Sketch version\:","\*\/"]
                titles = ["name","description","left-pot-label","left-pot-function","center-pot-label","center-pot-function","right-pot-label","right-pot-function","left-switch-label","left-switch-function","right-switch-label","right-switch-function","youtube-url","soundcloud-url","creator","package-version","sketch-version"]                
                
                # Assign category based on directory
                p = paths['sketch_path'].split("/")
                result['category'] = p[0].title()

                print(result['category'])

                # Determine what mods this effect uses
                fx_mods = ["fx_adsr_envelope","fx_amplitude_mod","fx_biquad_filter","fx_compressor","fx_delay","fx_destructor","fx_envelope_tracker","fx_gain","fx_looper","fx_mixer_2","fx_mixer_3","fx_mixer_4","fx_instrument_synth","fx_oscillator","fx_phase_shifter","fx_pitch_shift","fx_ring_mod","fx_slicer","fx_variable_delay"]
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

                result.update(this_file_data)               


                with open(os.path.join(paths['local_autogen_path'],'autogen.json'), 'w') as outfile:
                    json.dump(result, outfile)

                this_file_data['json_path'] = paths['hosted_root_autogen'] + paths['sketch_path'] + 'autogen.json'


            # Create binary
            os.system('/Applications/Arduino.app/Contents/MacOS/Arduino --pref build.path=../temp --verify '+paths['local_sketch'])

            # Create UF2 file
            success = False
            if os.path.isfile('../temp/'+paths['filename_full']+".bin"):
                os.system('python3 uf2conv.py ../temp/'+paths['filename_full']+".bin -o "+paths['local_autogen_path']+"CURRENT.UF2 -f SAMD51 -b 0x4000")
                if os.path.isfile(paths['local_autogen_path']+"/CURRENT.UF2"):
                    this_file_data['compiles'] = True
                    success = True
            if not success:
                this_file_data['compiles'] = False
            if len(result['name']) == 0:
                this_file_data['compiles'] = False                


            # Create HTML file
            if (os.path.isfile(paths['local_autogen_path']+"autogen_syntax.html")):
                os.remove(paths['local_autogen_path']+"autogen_syntax.html")

            args = ['node','syntax.js',paths['local_sketch'] , paths['local_hosted_html']+"/autogen_syntax.html"]
            if True:
                subprocess.call(args)
            else:
                subprocess.call('node node syntax.js',paths['local_sketch'] , paths['local_autogen_path']+"autogen_syntax.html")
                print(['node node syntax.js',paths['local_sketch'] , paths['local_autogen_path']+"autogen_syntax.html"].join(", "))
            #copyfile(root_output+"/autogen_syntax.html", html_output+"/autogen_syntax.html")

            if True:
                pprint(this_file_data)

            #success = execute_js('syntax.js '+sketch_path+" "+root+"/autogen_syntax.html")
            # Reset temp directory
            shutil.rmtree("../temp")
            os.mkdir("../temp")

            alldata.append(this_file_data)


with open('../autogen/all_data_autogen.json', 'w') as outfile:
    json.dump(alldata, outfile)



os.system("git add ../")
os.system("git commit -a -m \"Auto-gen update\"")
os.system("git push")
            
os.chdir('../../dreammaker_fx_web')
os.system("git pull")
os.system("git add .")
os.system("git commit -a -m \"Auto-gen update\"")
os.system("git push")       



