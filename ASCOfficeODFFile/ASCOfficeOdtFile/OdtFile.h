// OdtFile.h : Declaration of the COdtFile

#pragma once
#include "resource.h"       // main symbols

////////////////////////////////////////////////////////////////
// IASCOfficeFileTemplate
[
	object,
	uuid("3FC4EC15-9467-4D66-AD6A-A0C0BAEDD3CD"),
	dual,	helpstring("IASCOfficeFileTemplate Interface"),
	pointer_default(unique)
]
__interface IASCOfficeFileTemplate : IDispatch
{
	[id(1), helpstring("method LoadFromFile")] HRESULT LoadFromFile([in] BSTR sSrcFileName, [in] BSTR sDstPath, [in] BSTR sXMLOptions);
	[id(2), helpstring("method SaveToFile")] HRESULT SaveToFile([in] BSTR sDstFileName, [in] BSTR sSrcPath, [in] BSTR sXMLOptions);
}; 
////////////////////////////////////////////////////////////////


// _IASCOfficeFileTemplateEvents2
[
	dispinterface,
	uuid("9764153E-DAEB-40dd-94D4-A2C39218AF64"),
	helpstring("_IASCOfficeFileTemplateEvents2 Interface")
]

__interface _IASCOfficeFileTemplateEvents2
{
	//Max Value nPercent == 1000000
	//Example 23,56 % == 235600
	[id(2), helpstring("method OnProgressEx")] HRESULT OnProgressEx([in] LONG nID, [in] LONG nPercent, [in, out, ref] SHORT* Cancel);
};


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IOdtFile
[
	object,
	uuid("7DEC2313-C482-4039-A5C6-083F1EDD86AB"),
	dual,	helpstring("IOdtFile Interface"),
	pointer_default(unique)
]
__interface IOdtFile : IASCOfficeFileTemplate
{
};


// _IOdtFileEvents
[
	dispinterface,
	uuid("E001F8EB-4E63-4E33-A22E-51EB9BD53CC3"),
	helpstring("_IOdtFileEvents Interface")
]
__interface _IOdtFileEvents
{
};


// COdtFile

[
	coclass,
	default(IOdtFile, _IASCOfficeFileTemplateEvents2),
	threading(apartment),
	support_error_info("IOdtFile"),
	event_source(com),
	vi_progid("ASCOdtFile.OdtFile"),
	progid("ASCOdtFile.OdtFile.1"),
	version(1.0),
	uuid("04596A11-01C4-48D9-B020-B08E3F443F0F"),
	helpstring("OdtFile Class")
]
class ATL_NO_VTABLE COdtFile :
	public IOdtFile
{
public:
	COdtFile();
    ~COdtFile();

	__event __interface _IASCOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

public:

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

private:
    ATL::CComPtr<IASCOfficeFileTemplate> odfFile_;

};

