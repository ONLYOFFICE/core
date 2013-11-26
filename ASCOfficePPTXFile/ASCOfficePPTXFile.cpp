//ASCOfficePPTXFile.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

#include "ASCOfficePPTXFile.h"
#include "ASCOfficeDrawingConverter.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{95DEE027-7707-4078-A314-33D35AE842FD}", 
		 name = "ASCOfficePPTXFile", 
		 helpstring = "ASCOfficePPTXFile 1.0 Type Library",
		 resource_name = "IDR_ASCOFFICEPPTXFILE") ];
