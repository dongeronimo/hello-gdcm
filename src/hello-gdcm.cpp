#include <iostream>
#include <sstream>
#include <algorithm>
#include <exception>
#include <sstream>
#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmAttribute.h"
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"

#include "dicomSorters.h"
#include "hello-gdcm.h"
#include "DicomImage.h"
#include "imageInformationList.h"
#include "dicomDirectory.h"
#include "dllContext.h"
#include "utils.h"
#include "testHGDCM_GetNumberOfImagesInDirectory.h"
#include <memory>
#include <optional>

const char* test_dir = "/home/luciano/.config/unity3d/DefaultCompany/unity-test-gdcm";
//Unittests


unsigned int HGDCM_GetNumberOfImagesInDirectory(const char* filepath)
{
    unsigned int number = AdaptGetNumberOfImages(filepath);
    return number;
}

ImageInformationLibraryOutput** HGDCM_GetImageInformationList(const char* filepath)
{
    std::vector<ImageInformation> imageInfoList = CreateImageInformationListFromDirectoryContents(filepath);
    ImageInformationLibraryOutput** adaptedList = AdaptImageInformationListForOutput(imageInfoList);
    return adaptedList;
}
/*struct Serie{
    std::string id;
    std::vector<std::string> list;
};
struct Study{
    std::string id;
    std::vector<Serie> list;
};
/////////////////////////////////////////
void GetPatients(const char* path){
    //1)Carregar as imagens do diretório e ordená-las.
    gdcm::Directory directory = GetDirectory(path);
    gdcm::Scanner fileScanner = PrepareFileScanner();
    gdcm::Directory::FilenamesType files = ScanFilesOrRaiseError(fileScanner, directory);
    gdcm::Directory::FilenamesType sortedFiles = SortFilenames(files);
    //3)Agrupar as imagens por serie
    std::vector<Serie> series;
    Serie currentSerie = {"n/d", std::vector<std::string>()};
    std::for_each(sortedFiles.begin(), sortedFiles.end(), 
        [&series, &currentSerie](gdcm::Directory::FilenameType &currentFile){
            //Lê o arquivo atual
            gdcm::Reader reader;
            reader.SetFileName(currentFile.c_str());
            reader.Read();
            gdcm::File file = reader.GetFile();
            //Pega o id da serie
            gdcm::DataSet &dataset = file.GetDataSet();
            gdcm::Attribute<0x0020, 0x000e> seriesUidAttr;
            seriesUidAttr.Set(dataset);
            std::string seriesUid = seriesUidAttr.GetValue();
            //verifica se o id do arquivo é igual ao id da serieAtual
            if(currentSerie.id == "n/d"){
                currentSerie =  {seriesUid, std::vector<std::string>()};
            }
            if(seriesUid != currentSerie.id){
                series.push_back(currentSerie);//guarda a serie
                currentSerie =  {seriesUid, std::vector<std::string>()}; //cria uma nova serie
            }
            currentSerie.list.push_back(seriesUid);
        }
    );
    series.push_back(currentSerie);
    //4)Agrupar as series por estudo
    //5)Agrupar os estudos por paciente
}*/

class ImageDescription {
private:
    std::string patient;
    std::string study;
    std::string series;
    std::string filepath;
public:
    ImageDescription(std::string _patient, std::string _study, std::string _series, std::string _filepath):patient(_patient),study(_study), series(_series), filepath(_filepath)
    {}
    ImageDescription(){}
    ImageDescription(const ImageDescription& other){
        this->filepath = other.filepath;
        this->study = other.study;
        this->series = other.series;
        this->patient = other.patient;
    }
    ImageDescriptionStruct* AllocateImageDescriptionStruct(){
        ImageDescriptionStruct* ids = new ImageDescriptionStruct();
        ids->patient = Utils::StringToCopiedCstr(this->patient);
        ids->study = Utils::StringToCopiedCstr(this->study);
        ids->series = Utils::StringToCopiedCstr(this->series);
        ids->filepath = Utils::StringToCopiedCstr(this->series);
        return ids;
    }
    void CopyDataToStruct(ImageDescriptionStruct* ids){
        ids->patient = Utils::StringToCopiedCstr(this->patient);
        ids->study = Utils::StringToCopiedCstr(this->study);
        ids->series = Utils::StringToCopiedCstr(this->series);
        ids->filepath = Utils::StringToCopiedCstr(this->filepath);
    }
};
std::optional<std::vector<ImageDescription>> gImageDescriptionList;
/// <summary>
/// This is the 1st function that users of the librry call. It reads the list of images
/// in the given directory. The result is stored in gImageDescriptionList.
/// </summary>
/// <param name="dir"></param>
extern "C" __declspec(dllexport) void BeginLoadingImages(const char* dir) {
    //1)Carregar as imagens do diretório e ordená-las.
    gdcm::Directory directory = GetDirectory(dir);
    std::shared_ptr<gdcm::Scanner> fileScanner = PrepareFileScanner();
    gdcm::Directory::FilenamesType files = ScanFilesOrRaiseError(fileScanner, directory);
    gdcm::Directory::FilenamesType sortedFiles = SortFilenames(files);
    //Assemble dicom descriptions
    std::vector<ImageDescription> imageDescriptions;
    imageDescriptions.resize(sortedFiles.size());
    std::transform(sortedFiles.begin(), sortedFiles.end(), imageDescriptions.begin(),
        [](gdcm::Directory::FilenameType &currentFile){
            //Lê o arquivo atual
            gdcm::Reader reader;
            reader.SetFileName(currentFile.c_str());
            reader.Read();
            gdcm::File file = reader.GetFile();
            //Pega os dados da series.
            gdcm::DataSet &dataset = file.GetDataSet();
            gdcm::Attribute<0x0020, 0x000e> seriesUidAttr;
            seriesUidAttr.Set(dataset);
            std::string seriesUid = seriesUidAttr.GetValue();
            gdcm::Attribute<0x0020, 0x000d> studyUidAttr;
            studyUidAttr.Set(dataset);
            std::string studyUid = studyUidAttr.GetValue();
            gdcm::Attribute<0x0010, 0x0010> patientAttr;
            patientAttr.Set(dataset);
            std::string name = patientAttr.GetValue();
            ImageDescription desc = ImageDescription(name, studyUid, seriesUid, currentFile);
            return desc;
        });
    gImageDescriptionList = imageDescriptions;
}
//Essa é a que retorna o numero de imagens no diretório.
int GetNumberOfImagesInDirectory(){
    assert(gImageDescriptionList.has_value());
    return (*gImageDescriptionList).size();
}
//Essa é a que retorna a tabela de descrições de imagens
void GetImageDescriptionList(ImageDescriptionStruct* out_description){
    assert(gImageDescriptionList.has_value());
    int count = 0;
    std::for_each((*gImageDescriptionList).begin(), 
        (*gImageDescriptionList).end(),
        [&count, &out_description](ImageDescription& currentDesc){
        currentDesc.CopyDataToStruct(&out_description[count]);
        count++;
    });
}
//Main entry point
int main(int argc, char** argv) {
    BeginLoadingImages(test_dir);
    int numberOfImages = GetNumberOfImagesInDirectory();
    ImageDescriptionStruct* desc = new ImageDescriptionStruct[numberOfImages];
    GetImageDescriptionList(desc);
    for(auto i=0; i<numberOfImages; i++){
        std::cout<<desc[i].filepath<<std::endl;
    }
    /*GetPatients(test_dir);*/
    /*
    std::cout<<"Running tests..."<<std::endl;
    test_HGDCM_GetNumberOfImagesInDirectory::NumberOfImagesIsCorrect();
    test_HGDCM_GetNumberOfImagesInDirectory::AdapterIsNotCrashing();
    
    Loren* lorens = new Loren[10];
    std::cout<<"size of 10 lorens = "<<sizeof(Loren)*10<<std::endl;
    woo(10, lorens);
    for(auto i=0; i<10; i++){
        std::cout<<lorens[i].id<<", "<< lorens[i].field<<std::endl;
    }
    */
    return 0;
}

int foo() {
    return 1;
}
int bar(const char* c) {
    return std::strlen(c);
}
std::unique_ptr<DllContext> dllContext;
//Initializes the context. Must be called once and must be called before all other functions.
void Initialize(const char* imageDirectory){
    dllContext = std::unique_ptr<DllContext>(new DllContext(imageDirectory));
}
void LoadImages(OnProgressCallback onProgressCallback){
    dllContext->LoadImages(onProgressCallback);
}
void woo(int sz, Loren* outValue){
    dllContext->woo(sz, outValue);
    //for(auto i=0; i<sz; i++){
    //    outValue[i].id = i;
    //    outValue[i].field = "lol";
    //}
}
void foobar(int sz, Loren** outValue){
    for(auto i=0; i<sz; i++){
        std::stringstream ss;
        ss<<"loren = "<<i;
        outValue[i]->field = Utils::StringstreamToCstr(ss);
        outValue[i]->id = i;
    }
}
