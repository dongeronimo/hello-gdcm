#include "dicomSorters.h"
bool SortByPatient(gdcm::DataSet const & ds1, gdcm::DataSet const &ds2)
{
    gdcm::Attribute<0x0010, 0x0010> PatientId1;
    PatientId1.Set(ds1);
    gdcm::Attribute<0x0010, 0x0010> PatientId2;
    PatientId2.Set(ds2);
    return PatientId1 < PatientId2;
}
/*Sort by the study instance uid. A patient can have many studies*/
bool SortByStudyInstanceUIDDicomTag(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 )
{
    gdcm::Attribute<0x0020,0x000d> StudyInstanceUID1;
    StudyInstanceUID1.Set( ds1 );
    gdcm::Attribute<0x0020,0x000d> StudyInstanceUID2;
    StudyInstanceUID2.Set( ds2 );
    return StudyInstanceUID1 < StudyInstanceUID2;
}
/*Sort by series instance uid. A study can have many series*/
bool SortBySeriesUIDDicomTag(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 )
{
    gdcm::Attribute<0x0020,0x000e> at1;
    at1.Set( ds1 );
    gdcm::Attribute<0x0020,0x000e> at2;
    at2.Set( ds2 );
    return at1 < at2;
}
bool DatasetHasDirectionCosines(const gdcm::DataSet &ds)
{
    gdcm::Tag x(0x0020,0x0037);
    bool ds1HasDirectionCosines = ds.FindDataElement(x);
    return ds1HasDirectionCosines;
}
bool BothDatasetsHaveDirectionCosines(const gdcm::DataSet &ds1, const gdcm::DataSet &ds2)
{
    return DatasetHasDirectionCosines(ds1) && DatasetHasDirectionCosines(ds2);
}
bool SortByDirectionCosines(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 )
{
    if(!BothDatasetsHaveDirectionCosines(ds1, ds2))
    {
        return false;
    }
    else
    {
        // This is a floating point number is the comparison ok ?
        gdcm::Attribute<0x0020,0x0037> at1;
        at1.Set( ds1 );
        gdcm::Attribute<0x0020,0x0037> at2;
        at2.Set( ds2 );
        return at1 < at2;
    }   
}

bool SortByImagePositionAndOrientation(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2 )
{
    // Do the IPP sorting here
    gdcm::Attribute<0x0020,0x0032> ipp1;//image position 
    gdcm::Attribute<0x0020,0x0037> image1OrientationPlane;//direction cosines 
    ipp1.Set( ds1 );
    image1OrientationPlane.Set( ds1 );
    gdcm::Attribute<0x0020,0x0032> ipp2;
    gdcm::Attribute<0x0020,0x0037> image2OrientationPlane;
    ipp2.Set( ds2 );
    image2OrientationPlane.Set( ds2 );
    if( image1OrientationPlane != image2OrientationPlane )
    {
        return false;
    }
    // else
    double normal[3];
    normal[0] = image1OrientationPlane[1]*image1OrientationPlane[5] - image1OrientationPlane[2]*image1OrientationPlane[4];
    normal[1] = image1OrientationPlane[2]*image1OrientationPlane[3] - image1OrientationPlane[0]*image1OrientationPlane[5];
    normal[2] = image1OrientationPlane[0]*image1OrientationPlane[4] - image1OrientationPlane[1]*image1OrientationPlane[3];
    double dist1 = 0;
    for (int i = 0; i < 3; ++i) dist1 += normal[i]*ipp1[i];
    double dist2 = 0;
    for (int i = 0; i < 3; ++i) dist2 += normal[i]*ipp2[i];
    return dist1 < dist2;
}

gdcm::Directory::FilenamesType SortFilenames(gdcm::Directory::FilenamesType files)
{
    gdcm::Sorter sorter;
    sorter.SetSortFunction(SortByPatient);
    sorter.StableSort(files);
    
    sorter.SetSortFunction( SortByStudyInstanceUIDDicomTag );
    sorter.StableSort( sorter.GetFilenames());
    
    sorter.SetSortFunction( SortBySeriesUIDDicomTag );
    sorter.StableSort( sorter.GetFilenames() );
    
    sorter.SetSortFunction( SortByImagePositionAndOrientation );
    sorter.StableSort( sorter.GetFilenames() );
    
    return sorter.GetFilenames();
}    


