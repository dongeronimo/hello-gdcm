cmake_minimum_required(VERSION 3.4.1)
project (hello-gdcm)
set(GDCM_DIR "${CMAKE_SOURCE_DIR}/libgdcm/lib/gdcm-3.1")
#set(CMAKE_POSITION_INDEPENDENT_CODE ON) #Pra poder compilar com -fPIC

##   GDCM must not exist in the system libraries for me to be able to set the GDCM_PATH.
## If GDCM_PATH is set to where my custom GDCM is find_package will work.
##   For example if I did $ make -j8 DESTDIR=~/Documents/gdcm/ubuntu-x64/result install.
## GDCM_DIR should be GDCM_DIR=/home/luciano/Documents/gdcm/ubuntu-x64/result/usr/local/lib/gdcm-3.0
find_package(GDCM REQUIRED)
include_directories(include)
file(GLOB EXE_INCLUDES "include/*.h")
file(GLOB EXE_SOURCES "src/*.cpp")
add_executable(hello ${EXE_SOURCES} ${EXE_SOURCES})

include_directories(hello PRIVATE ${GDCM_INCLUDE_DIRS})
target_link_libraries(hello PRIVATE
    gdcmcharls
    gdcmCommon
    gdcmDICT
    gdcmDSED
    gdcmexpat
    gdcmIOD
    gdcmjpeg12
    gdcmjpeg16
    gdcmjpeg8
    gdcmMEXD
    gdcmMSFF
    gdcmopenjp2
    gdcmzlib
    socketxx
)

add_library(tomographer SHARED ${EXE_INCLUDES} ${EXE_SOURCES})
include_directories(tomographer PRIVATE ${GDCM_INCLUDE_DIRS})
target_link_libraries(tomographer PRIVATE
    gdcmcharls
    gdcmCommon
    gdcmDICT
    gdcmDSED
    gdcmexpat
    gdcmIOD
    gdcmjpeg12
    gdcmjpeg16
    gdcmjpeg8
    gdcmMEXD
    gdcmMSFF
    gdcmopenjp2
    gdcmzlib
    socketxx
)
target_compile_features(tomographer PRIVATE cxx_std_17)

add_executable(dll_example 
    include/hello-gdcm.h
    dll_example/dll_example.cpp
    dll_example/dll_example.h
)
#### PRINT ALL VARIABLES ####
#message(STATUS "print_all_variables------------------------------------------{")
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#message(STATUS "print_all_variables------------------------------------------}")
##### TO GUARANTEE THAT THERE WILL BE A DEBUG BUILD SO THAT KDEVELOP'S DEBUGGER WORKS ####
## Set a default build type if none was specified
#if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#  message(STATUS "Setting build type to 'Debug' as none was specified.")
#  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
#  # Set the possible values of build type for cmake-gui
#  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
#    "MinSizeRel" "RelWithDebInfo")
#endif()
#ADD_DEFINITIONS(-fPIC)

