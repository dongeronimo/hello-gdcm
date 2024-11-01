#ifndef __dicom_directory_h
#define __dicom_directory_h

#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmAttribute.h"
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"
#include "gdcmImageReader.h"
#include <memory>
const gdcm::Tag StudyInstanceUID(0x0020,0x000d); // Study Instance UID
const gdcm::Tag SeriesInstanceUID(0x0020,0x000e); // Series Instance UID
const gdcm::Tag PatientName(0x0010,0x0010); // Patient's Name
const gdcm::Tag DirectionCosines(0x0020,0x0037); // Direction Cosines
const gdcm::Tag ImagePosition(0x0020,0x0032); // Image Position

//Returns the object that represents the directory that may have dicom images.
//Param: directoryPath: the cstr with the path.
const gdcm::Directory GetDirectory(const char* directoryPath);
//Gets the filenames that exist inside the given Directory. They are unsorted and non-dicom files arent filtered.
const gdcm::Directory::FilenamesType GetFilenames(gdcm::Directory &d);
//Creates a file scanner aware of the Study Instance, Series Instance and Patient Name tags.
std::shared_ptr<gdcm::Scanner> PrepareFileScanner();
//Scans the given directory with the given scanner. Returns the files that the scanner scans, removing non-dicom files from the file list.
gdcm::Directory::FilenamesType ScanFilesOrRaiseError(std::shared_ptr<gdcm::Scanner> scanner, const gdcm::Directory& directory);
//Unittests

namespace dicomDirectoryTests
{
    void CanGetTheDirectory();
    void CanGetTheFilenames();
}    
#endif
