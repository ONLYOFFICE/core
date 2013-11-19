#pragma once

#import "shdocvw.dll"

#import "mshtml.tlb"

#include "shdocvw.tlh"
#include <mshtml.h>
#include "mshtml.tlh"
#include <string>
#include "htmlite.h"
#include <excpt.h>
#include <map>
#include <io.h>

#include "MhtFile.h"
#include "CHtmlCSS.h"

using namespace CSS;

class HtmlToXhtml
{
public:

    HtmlToXhtml  ( CString sHtmlPath, CString sXhtmlPath, const MhtFile* pMhtFile);
    ~HtmlToXhtml ( );

	void                  SetDownloadImages(BOOL bDownload)
	{
		m_bDownloadImages = bDownload;
	}
	CSimpleArray<CString> GetTempImages    ( )
	{
		return m_arrImageFileName;
	}

	void AddTempImages(CSimpleArray<CString> tempImages)
	{
		for(int i = 0; i < tempImages.GetSize(); i++)
		{
			m_arrImageFileName.Add(tempImages[i]);
		}
	}

    bool    Convert_mshtml();	
    bool	Convert_htmlayout();
	bool	WriteXHTML();
	bool    LoadHtml_htmlayout();
	bool	LoadHtml_mshtml();
	

	MSXML2::IXMLDOMNodePtr getDoc();
	MSXML2::IXMLDOMNodePtr getBody();

	static bool  FileExist          ( CString *sFilePath )
	{
		if ( 0 == sFilePath->Find( _T("file://") ) )
			*sFilePath = sFilePath->Mid( 7/*sizeof( _T("file://") )*/ );

		return ( -1 != _waccess( sFilePath->GetBuffer(), 0 ) );
	}	

private:

    typedef std::map< std::wstring, std::wstring > wstrmap;

    HtmlToXhtml();

	void read(const std::wstring & sHtmlPath);

    std::wstring loadCSS            ( const std::wstring      & sCSS );
    void         parseCSS           ( const std::wstring      & sCSS );
	void		 InsertPair			( wstrmap& map, const std::wstring &key, const std::wstring &val );
    void         convertElementFont ( htmlayout::dom::element   pHtmlNode );
	void		 convertElementBodyBackgroundImage( htmlayout::dom::element   pHtmlNode, MSXML2::IXMLDOMElementPtr pXmlNode ); 
    std::wstring getElementCSS      ( htmlayout::dom::element   pHtmlNode );
    std::wstring convertElementCSS  ( htmlayout::dom::element   pHtmlNode );
    std::wstring convertElementAttr ( htmlayout::dom::element   pHtmlNode,
                                      const std::wstring      & attribute );
	std::wstring SetLowerStyleNames ( const std::wstring& style);
    _bstr_t      FindAttribute      ( const htmlayout::dom::element oElement, const _bstr_t bsAttrName, const _bstr_t bsDefaultValue = _bstr_t( L"" ) );
	_bstr_t		 getFilePath();
	_bstr_t      delComment(_bstr_t style);
    bool         LoadExternalCSS    ( CString sHref, std::wstring *pwsBuffer, CString sPath, BOOL bWeb );
    void         nodeHtmlToXhtml    ( htmlayout::dom::element pHtmlNode, MSXML2::IXMLDOMNodePtr pXmlNode, double dEmValue = 16.0 );
	void         nodeHtmlToXhtml    ( MSHTML::IHTMLDOMNodePtr pHtmlNode, MSXML2::IXMLDOMNodePtr pXmlNode );


	void setAttrs(int & width, int & height, Gdiplus::Bitmap& img);
	void         PrepareHTML        ( );
	void         RemoveInvalidChars ( BSTR *pbsString );
	void         HtmlTagsToText     ( CStringA *psString );
	void         DecodeUrl          ( CString *psString );
	const std::wstring file2ansi(const std::vector<std::string>& file, const bool flag) const;
	const std::wstring HtmlToXhtml::encoding(const std::wstring& line) const ;

	int filter ( unsigned int code, struct _EXCEPTION_POINTERS *ep );
	bool HtmlToXhtml::loadFile(LPCBYTE dataptr, DWORD datasize);

private:

	bool					   m_bMht;             // Исходный файл в формате Mht?
	MhtFile					   m_oMhtFile;

    MSXML2::IXMLDOMDocumentPtr m_pXmlDoc;
    MSHTML::IHTMLDocument2Ptr  m_pHtmlDoc;
    HTMLite                    m_oHtmLite;

	CString                    m_sHtmlPath;        // Полный путь к HTML файлу.
	BYTE*                      m_pBuffer;          // Буффер, в котором хранится загружаемый HTML файл.
	DWORD                      m_dwBufferSize;     // Размер буффера.

	CString                    m_sXhtmlPath;
    wstrmap                    m_mStyles    [ 4 ]; // 4 categories of selector-style maps.
	CHtmlCSSList               m_oCssList;

	BOOL                       m_bDownloadImages;
	CSimpleArray<CString>      m_arrImageFileName;
	//в 2oox-document.xsl жестко прописано <w:pgSz w:w="11906" w:h="16838" /><w:pgMar w:top="1134" w:right="850" w:bottom="1134" w:left="1701"...
	//реальная ширина с учетом margin страницы в twips: width - 9355, height - 14570
	//в пунктах (twips = 20*pt) : width - 467,35, height - 728,5
	//в пикселях если считать dpi 96: width - 623 height - 971
	static const int m_cnPageWidthPx = 623;
	static const int m_cnPageHeightPx = 971;
 };