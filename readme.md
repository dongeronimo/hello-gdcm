A dicom loader using gdcm

Originally created to load dicoms for an android Unity app, will probably load dicoms anywhere else.

Setup:
1) Run setup_dependencies.py. It should fetch and build the dependencies.
2) Run setup_project.py. It should at least generate the project (at ./build)

OR

1) clone the repo
2) init submodules because gdcm is a submodule
3) use cmake to build gdcm: i use as binary destination ./GDCM-build and as install dir ./libgdcm (that's why these paths are in gitignore)
4) build gdcm, run build_all and install
5) use cmake for the hello-gdcm project, i use ./build as binary destination. (that's why this path is in gitignore)

Licence: simplified BSD license, approved by the Open Source Initiative (OSI)
* http://www.opensource.org/licenses/bsd-license.php

## Hello
Hello exists to test and learn how to use GDCM before incorporating the functionalities into the dll. It's easier to test and debug and exe then a library, specially if the consumer of the library is something like Unity.

## Tomographer

The dll is the tomographer project. It generates a shared library. The idea is that all gdcm madness be hidden behind the dll's public interface and that gdcm types do not leak to the project that consumes the dll.

The dll public interface is defined in hello-gdcm.h