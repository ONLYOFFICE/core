// HtmlFile.h : Declaration of the CHtmlFile

#pragma once
#include "resource.h"       // main symbols
#include "OfficeFileTemplate.h"
#include <string>
#include "..\..\..\..\..\..\Common\GdiplusEx.h"
#include "MhtFile.h"
#include "..\..\..\..\..\..\Common\TimeMeasurer.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IHtmlFile
[
	object,
	uuid("225197FF-DA39-4D1A-ADAB-8D21D9B443DE"),
	dual,	helpstring("IHtmlFile Interface"),
	pointer_default(unique)
]
__interface IHtmlFile : IAVSOfficeFileTemplate
{
	[id(100001)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(100002)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// CHtmlFile
[
	coclass,
	default(IHtmlFile, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeHtmlFile.HtmlFile"),
	progid("AVSOfficeHtmlFile.HtmlFile.1"),
	version(1.0),
	uuid("2E0E11FB-BA98-41E8-A5BC-44F6328C8F91"),
	helpstring("HtmlFile Class")
]

class ATL_NO_VTABLE CHtmlFile :
	public IHtmlFile
{
private:

	bool HtmlToXHtml_htmlayout( const wchar_t * html_path, const wchar_t * xhtml_path, const MhtFile* mhtFile, BOOL bDownloadImages );
	bool HtmlToXHtml_mshtml( const wchar_t * html_path, const wchar_t * xhtml_path, const MhtFile* mhtFile, BOOL bDownloadImages );
    void HtmPostProcessorRemoveXmlns( BSTR sFileName );
    void HtmPostProcessorReplaceAttrValue( BSTR sFileName, BSTR sFind, BSTR sReplace );	
	void ImagePath(const std::wstring& mediaPath, const BSTR& htmlPath);
    void OoxPostProcessorCopyImages( BSTR sFileName, BSTR sSrcFileName, BSTR sDstPath );
	void OoxPostProcessorCopyImagesFromMht( BSTR sFileName, BSTR sSrcFileName, BSTR sDstPath, const MhtFile& mhtFile );
    void OoxPostProcessorCutStubChildren( MSXML2::IXMLDOMDocumentPtr xml_doc, MSXML2::IXMLDOMNodePtr pNode );	
    void OoxPostProcessorCutStubs( BSTR sFileName );

private:
	const std::wstring getName(const std::wstring& fileName);
	const std::wstring getPath(const std::wstring& fileName);
	int getSize(MSXML2::IXMLDOMNamedNodeMapPtr attributs, const std::wstring& nameSize);
	const int getHtmFormatFromOptions(BSTR sXMLOptions);
	const int getHtmFormatToLoad(BSTR sSrcFileName);
	const int getDestFormatFromOptions(BSTR sXMLOptions, BSTR& destFilePathInNoStandardLoad);
	const bool getDownloadImages(BSTR sXMLOptions);
	const bool getListOptions(BSTR sXMLOptions);
	const bool getCKEditorOption(BSTR sXMLOptions);

	int RemoveDir(const CString sPath );

private:
	HRESULT	html_to_xhtml( BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions );
	HRESULT LoadHtmlFile( BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions );
	HRESULT LoadMhtFile( BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions );
	void    HtmlProgress(LONG nID, LONG nPercent, SHORT *pshCancel);

private:
	const CLSID getSaveFormat();

private:
	void copyFile(const std::wstring& outFilePath, const std::wstring& inFilePath);
	void convertFile(const std::wstring& outFilePath, const std::wstring& inFilePath, const int height, const int width);
	
private:
    static const wchar_t m_sLibPath[];
	CGdiPlusInit         m_oInit;

	BOOL                  m_bDownloadImages;
	CSimpleArray<CString> m_arrImageFilePath;

public:
	CHtmlFile()
	{
#ifdef _DEBUG
		m_bDownloadImages = TRUE;
#else
		m_bDownloadImages = FALSE;
#endif
	}
	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

public:
	HRESULT FinalConstruct();
	void FinalRelease();
	
};