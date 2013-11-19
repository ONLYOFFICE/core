// AVSOfficeFile.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include "OfficeFile.h"
#include "AVSOfficeFormatChecker.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{74cbf220-bf08-11de-8a39-0800200c9a66}", 
		 name = "AVSOfficeFile", 
		 helpstring = "AVSOfficeFile 1.0 Type Library",
		 resource_name = "IDR_AVSOFFICEFILE") ]
class CAVSOfficeFileModule
{
public:
// Override CAtlDllModuleT members
};
		 
