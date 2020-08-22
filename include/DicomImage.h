#ifndef __dicom_image_h
#define __dicom_image_h
#include "gdcmScanner.h"
class DicomImage 
{
private:
    gdcm::Directory::FilenamesType sorted_files;
    double pixelSpacingX;
    double pixelSpacingY;
    double sliceThickness;
    unsigned int imageRows;
    unsigned int imageColumns;
public:
    DicomImage(const char* directory);
    unsigned int NumberOfFiles(){return this->sorted_files.size();}
    double PixelSpacingX(){return this->pixelSpacingX;}
    double PixelSpacingY(){return this->pixelSpacingY;}
    double SliceThickness(){return this->sliceThickness;}
    unsigned int ImageRows(){return this->imageRows;}
    unsigned int ImageColumns(){return this->imageColumns;}
};
#endif
