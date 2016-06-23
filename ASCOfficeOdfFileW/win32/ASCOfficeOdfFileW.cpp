/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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

