// AVSOfficePPTXFile.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

#include "AVSOfficePPTXFile.h"
#include "AVSOfficeDrawingConverter.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{95DEE027-7707-4078-A314-33D35AE842FD}", 
		 name = "AVSOfficePPTXFile", 
		 helpstring = "AVSOfficePPTXFile 1.0 Type Library",
		 resource_name = "IDR_AVSOFFICEPPTXFILE") ];
