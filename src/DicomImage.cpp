#include "DicomImage.h"
#include <sstream>
#include <utility>
#include <tuple>

#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmAttribute.h"
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"
#include "gdcmImageReader.h"

#include "dicomDirectory.h"
#include "ScanError.h"
#include "dicomSorters.h"
//FIXME: Move it to it`s own file
//const gdcm::Directory GetDirectory(const char* directoryPath);
//const gdcm::Directory::FilenamesType GetFilenames(gdcm::Directory &d);
//gdcm::Scanner PrepareFileScanner();
//gdcm::Directory::FilenamesType ScanFilesOrRaiseError(gdcm::Scanner& scanner, gdcm::Directory& directory);
const gdcm::File OpenFirstImage(gdcm::Directory::FilenamesType sorted_files);
std::pair<double, double> GetPixelSpacing(gdcm::DataSet &dataset);
double GetSliceThickness(gdcm::DataSet &dataset);
unsigned int GetNumberOfRows(const gdcm::DataSet &dataset);
unsigned int GetNumberOfColumns(const gdcm::DataSet &dataset);
//FIXME: Unit test it
DicomImage::DicomImage(const char* directoryPath)
{
    gdcm::Directory d = GetDirectory(directoryPath);
    gdcm::Directory::FilenamesType l1 = GetFilenames(d);
    std::shared_ptr<gdcm::Scanner> s0 = PrepareFileScanner();
    gdcm::Directory::FilenamesType l2 = ScanFilesOrRaiseError(s0, d);
    this->sorted_files = SortFilenames(l2);
    //////Reads the first file in the series to get pixel spacing, slice thickness and number of pixels
    gdcm::File first_image = OpenFirstImage(this->sorted_files);
    gdcm::DataSet &dataset = first_image.GetDataSet();
    const std::pair<double, double> pixelSpacing = GetPixelSpacing(dataset);
    this->pixelSpacingX = pixelSpacing.first;
    this->pixelSpacingY = pixelSpacing.second;
    this->sliceThickness = GetSliceThickness(dataset);
    this->imageRows = GetNumberOfRows(dataset);
    this->imageColumns = GetNumberOfColumns(dataset);

    int8_t* tempBuffer = new int8_t[imageRows * imageColumns * sorted_files.size() * sizeof(short)];
    
    for (auto i = 0; i < sorted_files.size(); i++) {
        std::string currentFile = sorted_files.at(i);
        gdcm::ImageReader imgReader;
        imgReader.SetFileName(currentFile.c_str());
        imgReader.Read();
        const gdcm::Image& currentImage = imgReader.GetImage();
        const gdcm::PixelFormat& format = currentImage.GetPixelFormat();
        assert(format == gdcm::PixelFormat::INT16);//can only deal with short for now.
        uint64_t size = currentImage.GetBufferLength();
        std::vector<char> currentImageBuffer(size * sizeof(short));
        currentImage.GetBuffer(currentImageBuffer.data());
        
        memcpy(tempBuffer + i * size, currentImageBuffer.data(), size);
    }
    this->imageData.resize(imageRows * imageRows * sorted_files.size());
    char* destination = reinterpret_cast<char*>(imageData.data());
    memcpy(destination, tempBuffer, imageRows * imageColumns * sorted_files.size() * sizeof(short));
    delete tempBuffer;
}
unsigned int GetNumberOfRows(const gdcm::DataSet &dataset)
{
    gdcm::Attribute<0x0028, 0x0010> rowsAttr;
    rowsAttr.Set(dataset);
    return rowsAttr.GetValue();
}
unsigned int GetNumberOfColumns(const gdcm::DataSet &dataset)
{
    gdcm::Attribute<0x0028, 0x0011> colsAttr;
    colsAttr.Set(dataset);
    return colsAttr.GetValue();

}
double GetSliceThickness(gdcm::DataSet &dataset)
{
    gdcm::Attribute<0x0018, 0x0050> sliceThicknessAttr;
    sliceThicknessAttr.Set(dataset);
    double thickness = sliceThicknessAttr.GetValue();
    return thickness;
}
std::pair<double, double> GetPixelSpacing(gdcm::DataSet &dataset)
{
    gdcm::Attribute<0x0028, 0x0030> pixelSpacingAttr;
    pixelSpacingAttr.Set(dataset);
    double x = pixelSpacingAttr[0];
    double y = pixelSpacingAttr[1];
    return std::make_pair(x,y);
}
const gdcm::File OpenFirstImage(gdcm::Directory::FilenamesType sorted_files)
{
    gdcm::Reader reader;
    std::string first_file = *sorted_files.begin();
    reader.SetFileName(first_file.c_str());
    if(!reader.Read())
    {
        //FIXME Must throw an exception
    }
    gdcm::File &file = reader.GetFile();
    return file;
}


