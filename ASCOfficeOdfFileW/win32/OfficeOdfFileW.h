// OfficeOdfFileW.h : Declaration of the COfficeOdfFileW

#pragma once
#include "resource.h"       // main symbols

#include <string>

#include "./../Common/OfficeFileTemplate.h"
#include "./../Common/OfficeFileErrorDescription.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#ifdef _DEBUG
	#import "..\Redist\ASCOfficeUtils.dll" rename_namespace("ASCOfficeUtils") raw_interfaces_only
#else
	#import "..\Redist\ASCOfficeUtils.dll" rename_namespace("ASCOfficeUtils") raw_interfaces_only
#endif

// IOfficeOdfFileW
[
	object,
	uuid("E171F3C8-5715-41AB-9678-85AF4B7CE2FE"),
	dual,	helpstring("IOfficeOdfFileW Interface"),
	pointer_default(unique)
]
__interface IOfficeOdfFileW : IAVSOfficeFileTemplate
{
};


// _IOfficeOdfFileWEvents
[
	dispinterface,
	uuid("BF816058-77B7-497A-9204-1AE5A37956FC"),
	helpstring("_IOfficeOdfFileWEvents Interface")
]
__interface _IOfficeOdfFileWEvents
{
};


// COfficeOdfFileW

[
	coclass,
	default(IOfficeOdfFileW, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("ASCOfficeOdfFileW.OfficeOdfFileW"),
	progid("ASCOfficeOdfFileW.OfficeOdfFileW.1"),
	version(1.0),
	uuid("E1D4E9D1-69A7-494B-B2A0-0F7F37449044"),
	helpstring("OfficeOdfFileW Class")
]
class ATL_NO_VTABLE COfficeOdfFileW :
	public IOfficeOdfFileW
{
public:
	COfficeOdfFileW()
	{
	}

	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		office_utils_.CoCreateInstance(__uuidof(ASCOfficeUtils::COfficeUtils));    

		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

private:
    ATL::CComPtr< ASCOfficeUtils::IOfficeUtils > office_utils_;

    bool initialized();

	HRESULT SaveToFileImpl(const std::wstring & srcPath, const std::wstring & srcTempPath,
						   const std::wstring & dstTempPath,	 const std::wstring & dstFileName);

	std::wstring DetectTypeDocument(const std::wstring & Path);

protected:

	static void OnProgressFunc (LPVOID lpParam, long nID, long nPercent);
	static void OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop);

};

