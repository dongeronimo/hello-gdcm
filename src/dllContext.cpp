#include "dllContext.h"
#include <sstream>
#include <algorithm>
#include "gdcmAttribute.h"
#include "utils.h"
#include "dicomDirectory.h"
#include "dicomSorters.h"
DllContext::DllContext(const char* imageDirectory)
    :lorenCounter(0),dirPath(imageDirectory)
{
}

void DllContext::LoadImages(OnProgressCallback progressCallback)
{
    //Read the directory
    gdcm::Directory directory = GetDirectory(dirPath.c_str());
    std::shared_ptr<gdcm::Scanner> fileScanner = PrepareFileScanner();
    gdcm::Directory::FilenamesType files = ScanFilesOrRaiseError(fileScanner, directory);
    auto sortedFiles = SortFilenames(files);
    const int numberOfFiles = sortedFiles.size();
    int currentFileCount = 0;
    //Load the images
    std::vector<gdcm::Image> images;
    images.resize(sortedFiles.size());
    
    std::for_each(sortedFiles.begin(), sortedFiles.end(), 
        [numberOfFiles, &currentFileCount, &progressCallback](gdcm::Directory::FilenameType &currentFilePath){
            //Carregar a imagem
            gdcm::Reader reader;
            reader.SetFileName(currentFilePath.c_str());
            gdcm::File file = reader.GetFile();
            gdcm::DataSet &dataset = file.GetDataSet();
            //const gdcm::Tag SeriesInstanceUID(0x0020,0x000e); // Series Instance UID
            //Pegar o código da série
            gdcm::Attribute<0x0020, 0x000e> seriesUidAttr;
            seriesUidAttr.Set(dataset);
            std::string seriesUid = seriesUidAttr.GetValue();
            //TODO: Po-la em uma tabela junto com as imagens de mesmo código de série
        }
    );
    
    std::transform(sortedFiles.begin(), sortedFiles.end(), images.begin(),
        [numberOfFiles, &currentFileCount, &progressCallback](gdcm::Directory::FilenameType &currentFilePath){
            //TODO: Para cada imagem verificar o código da série e guardar numa lista de imagens com o mesmo codigo de série
            gdcm::Reader reader;
            reader.SetFileName(currentFilePath.c_str());
            gdcm::File &file = reader.GetFile();
            
            gdcm::ImageReader imageReader;
            imageReader.SetFile(file);//SetFileName(currentFilePath.c_str());
            const gdcm::Image image = imageReader.GetImage();
            
            if (progressCallback!=NULL){
                progressCallback(double(currentFileCount)/double(numberOfFiles));
            }
            currentFileCount++;
            return image;
        });
    //TODO: Create the patient information

}


void DllContext::woo(int sz, Loren* outValue)
{
    for(auto i=0; i<sz; i++){
        outValue[i].id = i;
        std::stringstream ss;
        ss<<"lol counter = "<<lorenCounter;
        const char* field = Utils::StringstreamToCstr(ss);
        outValue[i].field = field;
    }
    lorenCounter++;
}
