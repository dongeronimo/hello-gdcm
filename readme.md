A dicom loader using gdcm

Originally created to load dicoms for an android Unity app, will probably load dicoms anywhere else.

Setup:
1) clone the repo
2) init submodules because gdcm is a submodule
3) use cmake to build gdcm: i use as binary destination ./GDCM-build and as install dir ./libgdcm (that's why these paths are in gitignore)
4) build gdcm, run build_all and install
5) use cmake for the hello-gdcm project, i use ./build as binary destination. (that's why this path is in gitignore)

Licence: simplified BSD license, approved by the Open Source Initiative (OSI)
* http://www.opensource.org/licenses/bsd-license.php