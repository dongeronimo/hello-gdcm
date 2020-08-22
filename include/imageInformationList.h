#ifndef __image_information_list_h
#define __image_information_list_h
#include <vector>
#include <string>
#include <array>
#include <exception>
#include <sstream>


struct ImageInformation {
    std::string filename;
    std::string patientName;
    std::string study;
    std::string series;
    std::array<double, 6> directionCosines;
    std::array<double, 3> imagePosition;
};

struct ImageInformationLibraryOutput {
    const char* filename;
    const char* patientName;
    const char* study;
    const char* series;
    double directionCosineX0,directionCosineY0,directionCosineZ0;
    double directionCosineX1,directionCosineY1,directionCosineZ1;
    double imagePositionX, imagePositionY, imagePositionZ;
};

class CouldNotReadFileException:public std::exception {
    private:
        std::stringstream message;
    public:
        CouldNotReadFileException(const char* badFilePath){
            message<<"Could not read file "<<badFilePath;
        }
        std::string Message(){
            return message.str();
        }
};
unsigned int GetNumberOfImages(std::string path);
unsigned int AdaptGetNumberOfImages(const char* path);
std::vector<ImageInformation> CreateImageInformationListFromDirectoryContents(const char* dirpath);
ImageInformationLibraryOutput** AdaptImageInformationListForOutput(std::vector<ImageInformation> &info);

#endif
