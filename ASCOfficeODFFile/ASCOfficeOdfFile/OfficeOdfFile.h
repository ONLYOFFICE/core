// OfficeOdfFile.h : Declaration of the COfficeOdfFile

#pragma once
#include "resource.h"       // main symbols
#include <string>

//#import "libid:92F87FA9-D3C2-4820-82F6-DEAEC0A3539D" rename_namespace("AVSOfficeUtils"), raw_interfaces_only
#import "../../Redist/ASCOfficeUtils.dll" rename_namespace("AVSOfficeUtils"), raw_interfaces_only

////////////////////////////////////////////////////////////////
// IAVSOfficeFileTemplate
[
	object,
	uuid("3FC4EC15-9467-4D66-AD6A-A0C0BAEDD3CD"),
	dual,	helpstring("IAVSOfficeFileTemplate Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeFileTemplate : IDispatch
{
	[id(1), helpstring("method LoadFromFile")] HRESULT LoadFromFile([in] BSTR sSrcFileName, [in] BSTR sDstPath, [in] BSTR sXMLOptions);
	[id(2), helpstring("method SaveToFile")] HRESULT SaveToFile([in] BSTR sDstFileName, [in] BSTR sSrcPath, [in] BSTR sXMLOptions);
};
////////////////////////////////////////////////////////////////


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IOfficeOdfFile
[
	object,
	uuid("CD223963-DF33-44FD-9A6A-4753760164E7"),
	dual,	helpstring("IOfficeOdfFile Interface"),
	pointer_default(unique)
]
__interface IOfficeOdfFile : IAVSOfficeFileTemplate
{
};


// _IOfficeOdfFileEvents
[
	dispinterface,
	uuid("ACAD723A-D5F0-4738-AC7D-CA17AE701629"),
	helpstring("_IOfficeOdfFileEvents Interface")
]
__interface _IOfficeOdfFileEvents
{
};


// COfficeOdfFile

[
	coclass,
	default(IOfficeOdfFile, _IOfficeOdfFileEvents),
	threading(apartment),
	support_error_info("IOfficeOdfFile"),
	event_source(com),
	vi_progid("AVSOfficeOdfFile.OfficeOdfFile"),
	progid("AVSOfficeOdfFile.OfficeOdfFile.1"),
	version(1.0),
	uuid("99901ECB-3527-494D-A9EC-5A7CA98AD18B"),
	helpstring("OfficeOdfFile Class")
]
class ATL_NO_VTABLE COfficeOdfFile :
	public IOfficeOdfFile
{
public:
	COfficeOdfFile();

	__event __interface _IOfficeOdfFileEvents;


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

private:
    ATL::CComPtr< AVSOfficeUtils::IOfficeUtils > office_utils_;
    
private:
    bool initialized();
    HRESULT LoadFromFileImpl(const std::wstring & srcFileName,
        const std::wstring & srcTempPath,
        const std::wstring & dstTempPath,
        const std::wstring & dstPath);
};
