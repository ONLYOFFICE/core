// ASCOfficeUtils.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{92F87FA9-D3C2-4820-82F6-DEAEC0A3539D}", 
		 name = "ASCOfficeUtils", 
		 helpstring = "ASCOfficeUtils 1.0 Type Library",
		 resource_name = "IDR_ASCOFFICEUTILS") ]
class CAVSOfficeUtilsModule
{
public:
// Override CAtlDllModuleT members
};
		 
#include "OfficeUtils.h"
#include "OfficeOCR.h"