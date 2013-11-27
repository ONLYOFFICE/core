// AVSDocumentConverter.h : Declaration of the CAVSDocumentConverter

#pragma once
#include "resource.h"       // main symbols
#include "..\Common\OfficeFileTemplate.h"
#include "AtlDefine.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IAVSDocumentConverter
[
	object,
	uuid("DA839E4E-A559-4ee1-A863-4A6D44484989"),
	dual,	helpstring("IAVSOfficeEpubFile Interface"),
	pointer_default(unique)
]
__interface IOfficeEpubFile : IAVSOfficeFileTemplate
{
	[id(10)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(20)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
	[id(30), propget] HRESULT CommandRenderer([out, retval] IUnknown** ppunkRend);
	[id(30), propput] HRESULT CommandRenderer([in] IUnknown* punkRend);
};

// CAVSOfficeEpubFile

[
	coclass,
	default(IOfficeEpubFile),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeEpubFile.AVSDocume"),
	progid("AVSOfficeEpubFile.AVSDocu.1"),
	version(1.0),
	uuid("82506410-0164-459a-A191-381C22F1550E"),
	helpstring("AVSOfficeEpubFile Class")
]
class ATL_NO_VTABLE COfficeEpubFile : public IOfficeEpubFile
{
public:
	COfficeEpubFile():m_piCommandsRenderer(NULL)
	{
		m_nTasksAll = 0;
		m_nTasksComplete = 0;
		m_bCancel = false;
		m_sMetadata = _T("");
	}
	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEINTERFACE (m_piCommandsRenderer);
	}
public:
	STDMETHOD(LoadFromFile)( BSTR bstrSrcFileName,  BSTR bstrDstPath,  BSTR bstrXMLOptions);
	STDMETHOD(SaveToFile)( BSTR bstrDstFileName,  BSTR bstrSrcPath,  BSTR bstrXMLOptions);
	STDMETHOD(SetAdditionalParam)( BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)( BSTR ParamName, VARIANT* ParamValue);
	STDMETHOD(get_CommandRenderer)(IUnknown** ppunkRend);
	STDMETHOD(put_CommandRenderer)(IUnknown* ppunkRend);
public:
	HRESULT OnProgressHtml( LONG nID, LONG nPercent );
	HRESULT OnProgressHtmlEx( LONG nID, LONG nPercent, SHORT* Cancel );
private: 
	BOOL IsEpubFile(CString sFilename, long& nError );
	bool SaveCover( CString sFilename, long& nWidth, long& nHeight );
	void UpdateGdiPlusRenderer(double dWidthMm, double dHeightMm,AVSMediaCore3::IAVSUncompressedVideoFrame** piImage,AVSGraphics::IASCGraphicsRenderer** piRend);
private: 

	AVSGraphics::IASCDocumentRenderer *	m_piCommandsRenderer;
	int m_nTasksAll;
	int m_nTasksComplete;
	bool m_bCancel;
	CString m_sMetadata;
};