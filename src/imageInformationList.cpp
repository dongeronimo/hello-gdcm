#include "imageInformationList.h"
#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmAttribute.h"
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"

#include "dicomSorters.h"
#include "dicomDirectory.h"
#include <algorithm>
#include <exception>
#include <sstream>
std::vector<ImageInformation> MapFromGdcmFilepathsToImageInformation(gdcm::Directory::FilenamesType &sortedFiles);
std::array<double, 3> GetImagePositionFromDataset(const gdcm::DataSet& dataset);
std::array<double, 6> GetDirectionCosineFromDataset(const gdcm::DataSet& dataset);
std::string GetStudyFromDataset(const gdcm::DataSet& dataset);
std::string GetSeriesFromDataset(const gdcm::DataSet& dataset);
std::string GetNameFromDataset(const gdcm::DataSet& dataset);
gdcm::Reader* CreateReader(const char* path);
ImageInformationLibraryOutput** AllocateListOfPointers(std::vector<ImageInformation> &info);
char* CloneNameToNewCstr(ImageInformation &current);
char* CloneFilepathToNewCstr(ImageInformation &current);
char* CloneStudyToNewCstr(ImageInformation &current);
char* CloneSeriesToNewCstr(ImageInformation &current);
double* CloneDirectionCosineToNewArray(ImageInformation &current);
double* CloneImagePositionToNewArray(ImageInformation &current);
ImageInformationLibraryOutput* AllocateNewImageInformationOutput(char* filepath, char* name,char* study,char* series,
        double* directionCosines,double* imagePosition);

unsigned int GetNumberOfImages(std::string path)
{
    gdcm::Directory directory = GetDirectory(path.c_str());
    std::shared_ptr<gdcm::Scanner> fileScanner = PrepareFileScanner();
    gdcm::Directory::FilenamesType files = ScanFilesOrRaiseError(fileScanner, directory);
    auto sortedFiles = SortFilenames(files);
    return sortedFiles.size();
}

unsigned int AdaptGetNumberOfImages(const char* path)
{
    std::string str(path);
    return GetNumberOfImages(str);
}
std::vector<ImageInformation> CreateImageInformationListFromDirectoryContents(const char* dirpath)
{
    gdcm::Directory directory = GetDirectory(dirpath);
    std::shared_ptr<gdcm::Scanner> fileScanner = PrepareFileScanner();
    gdcm::Directory::FilenamesType files = ScanFilesOrRaiseError(fileScanner, directory);
    auto sortedFiles = SortFilenames(files);
    std::vector<ImageInformation> imageInfoList = MapFromGdcmFilepathsToImageInformation(sortedFiles);
    return imageInfoList;
}

ImageInformationLibraryOutput** AdaptImageInformationListForOutput(std::vector<ImageInformation> &info)
{
    ImageInformationLibraryOutput** list = AllocateListOfPointers(info);
    for(auto idx=0; idx<info.size(); idx++)
    {
        auto current = info[idx];
        char* filepath = CloneFilepathToNewCstr(current);
        char* name = CloneNameToNewCstr(current);
        char* study = CloneStudyToNewCstr(current);
        char* series = CloneSeriesToNewCstr(current);
        double* directionCosines = CloneDirectionCosineToNewArray(current);
        double* imagePosition = CloneImagePositionToNewArray(current);
        ImageInformationLibraryOutput* newItem = AllocateNewImageInformationOutput(filepath, name, study, series, 
                                                                                   directionCosines,
                                                                                   imagePosition);
        list[idx] = newItem;
    }
    return list;
}

ImageInformationLibraryOutput* AllocateNewImageInformationOutput(char* filepath, char* name,char* study,char* series,double* directionCosines,double* imagePosition)
{
    ImageInformationLibraryOutput* newItem = new ImageInformationLibraryOutput;
    newItem->directionCosineX0 = directionCosines[0];
    newItem->directionCosineY0 = directionCosines[1];
    newItem->directionCosineZ0 = directionCosines[2];
    
    newItem->directionCosineX1 = directionCosines[3];
    newItem->directionCosineY1 = directionCosines[4];
    newItem->directionCosineZ1 = directionCosines[5];
    
    
    newItem->filename = filepath;
    newItem->imagePositionX = imagePosition[0];
    newItem->imagePositionY = imagePosition[1];
    newItem->imagePositionZ = imagePosition[2];
    newItem->patientName = name;
    newItem->series = series;
    newItem->study = study;
    return newItem;
}

double* CloneImagePositionToNewArray(ImageInformation &current)
{
    double* imagePosition = new double[current.imagePosition.size()];
    memcpy(imagePosition, current.imagePosition.data(), sizeof(double)*3);
    return imagePosition;
}

double* CloneDirectionCosineToNewArray(ImageInformation &current)
{
    double* directionCosines = new double[current.directionCosines.size()];
    memcpy(directionCosines, current.directionCosines.data(), sizeof(double)*6);
    return directionCosines;
}
char* CloneSeriesToNewCstr(ImageInformation &current)
{
    char* series = new char[current.series.size()];
    strcpy(series, current.series.c_str());
    return series;
}
char* CloneStudyToNewCstr(ImageInformation &current)
{
    char* study = new char[current.study.size()];
    strcpy(study, current.study.c_str());
    return study;
}
char* CloneFilepathToNewCstr(ImageInformation &current)
{
    char* filepath = new char[current.filename.size()];
    strcpy(filepath, current.filename.c_str());
    return filepath;
}
char* CloneNameToNewCstr(ImageInformation &current)
{
    char* name = new char[current.patientName.size()];
    strcpy(name, current.patientName.c_str());
    return name;
}
ImageInformationLibraryOutput** AllocateListOfPointers(std::vector<ImageInformation> &info)
{
    ImageInformationLibraryOutput** list = new ImageInformationLibraryOutput*[info.size()];
    return list;
}
std::string GetNameFromDataset(const gdcm::DataSet& dataset)
{
    gdcm::Attribute<0x0010, 0x0010> patientTag;
    patientTag.Set(dataset);
    std::string name = patientTag.GetValue();      //gdcm::string
    return name;
}
std::string GetSeriesFromDataset(const gdcm::DataSet& dataset)
{
    gdcm::Attribute<0x0020,0x000e> seriesTag;
    seriesTag.Set(dataset);
    std::string seriesUid = seriesTag.GetValue();      //gdcm::string
    return seriesUid;
}
std::string GetStudyFromDataset(const gdcm::DataSet& dataset)
{
    gdcm::Attribute<0x0020,0x000d> studyTag;
    studyTag.Set(dataset);
    std::string studyUid = studyTag.GetValue();      //gdcm::string
    return studyUid;
}
std::array<double, 6> GetDirectionCosineFromDataset(const gdcm::DataSet& dataset)
{
    gdcm::Attribute<0x0020,0x0037> directionCosinesTag;//direction cosines 
    directionCosinesTag.Set(dataset);
    std::array<double, 6> directionCosines = {directionCosinesTag[0],directionCosinesTag[1], directionCosinesTag[2],
        directionCosinesTag[3], directionCosinesTag[4], directionCosinesTag[5]};
    return directionCosines;
}
std::array<double, 3> GetImagePositionFromDataset(const gdcm::DataSet& dataset)
{
    gdcm::Attribute<0x0020,0x0032> imagePositionTag;//image position 
    imagePositionTag.Set(dataset);
    std::array<double, 3> imagePosition = {imagePositionTag[0], imagePositionTag[1], imagePositionTag[2]};
    return imagePosition;
}

std::vector<ImageInformation> MapFromGdcmFilepathsToImageInformation(gdcm::Directory::FilenamesType &sortedFiles)
{
    std::vector<ImageInformation> items;
    items.resize(sortedFiles.size());
    std::transform(sortedFiles.begin(), sortedFiles.end(), items.begin(),
        [](gdcm::Directory::FilenameType &filepath){
            gdcm::Reader* reader = CreateReader(filepath.c_str());
            gdcm::File &currentFile = reader->GetFile();
            gdcm::DataSet currentDataSet = currentFile.GetDataSet();
            std::string name = GetNameFromDataset(currentDataSet);  
            std::string seriesUid = GetSeriesFromDataset(currentDataSet);
            std::string studyUid = GetStudyFromDataset(currentDataSet); 
            std::array<double, 6> directionCosines = GetDirectionCosineFromDataset(currentDataSet);
            std::array<double, 3> imagePosition = GetImagePositionFromDataset(currentDataSet);
            const ImageInformation listItem = {filepath, name, studyUid, seriesUid, directionCosines, imagePosition};
            return listItem;
        }
    );
    return items;
}

gdcm::Reader* CreateReader(const char* path)
{
    gdcm::Reader* reader = new gdcm::Reader();
    reader->SetFileName(path);
    bool canRead = reader->Read();
    if (!canRead)
    {
        throw CouldNotReadFileException(path);
    }    
    return reader;
}
