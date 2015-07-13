// OfficeXlsFile.h : Declaration of the COfficeXlsFile

#pragma once
#include "resource.h"       // main symbols

#include "../../Common/OfficeFileTemplate.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IOfficeXlsFile
[
	object,
	uuid("DC742420-E2C0-452D-8B59-9E52FD86CD9F"),
	dual,	helpstring("IOfficeXlsFile Interface"),
	pointer_default(unique)
]
__interface IOfficeXlsFile : IAVSOfficeFileTemplate
{
};


// _IOfficeXlsFileEvents
[
	dispinterface,
	uuid("E3009CC7-399B-49FE-9D88-6CE6C8FF0F97"),
	helpstring("_IOfficeXlsFileEvents Interface")
]
__interface _IOfficeXlsFileEvents
{
};


// COfficeXlsFile

[
	coclass,
	default(IOfficeXlsFile, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("ASCOfficeXlsFile2.OfficeXlsFile"),
	progid("ASCOfficeXlsFile2.OfficeXlsFile.1"),
	version(1.0),
	uuid("737DBB37-0309-4D1E-8DA4-3D6E247E693E"),
	helpstring("OfficeXlsFile Class")
]
class ATL_NO_VTABLE COfficeXlsFile :
	public IOfficeXlsFile
{
public:
	COfficeXlsFile();

	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    STDMETHOD(LoadFromFile)	(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)	(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);
   
	HRESULT LoadFromFileImpl(const std::wstring & srcFileName, const std::wstring & dstTempPath, const std::wstring & dstPath);

protected:

	static void OnProgressFunc (LPVOID lpParam, long nID, long nPercent);
	static void OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop);
};

