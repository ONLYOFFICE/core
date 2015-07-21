// ASCHTMLRenderer.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include "HTMLRenderer.h"
#include "HTMLRenderer2.h"
#include "HTMLRenderer3.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{08DC1F03-6867-4585-B939-422C9078F5FD}", 
		 name = "ASCHTMLRenderer", 
		 helpstring = "ASCHTMLRenderer 1.0 Type Library",
		 resource_name = "IDR_ASCHTMLRENDERER") ]
class CASCHTMLRendererModule
{
public:
// Override CAtlDllModuleT members
};
