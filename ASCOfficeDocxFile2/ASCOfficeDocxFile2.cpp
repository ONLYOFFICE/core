// AVSOfficeDocxFile2.cpp : Implementation of DLL Exports.
#include "stdafx.h"
#include "resource.h"

#include "DocxFile2.h"
#include "XlsxFile2.h"
#include "PptxFile.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{A1EEE61A-FAA7-47af-B078-4E955623B9CA}", 
		 name = "ASCOfficeDocxFile2", 
		 helpstring = "ASCOfficeDocxFile2 1.0 Type Library",
		 resource_name = "IDR_ASCOFFICEDOCXFILE2") ];