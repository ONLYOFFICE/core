// EditorEngine.cpp : Implementation of DLL Exports.
#include "stdafx.h"
#include "resource.h"

#include "EditorEngine.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{0F4A3925-17B1-4175-98CB-D5EA744B4434}", 
		 name = "EditorEngine", 
		 helpstring = "EditorEngine 1.0 Type Library",
		 resource_name = "IDR_EDITORENGINE") ]
class CEditorEngineModule
{
public:
// Override CAtlDllModuleT members
};
		 
