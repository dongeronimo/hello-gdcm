#pragma once
#include <Windows.h>
#include <cassert>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
//void BeginLoadingImages(const char* dir);
typedef void (*_begin_loading_images_t)(const char* directory);

namespace tomographer
{
    static _begin_loading_images_t BeginLoadingImages = nullptr;
}
void InitializeFunctions() {
    HMODULE m = LoadLibrary("C:\\hello_gdcm\\hello-gdcm\\build\\Debug\\tomographer.dll");
    assert(m != 0);
    auto result = tomographer::BeginLoadingImages = (_begin_loading_images_t)GetProcAddress(m, "BeginLoadingImages");
    assert(tomographer::BeginLoadingImages != nullptr);
}