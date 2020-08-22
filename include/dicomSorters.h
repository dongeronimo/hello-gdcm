#ifndef __dicomSorters_h
#define __dicomSorters_h
#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmAttribute.h"
#include "hello-gdcm.h"
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"
/*Sort by patient id (tag 0010:0010).*/
bool SortByPatient(gdcm::DataSet const & ds1, gdcm::DataSet const &ds2);
/*Sort by study (0020:000d)*/
bool SortByStudyInstanceUIDDicomTag(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 );
/*Sort by series instance uid. A study can have many series. Tag 0020:000e*/
bool SortBySeriesUIDDicomTag(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 );
/*Sort by direction cosines (tag 0020:0037)*/
bool SortByDirectionCosines(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 );
/*Sort by image position and orientation (tags 0020:0032 and 0020:0037)*/
bool SortByImagePositionAndOrientation(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 );

gdcm::Directory::FilenamesType SortFilenames(gdcm::Directory::FilenamesType files);
#endif
