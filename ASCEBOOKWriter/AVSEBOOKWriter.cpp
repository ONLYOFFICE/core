// AVSEBOOKWriter.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include "EBOOKWriter.h"
#include "MobiFormat/MobiFile.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{218DB989-85F3-4512-8A6D-71A7211FBC5A}", 
		 name = "AVSEBOOKWriter", 
		 helpstring = "AVSEBOOKWriter 1.0 Type Library",
		 resource_name = "IDR_AVSEBOOKWRITER") ];