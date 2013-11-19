// AVSDjVuFile.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include "OfficeDjvuFile.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{4481D89B-F057-495e-A9C7-54D72A248720}", 
		 name = "AVSOfficeDjVuFile", 
		 helpstring = "AVSOfficeDjVuFile 1.0 Type Library",
		 resource_name = "IDR_AVSDJVUFILE") ]
class CAVSDjVuFileModule
{
public:
// Override CAtlDllModuleT members
};
