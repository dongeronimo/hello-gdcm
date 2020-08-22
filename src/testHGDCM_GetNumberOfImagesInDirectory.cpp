#include "testHGDCM_GetNumberOfImagesInDirectory.h"
#include "imageInformationList.h"
#include <iostream>
#include <sstream>
#include <assert.h>
const std::string path = "/home/luciano/Downloads/Marching Man";
void test_HGDCM_GetNumberOfImagesInDirectory::NumberOfImagesIsCorrect()
{
    auto qtd = GetNumberOfImages(path);
    assert(qtd==94);
    std::cout<<__FUNCTION__<<" passed"<<std::endl;
}
void test_HGDCM_GetNumberOfImagesInDirectory::AdapterIsNotCrashing()
{
    const char* pathAsCstr = "/home/luciano/Downloads/Marching Man";
    auto qtd = AdaptGetNumberOfImages(pathAsCstr);
    assert(qtd==94);
    std::cout<<__FUNCTION__<<" passed"<<std::endl;
}
