#pragma once
#include "resource.h"       // main symbols
#include <string>
#include "./../Common/OfficeFileTemplate.h"
#include "./../Common/OfficeFileErrorDescription.h"
#include "./../Common/OfficeDefines.h"

#include "../Common/XmlUtils.h"
#include "../Common/ASCUtils.h"
#include "XPS.h"
#include "OfficeUtilsEvents.h"
#include "File.h"

#include "TemporaryCS.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)


// IXPSFile
[
	object,
	uuid("A04D7AB8-22E9-4d83-9941-EB5E1B22113D"),
	dual,	helpstring("IXPSFileInterface"),
	pointer_default(unique)
]
__interface IXPSFile: IAVSOfficeFileTemplate
{
	[id(101), propget] HRESULT DocumentRenderer([out, retval] IUnknown** ppRenderer);
	[id(101), propput] HRESULT DocumentRenderer([in] IUnknown* pRenderer);

	[id(102), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
    	[id(102), propput] HRESULT TempDirectory( [in] BSTR bstrDir );

	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in]  VARIANT   ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);
};


// CXPSFile

[
	coclass,
	default(IXPSFile, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeXPSFile.XPSFile"),
	progid("AVSOfficeXPSFile.XPSFile.1"),
	version(1.0),
	uuid("E0F07262-1673-4cf8-867C-05F3EB11F1BB"),
	helpstring("XPSFile Class")
]
class ATL_NO_VTABLE CXPSFile: public IXPSFile, public IAVSOfficePages
{
private:
	CString m_strTempDirectory;
	ASCGraphics::IASCDocumentRenderer* m_pDocumentRenderer;

	CRITICAL_SECTION m_oCS;

	ASCOfficeUtils::IOfficeUtils*	m_pOfficeUtils;
	XPS::Folder*					m_pFolder;

public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

	STDMETHOD(get_DocumentRenderer)(IUnknown** ppRenderer)
	{
		if( NULL == ppRenderer )
			return S_FALSE;
		(*ppRenderer) = NULL;
		if( NULL == m_pDocumentRenderer )
			return S_OK;
		return m_pDocumentRenderer->QueryInterface(__uuidof(ASCGraphics::IASCDocumentRenderer), (void**)ppRenderer);
	}
	STDMETHOD(put_DocumentRenderer)(IUnknown* pRenderer)
	{
		RELEASEINTERFACE( m_pDocumentRenderer );
		if( NULL != pRenderer )
			pRenderer->QueryInterface(__uuidof(ASCGraphics::IASCDocumentRenderer), (void**)&m_pDocumentRenderer);
		return S_OK;
	}
	STDMETHOD(get_TempDirectory)(BSTR* pbsPath)
	{
		if( NULL == pbsPath )
			return S_FALSE;
		
		*pbsPath = m_strTempDirectory.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_TempDirectory)(BSTR bsPath)
	{
		m_strTempDirectory = (CString)bsPath;
		return S_OK;
	}
	STDMETHOD(get_PagesCount)(LONG* plCount);
	STDMETHOD(GetPageInfo)(int nPage, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
	STDMETHOD(DrawPage)(int nPage, IUnknown* punkRenderer, BOOL* pBreak);

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT   ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT *	ParamValue)
	{
		return S_OK;
	}


public:
	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	CXPSFile();
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();
};