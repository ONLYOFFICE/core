// XlsFile.h : Declaration of the CXlsFile

#pragma once
#include "resource.h"       // main symbols
#include "..\..\Common\OfficeFileTemplate.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IXlsFile
[
	object,
	uuid("2A3C19F6-2587-45F0-9017-8710AA88F848"),
	dual,	helpstring("IXlsFile Interface"),
	pointer_default(unique)
]
__interface IXlsFile : IAVSOfficeFileTemplate
{
};


// _IXlsFileEvents
[
	dispinterface,
	uuid("AAA0B6F0-938F-40BA-8962-60A9E5494624"),
	helpstring("_IXlsFileEvents Interface")
]
__interface _IXlsFileEvents
{
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] DOUBLE progress);
	[id(2), helpstring("method OnComplete")] HRESULT OnComplete([in] LONG status);
};


// CXlsFile

[
	coclass,
	default(IXlsFile, _IXlsFileEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeXlsFile.XlsFile"),
	progid("AVSOfficeXlsFile.XlsFile.1"),
	version(1.0),
	uuid("F6EB038F-6506-4B83-8BE0-57EF8F3875FE"),
	helpstring("XlsFile Class")
]
class ATL_NO_VTABLE CXlsFile :
	public IXlsFile
{
public:
	CXlsFile()
	{
	}

	__event __interface _IXlsFileEvents;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);
};

