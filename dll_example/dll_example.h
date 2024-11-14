#pragma once
#include <Windows.h>
#include <cassert>
#include <stdio.h>
#include <stdint.h>
#define WIN32_LEAN_AND_MEAN

/// <summary>
/// this namespace holds the function pointers for the dll functions.
/// They are loaded using InitializeFunctions.
/// </summary>
namespace tomographer
{
    struct LoadDicomImageResult {
        uint32_t numberOfFiles;
        float spacingX;
        float spacingY;
        float thickness;
        uint32_t rows;
        uint32_t cols;
        short* data;
    };
    typedef LoadDicomImageResult(*load_dicom_image_t)(const char* dir);

    load_dicom_image_t LoadDicomImage;
}
template <typename t>
t LoadFN(HMODULE _module, const char* name) {
    t ptr = (t)GetProcAddress(_module, name);
    assert(ptr != nullptr);
    return ptr;
}
void InitializeFunctions() {
    HMODULE m = LoadLibrary("C:\\hello_gdcm\\hello-gdcm\\build\\Debug\\tomographer.dll");
    assert(m != 0);
    tomographer::LoadDicomImage = LoadFN<tomographer::load_dicom_image_t>(m, "LoadDicomImage");
}