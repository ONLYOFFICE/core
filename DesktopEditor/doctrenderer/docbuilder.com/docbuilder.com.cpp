// docbuilder.com.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

#include "docbuilder.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{B43F4AFD-2278-4175-992C-D7AE390507D8}", 
		 name = "ascdocbuilder", 
		 helpstring = "ascdocbuilder 1.0 Type Library",
		 resource_name = "IDR_ASCDOCBUILDER") ];

		 
