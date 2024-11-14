import sys
import shutil
import os
script_name = sys.argv[0]
arguments = sys.argv[1:]

source_folder = arguments[0]+"/build/Debug/"
destination_folder = arguments[1]+"/build/Debug/"
print(f"{source_folder}")
print(f"{destination_folder}")
#delete previous folder
if os.path.exists(destination_folder):
   shutil.rmtree(destination_folder)
   print(f"{destination_folder} deleted.")
else:
   print(f"{destination_folder} does not exist.")

os.makedirs(destination_folder)
print(f"{source_folder+"tomographer.dll"}")
print(f"{destination_folder+"tomographer.dll"}")
shutil.copyfile(source_folder+"tomographer.dll", destination_folder+"tomographer.dll")