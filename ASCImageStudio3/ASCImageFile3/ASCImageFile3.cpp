// ASCImageFile3.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{E3305D24-A207-4431-BC43-2C09A53CB9B6}", 
		 name = "ASCImageFile3", 
		 helpstring = "ASCImageFile 3",
		 resource_name = "IDR_ASCIMAGEFILE3") ];

#include "ImageFile3.h"