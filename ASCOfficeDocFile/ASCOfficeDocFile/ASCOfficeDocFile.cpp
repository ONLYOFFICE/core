// AVSDocFile.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{35C0955D-A67D-4517-B670-5826ED38343D}", 
		 name = "AVSDocFile", 
		 helpstring = "AVSOfficeDocFile 1.0 Type Library",
		 resource_name = "IDR_AVSDOCFILE") ]
class CAVSDocFileModule
{
public:
// Override CAtlDllModuleT members
};
		 
