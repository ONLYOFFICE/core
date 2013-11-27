#pragma once

#define CREATE_UNZIPPED_XMLS
//#define CREATE_ZIPPED_DOCX
#undef DUMP_MEMORY_LEAKS

#ifdef CREATE_ZIPPED_DOCX

#undef USE_ZLIB_DLL
#define ZLIB_WINAPI

#ifdef USE_ZLIB_DLL 
#pragma comment(lib, "zlibwapi.lib")
#else
#pragma comment(lib, "zlibstat.lib")
#endif //USE_ZLIB_DLL

#include "zip.h"

#else


#import "..\..\Redist\ASCOfficeUtils.dll" raw_interfaces_only

//#endif // _DEBUG

#define DOC_FILE_COMPONENT_NAME L"ASCOfficeDocFile.dll"
#define DOC_FILE_TEMPLATE_FILE_NAME L"Template.doc"

using namespace ASCOfficeUtils;

// Disable warning message 4267.
#pragma warning( disable : 4267 )

#endif //CREATE_ZIPPED_DOCX
