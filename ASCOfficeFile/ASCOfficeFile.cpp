// ASCOfficeFile.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include "OfficeFile.h"
#include "ASCOfficeFormatChecker.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{74cbf220-bf08-11de-8a39-0800200c9a66}", 
		 name = "ASCOfficeFile", 
		 helpstring = "ASCOfficeFile 1.0 Type Library",
		 resource_name = "IDR_ASCOFFICEFILE") ]
class CASCOfficeFileModule
{
public:
// Override CAtlDllModuleT members
};
		 
