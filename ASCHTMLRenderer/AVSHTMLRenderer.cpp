// AVSHTMLRenderer.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include "HTMLRenderer.h"
#include "HTMLRenderer2.h"
#include "HTMLRenderer3.h"
#include "AVSSVGWriter.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{08DC1F03-6867-4585-B939-422C9078F5FD}", 
		 name = "AVSHTMLRenderer", 
		 helpstring = "AVSHTMLRenderer 1.0 Type Library",
		 resource_name = "IDR_AVSHTMLRENDERER") ]
class CAVSHTMLRendererModule
{
public:
// Override CAtlDllModuleT members
};
