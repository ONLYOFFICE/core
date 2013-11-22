// AVSImageConverter.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{730DDDB5-0109-49E1-BF73-2B62D9FF25CE}", 
		 name = "AVSImageConverter", 
		 helpstring = "AVSImageConverter 1.0 Type Library",
		 resource_name = "IDR_AVSIMAGECONVERTER") ]
class CAVSImageConverterModule
{
public:
// Override CAtlDllModuleT members
};

#include "ImageInfo.h"

		 
