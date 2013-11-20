// OfficeCore.cpp : Implementation of DLL Exports.
#include "stdafx.h"
#include "resource.h"

// uncompressed frame
#include "UncompressedFrame/UncompressedFrame.h"

// fonts
#include "Fonts/WinFonts.h"

// images
#include "Images/ImageGdipFile.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{2FA8EF81-55E5-4f98-B399-2DF9A5DA5E41}", 
		 name = "OfficeCore", 
		 helpstring = "OfficeCore 1.0 Type Library",
		 resource_name = "IDR_OFFICECORE") ]
class COfficeCoreModule
{
public:
// Override CAtlDllModuleT members
};
