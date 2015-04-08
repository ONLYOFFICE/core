// ASCOfficeOdfFileW.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "ASCOfficeOdfFileW.h"
[
	module(dll, uuid = "{EDB084E5-EC38-47EF-B421-D5E1A71F98A6}", helpstring = "ASCOfficeOdfFileW 1.0 Type Library", name = "ASCOfficeOdfFileWLib")
];


//class CASCOfficeOdfFileWModule : public CAtlDllModuleT< CASCOfficeOdfFileWModule >
//{
//public :
//	DECLARE_LIBID(LIBID_ASCOfficeOdfFileWLib)
//	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ASCOFFICEODFFILEW, "{A5D1961C-EE80-4038-A6CA-21F8C454571D}")
//};

//CASCOfficeOdfFileWModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
//extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
//{
//	hInstance;
//    return _AtlModule.DllMain(dwReason, lpReserved); 
//}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// Used to determine whether the DLL can be unloaded by OLE
//STDAPI DllCanUnloadNow(void)
//{
//    return _AtlModule.DllCanUnloadNow();
//}


// Returns a class factory to create an object of the requested type
//STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
//{
//    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
//}


// DllRegisterServer - Adds entries to the system registry
//STDAPI DllRegisterServer(void)
//{
//    // registers object, typelib and all interfaces in typelib
//    HRESULT hr = _AtlModule.DllRegisterServer();
//	return hr;
//}


// DllUnregisterServer - Removes entries from the system registry
//STDAPI DllUnregisterServer(void)
//{
//	HRESULT hr = _AtlModule.DllUnregisterServer();
//	return hr;
//}

