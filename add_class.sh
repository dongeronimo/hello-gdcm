cd include
touch $1.h
cd ..
cd src
touch $1.cpp
cd ..
cd build/ubuntu-x64
cmake .
