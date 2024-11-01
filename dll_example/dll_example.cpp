#include "dll_example.h"
#include "hello-gdcm.h"

int main(int argc, char** argv)
{
    InitializeFunctions();
    tomographer::BeginLoadingImages("C:\\Users\\lucia\\OneDrive\\Documents\\dicoms\\Marching Man");
    return 0;
}