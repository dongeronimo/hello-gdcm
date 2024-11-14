#include "dll_example.h"
#include <vector>
int main(int argc, char** argv)
{
    InitializeFunctions();
    //load the list of images into the dll
    tomographer::LoadDicomImageResult dicomImageAttrs = tomographer::LoadDicomImage("C:\\Users\\lucia\\OneDrive\\Documents\\dicoms\\Marching Man");
    for (auto i = 0; i < dicomImageAttrs.cols * dicomImageAttrs.cols * dicomImageAttrs.numberOfFiles; i++) {
        printf("%d ", dicomImageAttrs.data[i]);
    }
    return 0;
}