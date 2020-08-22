#ifndef __dll_context_h
#define __dll_context_h
#include <string>
/*The dll context. The dll is stateful, accumulating state changes.*/
struct Loren{
    int id;
    const char* field;
};

typedef void (*OnProgressCallback)(double);
class DllContext{
private:
    int lorenCounter;
    std::string dirPath;
public:
    DllContext(const char* imageDirectory);
    void LoadImages(OnProgressCallback progressCallback);
    void woo(int sz, Loren* outValue);    
};
#endif
