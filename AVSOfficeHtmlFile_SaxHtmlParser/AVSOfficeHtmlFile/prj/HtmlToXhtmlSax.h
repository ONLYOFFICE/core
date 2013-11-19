#pragma once

#import "shdocvw.dll" exclude ("OLECMDID", "OLECMDF", "OLECMDEXECOPT", "tagREADYSTATE")

#include "shdocvw.tlh"
#include <string>
#include <excpt.h>
#include <map>
#include <io.h>
#include "CHtmlCSS.h"
#include "..\..\HTMLReaderLib\LiteHTMLConverter.h"
#include <vector>
#include "..\src\MhtFile.h"

using namespace CSS;

typedef std::map< std::wstring, std::wstring > wstrmap;

class CHtmlToXhtmlSax:
	public IXHTMLWriter
{
public:
	CHtmlToXhtmlSax (CString sHtmlPath, CString sXhtmlPath, const MhtFile* pMhtFile);
	virtual ~CHtmlToXhtmlSax(void);

public:
	void SetDownloadImages(BOOL bDownload);
	CSimpleArray<CString> GetTempImages ();
	void AddTempImages (CSimpleArray<CString> &tempImages);
	static bool FileExist (CString *sFilePath);
	void SetDefaultCodePage (int aDefaultCodePage = CP_ACP);

	
    bool Convert ();
	BOOL SaveXML ();
	MSXML2::IXMLDOMNodePtr getDoc();
	MSXML2::IXMLDOMNodePtr getHtml();
	MSXML2::IXMLDOMNodePtr getBody();

    void parseCSS (const std::wstring& sCSS);
	void InsertPair (wstrmap& map, const std::wstring &key, const std::wstring &val);
	void convertElementFont (CHTMLTagWrapper*   pHtmlNode);
	void convertElementBodyBackgroundImage (CHTMLTagWrapper* pHtmlNode, MSXML2::IXMLDOMElementPtr pXmlNode);
	std::wstring getElementCSS (CHTMLTagWrapper* pHtmlNode);
    std::wstring convertElementAttr (CHTMLTagWrapper* pHtmlNode, const std::wstring& attribute);
	//std::wstring SetLowerStyleNames (const std::wstring& style);

    _bstr_t FindAttribute (CHTMLTagWrapper* oElement, const _bstr_t bsAttrName, const _bstr_t bsDefaultValue = _bstr_t( L"" ));
	_bstr_t getFilePath () const;
	static CString delComment (CString& style);

    bool LoadExternalCSS (CString sHref, std::wstring *pwsBuffer, CString sPath, BOOL bWeb);
    void nodeHtmlToXhtml (CHTMLTagWrapper* pHtmlNode, MSXML2::IXMLDOMNodePtr pXmlNode, double dEmValue = 16.0);

	BOOL AddAttributesToNode (CLiteHTMLElemAttr &oAttr, MSXML2::IXMLDOMElement *xml_element);	// copy atributes from our class to msxml node
	static CString convertBase64ToImage (CString &pBase64);

	void RemoveInvalidChars (BSTR *pbsString);
	void HtmlTagsToText (CStringA *psString);
	//static const std::wstring file2ansi (const std::vector<std::string>& file, const bool flag);
	

	int filter (unsigned int code, struct _EXCEPTION_POINTERS *ep);
	bool loadFile(LPCBYTE dataptr, DWORD datasize);

	// tools
	static void setAttrs (int & width, int & height, Gdiplus::Bitmap& img);
	static const std::wstring encoding (const std::wstring& line);
	static const std::wstring loadCSS (const CString& sCSS);	

//#pragma region IXHTMLWriter_impl
// IXHTMLWriter implementation
protected:

	BOOL CloseTagPrefix (CHTMLTagWrapper *pPrevTagWrapper, bool aCloseTag = false);
	virtual DWORD ErrorCode ();
	virtual void SetOutput (CString &aOut);
	virtual BOOL BeginFile ();
	virtual BOOL StartTag (CHTMLTagWrapper *pTagWrapper, CHTMLTagWrapper *pPrevTagWrapper = NULL);
	virtual BOOL EndTag (CHTMLTagWrapper *pTagWrapper);
	virtual BOOL EndFile ();
	virtual void Characters (CHTMLTagWrapper *pTagWrapper, CString aText, bool aBeforeLastChild);
//#pragma region IXHTMLWriter_impl

private:

	bool					m_bMht;             // Исходный файл в формате Mht?
	MhtFile					m_oMhtFile;

	CLiteHTMLConverter		m_oConverter;

	CString					m_sHtmlPath;		// Полный путь к HTML файлу.
	BYTE*					m_pBuffer;			// Буффер, в котором хранится загружаемый HTML файл.
	DWORD					m_dwBufferSize;		// Размер буффера.

	CString					m_sXhtmlPath;
    wstrmap					m_mStyles [4];	// 4 categories of selector-style maps.
	CHtmlCSSList			m_oCssList;

	BOOL					m_bDownloadImages;
	CSimpleArray<CString>	m_arrImageFileName;

	int						m_nDefaultCodePage;

	//в 2oox-document.xsl жестко прописано <w:pgSz w:w="11906" w:h="16838" /><w:pgMar w:top="1134" w:right="850" w:bottom="1134" w:left="1701"...
	//реальная ширина с учетом margin страницы в twips: width - 9355, height - 14570
	//в пунктах (twips = 20*pt) : width - 467,35, height - 728,5
	//в пикселях если считать dpi 96: width - 623 height - 971
	static const int m_cnPageWidthPx = 623;
	static const int m_cnPageHeightPx = 971;


	CComBSTR	m_bsStyle;
	CComBSTR	m_bsTitle;

	MSXML2::IXMLDOMDocumentPtr	m_pXmlDoc;
	MSXML2::IXMLDOMNodePtr		m_pXmlHtmlNodePtr;
	MSXML2::IXMLDOMNodePtr		m_pXmlBodyNodePtr;
	MSXML2::IXMLDOMNodePtr		m_pXmlFramesetNodePtr;
	MSXML2::IXMLDOMNodePtr		m_pXmlHeadNodePtr;
	MSXML2::IXMLDOMNodePtr		m_pXmlTitleNodePtr;
	MSXML2::IXMLDOMNodePtr		m_pXmlStyleNodePtr;

	// current parsed node
	MSXML2::IXMLDOMNodePtr		m_pXmlCurrentNodePtr;

private:
	// preprocess tag (common)
	BOOL ProcessTag (CHTMLTagWrapper *pTagWrapper);
	// 
	// preprocess default tag
	BOOL PreprocessDefaultTag (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess table tag
	BOOL PreprocessTable (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess p tag
	BOOL PreprocessP (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess img tag
	BOOL PreprocessImg (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess a tag
	BOOL PreprocessA (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess <frameset> tag
	BOOL PreprocessFrameset (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess body tag
	BOOL PreprocessBody (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess frame tag
	BOOL PreprocessFrame (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess font tag
	BOOL PreprocessFontTag (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
	// preprocess text
	BOOL PreprocessText (CHTMLTagWrapper *pParentTagWrapper, const CString aText, bool aBeforeLastChild);
	// preprocess textarea tag
	BOOL PreprocessTextarea (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue = 16.0);
};
