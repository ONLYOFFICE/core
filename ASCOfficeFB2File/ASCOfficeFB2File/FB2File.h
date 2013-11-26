// FB2File.h : Declaration of the CFB2File

#pragma once
#include "resource.h"       // main symbols
#include "..\..\Common\OfficeFileTemplate.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#import "..\..\Redist\ASCOfficeUtils.dll" raw_interfaces_only


// IFB2File
[
	object,
	uuid("2B2D5599-AB8E-4576-A12A-685991977712"),
	dual,	helpstring("IFB2File Interface"),
	pointer_default(unique)
]
__interface IFB2File : IAVSOfficeFileTemplate
{
	[id(10)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(20)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};


// _IFB2FileEvents
[
	dispinterface,
	uuid("1B5DE88E-5ADA-4b40-A28D-64839E6E135F"),
	helpstring("_IFB2FileEvents Interface")
]
__interface _IFB2FileEvents
{
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] DOUBLE progress);
	[id(2), helpstring("method OnComplete")] HRESULT OnComplete([in] LONG status);
};


// CFB2File

[
	coclass,
	default(IFB2File, _IFB2FileEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeFB2File.FB2File"),
	progid("AVSOfficeFB2File.FB2File.1"),
	version(1.0),
	uuid("4972B4CE-847E-4425-8D98-47549F59CD4F"),
	helpstring("FB2File Class")
]
class ATL_NO_VTABLE CFB2File :
	public IFB2File
{
public:
	CFB2File()
	{
	}

	__event __interface _IFB2FileEvents;


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

	STDMETHOD(SetAdditionalParam)( BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)( BSTR ParamName, VARIANT* ParamValue);
private:
	void CFB2File::createOriginDocx(const CString path) const;
	const unsigned long LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const;
	int RemoveDir(const CString sPath);


private:
	ASCOfficeUtils::IOfficeUtils*		m_pOfficeUtils;
	CString m_sMetadata;
};

