#ifndef __ScanError_h
#define __ScanError_h
#include <exception>
struct ScanError : public std::exception 
{
	const char * what () const throw ()
    {
    	return "could not scan directory";
    }
};
#endif
