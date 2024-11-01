#include "dicomDirectory.h"
#include "ScanError.h"
const gdcm::Directory GetDirectory(const char* directoryPath)
{
    gdcm::Directory d;
    d.Load( directoryPath, true); // recursive !
    return d;
}
void dicomDirectoryTests::CanGetTheDirectory()
{
    const char* test_dir = "/home/luciano/.config/unity3d/DefaultCompany/unity-test-gdcm";
    gdcm::Directory dir = GetDirectory(test_dir);
    assert(dir.GetFilenames().size() == 2139);
    assert(dir.GetToplevel().compare("/home/luciano/.config/unity3d/DefaultCompany/unity-test-gdcm") == 0);
    std::cout<<__FUNCTION__<<" passed"<<std::endl;
}    
const gdcm::Directory::FilenamesType GetFilenames(gdcm::Directory &d)
{
    const gdcm::Directory::FilenamesType &l1 = d.GetFilenames();
    return l1;
}
void dicomDirectoryTests::CanGetTheFilenames()
{
    const char* test_dir = "/home/luciano/.config/unity3d/DefaultCompany/unity-test-gdcm";
    gdcm::Directory directory = GetDirectory(test_dir);
    gdcm::Directory::FilenamesType filenames = directory.GetFilenames();
    auto x = std::find(filenames.begin(), filenames.end(), "/home/luciano/.config/unity3d/DefaultCompany/unity-test-gdcm/vhm.144.dcm");
    bool hasOneOfTheFilesExpected =  x != std::end(filenames);
    assert(hasOneOfTheFilesExpected==true);
    std::cout<<__FUNCTION__<<" passed"<<std::endl;
}

//FIXME: Unit test it
std::shared_ptr<gdcm::Scanner> PrepareFileScanner()
{
    std::shared_ptr<gdcm::Scanner> s0 = std::make_shared<gdcm::Scanner>();
    s0->AddTag( StudyInstanceUID );
    s0->AddTag( SeriesInstanceUID );
    s0->AddTag( PatientName );
    s0->AddTag( DirectionCosines);
    s0->AddTag( ImagePosition);
    return s0;
}
//FIXME: Unit test it
gdcm::Directory::FilenamesType ScanFilesOrRaiseError(std::shared_ptr<gdcm::Scanner> scanner, const gdcm::Directory& directory)
{
    auto fs = directory.GetFilenames();
    bool b = scanner->Scan(fs);
    if(!b)
    {
        throw ScanError();
    }
    else
    {
        return scanner->GetKeys();
    }
}
