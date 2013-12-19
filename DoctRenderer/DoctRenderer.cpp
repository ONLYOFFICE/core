// DoctRenderer.cpp : Implementation of DLL Exports.
#include "stdafx.h"
#include "resource.h"

#include "DoctRenderer.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{8AFF13BF-79BE-4682-89AF-2CA81245D3E9}", 
		 name = "DoctRenderer", 
		 helpstring = "DoctRenderer 1.0 Type Library",
		 resource_name = "IDR_DOCTRENDERER") ]
class CDoctRendererModule
{
public:
// Override CAtlDllModuleT members
};
		 
