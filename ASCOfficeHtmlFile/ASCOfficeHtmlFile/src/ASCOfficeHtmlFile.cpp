// ASCOfficeHtmlFile.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{49CA524B-FD86-4D2B-8FB4-822641DDCE52}", 
		 name = "ASCOfficeHtmlFile", 
		 helpstring = "ASCOfficeHtmlFile 1.0 Type Library",
		 resource_name = "IDR_ASCHTMLFILE") ]
class CAVSOfficeHtmlFileModule
{
public:
// Override CAtlDllModuleT members
};

#include "..\..\HTMLReaderLib\LiteHTMLAttributes.h"
#include "..\..\HTMLReaderLib\LiteHTMLConverter.h"
