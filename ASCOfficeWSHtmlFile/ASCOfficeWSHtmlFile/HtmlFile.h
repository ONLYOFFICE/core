// HtmlFile.h : Declaration of the CWSHtmlFile

#pragma once
#include "resource.h"       // main symbols
#include "..\..\Common\OfficeFileTemplate.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IWSHtmlFile
[
	object,
	uuid("CCF70ADF-EB35-4e11-A0BB-D039DC538BE6"),
	dual,	helpstring("IWSHtmlFile Interface"),
	pointer_default(unique)
]
__interface IWSHtmlFile : IAVSOfficeFileTemplate
{
};


// _IWSHtmlFileEvents
[
	dispinterface,
	uuid("1BA617F0-31CA-4104-9F80-7B098155755F"),
	helpstring("_IWSHtmlFileEvents Interface")
]
__interface _IWSHtmlFileEvents
{
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] DOUBLE progress);
	[id(2), helpstring("method OnComplete")] HRESULT OnComplete([in] LONG status);
};


// CWSHtmlFile

[
	coclass,
	default(IWSHtmlFile, _IWSHtmlFileEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeWSHtmlFile.WSHtmlFile"),
	progid("AVSOfficeWSHtmlFile.WSHtmlFile.1"),
	version(1.0),
	uuid("4B835C27-09B7-49e8-B9C8-5F7A5E5048BF"),
	helpstring("WSHtmlFile Class")
]
class ATL_NO_VTABLE CWSHtmlFile :
	public IWSHtmlFile
{
public:
	CWSHtmlFile()
	{
	}

	__event __interface _IWSHtmlFileEvents;


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

