#ifndef __hello_gdcm_h
#define __hello_gdcm_h

//#include "gdcmReader.h"
//#include "gdcmWriter.h"
//#include "gdcmAttribute.h"
//#include "hello-gdcm.h"
//#include "gdcmSorter.h"
//#include "gdcmIPPSorter.h"
//#include "gdcmScanner.h"
//#include "gdcmDataSet.h"
//#include "gdcmAttribute.h"

//#include "imageInformationList.h"
//#include "dllContext.h"
struct ImageDescriptionStruct{
    const char* patient;
    const char* study;
    const char* series;
    const char* filepath;
};
//extern "C" {
//    //Essa é a fn que deve ser chamada primeiro, para inicializar a lista de imagens do diretório.
//    void BeginLoadingImages(const char* dir);
//    //Essa é a que retorna o numero de imagens no diretório.
//    int GetNumberOfImagesInDirectory();
//    //Essa é a que retorna a tabela de descrições de imagens
//    void GetImageDescriptionList(ImageDescriptionStruct* out_description);
//    //int foo();
//    //int bar(const char* c);
//    //void Initialize(const char* imageDirectory);
//    //void LoadImages(OnProgressCallback onProgressCallback);
//    //void woo(int sz, Loren* outValue);
//    //void foobar(int sz, Loren** outValue);
//    /*Reads the directory and return the number of images in it.*/
//    //unsigned int HGDCM_GetNumberOfImagesInDirectory(const char* filepath);
//    /*Returns an array of ImageInformationLibraryOutput pointers. It's lenght is the value returned by HGDCM_GetNumberOfImagesInDirectory*/
//    //ImageInformationLibraryOutput** HGDCM_GetImageInformationList(const char* filepath);
//    /**Returns HELLO_GDCM_SUCCESS if the file can be read, ERROR_COULD_NOT_READ if not.*/
//    //unsigned int HGDCMCanRead(const char* filepath);
//}
//void TestHGDCMCanReadReturnsErrorIfInvalidPath();
//void TestHGDCMCanReadReturnsSuccessIfValidPath();
#endif
/**
 * Epic:
 As library client I want to volume render dicom images.
 
 */

/**
 TODO: Criar a entidade da fatia
 TODO: Enviar a fatia pra unity
 TODO: Enxergar a fatia
 TODO: Carregar varias fatias
 TODO: Combinas as fatias em uma unica textura 3d na Unity
 TODO: Exibir a textura 3d
 TODO: Volume renderer
 */
