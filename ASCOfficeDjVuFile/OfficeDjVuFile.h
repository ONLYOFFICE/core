#pragma once
#include "resource.h"       // main symbols
#include <string>
#include ".\..\Common\OfficeFileTemplate.h"
#include ".\..\Common\OfficeFileErrorDescription.h"
#include ".\..\Common\OfficeDefines.h"
#include ".\..\Common\XmlUtils.h"
#include ".\..\Common\ASCUtils.h"

#include "libdjvu/DjVuDocument.h"
#include "libdjvu/DjVuImage.h"
#include "libdjvu/GBitmap.h"
#include "libdjvu/GScaler.h"
#include "libdjvu/IFFByteStream.h"
#include "libdjvu/BSByteStream.h"
#include "libdjvu/DataPool.h"
#include "libdjvu/DjVuText.h"
#include "libdjvu/DjVmNav.h"

#include ".\..\Common\MediaFormatDefine.h"
#include "Image.h"
#include ".\..\Common\TemporaryCS.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)


// IDjVuFile
[
	object,
	uuid("5C51FA43-3730-45b8-8DDA-4909545ECC8C"),
	dual,	helpstring("IDjVuFile Interface"),
	pointer_default(unique)
]
__interface IDjVuFile : IAVSOfficeFileTemplate
{
	[id(101), propget] HRESULT DocumentRenderer([out, retval] IUnknown** ppRenderer);
	[id(101), propput] HRESULT DocumentRenderer([in] IUnknown* pRenderer);

	[id(102), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
    [id(102), propput] HRESULT TempDirectory( [in] BSTR bstrDir );

	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in]  VARIANT   ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);
};




// CDjVuFile

[
	coclass,
	default(IDjVuFile),
	threading(apartment),
	event_source(com),
	vi_progid("AVSDjVuFile.DjVuFile"),
	progid("AVSDjVuFile.DjVuFile.1"),
	version(1.0),
	uuid("648CEF06-1536-4df1-ABEA-C965DEB50B02"),
	helpstring("DjVuFile Class")
]
class ATL_NO_VTABLE CDjVuFile : public IDjVuFile, public IAVSOfficePages
{
private:
	CString m_strTempDirectory;
	AVSGraphics::IASCDocumentRenderer* m_pDocumentRenderer;
	//OfficeEditor::IAVSCommandsRenderer* m_pDocumentRenderer;
	NSImage::CImage m_oImage;

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
		return m_pDocumentRenderer->QueryInterface(__uuidof(AVSGraphics::IASCRenderer), (void**)ppRenderer);
	}
	STDMETHOD(put_DocumentRenderer)(IUnknown* pRenderer)
	{
		RELEASEINTERFACE( m_pDocumentRenderer );
		if( NULL != pRenderer )
			pRenderer->QueryInterface(__uuidof(AVSGraphics::IASCRenderer), (void**)&m_pDocumentRenderer);
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

	CDjVuFile();
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

private:
	void CreateFrame(GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& text);
	void CreateFrame(AVSGraphics::IASCRenderer* renderer, GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& text);
	void CreateFrame2(AVSGraphics::IASCRenderer* renderer, GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& text);
	void CreateFrame3(AVSGraphics::IASCGraphicsRenderer* renderer, GP<DjVuImage>& pImage, BOOL* pBreak);
	CString SavePage(int nPageNum, IUnknown* pPage);
	XmlUtils::CXmlNode ParseText(GP<DjVuImage> pPage);
	void PageToRenderer(double width, double height);
	void TextToRenderer(XmlUtils::CXmlNode text, double koef, bool isView = true);
	void ImageToRenderer(const CString& strName, double width, double height);

	void PageToRenderer(AVSGraphics::IASCRenderer* renderer, double width, double height);
	void TextToRenderer(AVSGraphics::IASCRenderer* renderer, XmlUtils::CXmlNode text, double koef, bool isView = true);
	void ImageToRenderer(AVSGraphics::IASCRenderer* renderer, const CString& strName, double width, double height);

	void ParseCoords(CString& coordsStr, double* coords, double koef);
	void DrawRect(double* coords);
	void DrawText(double* coords, CString text);

	void DrawRect(AVSGraphics::IASCRenderer* renderer, double* coords);
	void DrawText(AVSGraphics::IASCRenderer* renderer, double* coords, CString text);

private:
	GP<DjVuDocument> m_pDoc;
	CRITICAL_SECTION m_oCS;
};