// RtfFile.h : Declaration of the CRtfFile

#pragma once
#include "resource.h"       // main symbols
#include "../../Common/OfficeFileTemplate.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IRtfFile
[
	object,
	uuid("CEAEE73E-7956-4913-B0B1-B44EF5D63F12"),
	dual,	helpstring("IRtfFile Interface"),
	pointer_default(unique)
]
__interface IRtfFile : IAVSOfficeFileTemplate
{
};

// CRtfFile
[
	coclass,
	default(IRtfFile, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeRtfFile.RtfFile"),
	progid("AVSOfficeRtfFile.RtfFile.1"),
	version(1.0),
	uuid("C1A7B36E-2CA1-4357-803D-440BA92EA753"),
	helpstring("RtfFile Class")
]
class ATL_NO_VTABLE CRtfFile : public IRtfFile
{
public:
	CRtfFile()
	{
	}
	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_nLastProgress = -1;
		return S_OK;
	}

	void FinalRelease()
	{
	}
private:
	long m_nLastProgress;
	bool IsRtfFile(CString sFilename, long& nError );
public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);
};