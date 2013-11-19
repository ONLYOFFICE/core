// AVSOfficeEditor.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include "AVSOfficeViewer.h"
#include "OfficeCommandsRenderer.h"
#include "OfficeHDCRenderer.h"
#include "OfficeGdiplusRenderer.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{9B236E80-E92D-4CAA-94E5-FB832847CEAE}", 
		 name = "AVSOfficeEditor", 
		 helpstring = "AVSOfficeEditor 1.0 Type Library",
		 resource_name = "IDR_AVSOFFICEEDITOR") ];
		 
