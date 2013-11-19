#include "StdAfx.h"
#include "HtmlToXhtmlSax.h"

#include <exdisp.h>
#include <string>
#include <iostream>
#include <locale>
#include <locale.h>
#include <fstream>
#include <vector>
#include <algorithm>

#include "..\src\utils.h"
#include <atlenc.h>
#include <atlcoll.h>
#include "gdiplus.h"
#include "..\src\path.h"
#include "..\src\utf8.h"
//#include "Utility.h"
#include "..\src\CHtmlUtility.h"

#include "FileDownloader.h"
#include "OfficeDrawing/File.h"
#include "HtmlUtils.h"
#include "..\..\..\..\..\..\Common\Base64.h"

// initializer
//CLiteHTMLElemAttr::CNamedColors CLiteHTMLElemAttr::_namedColors(166 /* block size */);
//CLiteHTMLConverter::CTagsInfo CLiteHTMLConverter::m_mTagsInfo(200 /* block size */);

// 
CHtmlToXhtmlSax::CHtmlToXhtmlSax (CString sHtmlPath, CString sXhtmlPath, const MhtFile* pMhtFile)
	: m_pBuffer (NULL)
	, m_sHtmlPath (sHtmlPath)
	, m_sXhtmlPath (sXhtmlPath)
	, m_pXmlDoc (L"Msxml2.DOMDocument")
	, m_bMht (false)
{
	m_dwBufferSize = 0;

	if (NULL != m_pXmlDoc)
		m_pXmlDoc->put_preserveWhiteSpace (VARIANT_TRUE);

	//PrepareHTML();
	m_oConverter.SetWriter (this);

	if (NULL != pMhtFile)
	{
		m_bMht     = true;
		m_oMhtFile = *pMhtFile;
	}
}

CHtmlToXhtmlSax::~CHtmlToXhtmlSax()
{
	if (m_pBuffer)
	{
		m_dwBufferSize = 0;
		delete [] m_pBuffer;
	}
}

////////////////////////////////////////////////////////////////////////
// temp images
void CHtmlToXhtmlSax::SetDownloadImages(BOOL bDownload)
{
	m_bDownloadImages = bDownload;
}
CSimpleArray<CString> CHtmlToXhtmlSax::GetTempImages ()
{
	return m_arrImageFileName;
}

void CHtmlToXhtmlSax::AddTempImages (CSimpleArray<CString>& tempImages)
{
	for(int i = 0; i < tempImages.GetSize(); i++)
	{
		m_arrImageFileName.Add(tempImages[i]);
	}
}

void CHtmlToXhtmlSax::SetDefaultCodePage (int aDefaultCodePage)
{
	m_nDefaultCodePage = aDefaultCodePage;
}

//////////////////////////////////////////////////////////////////////////
// helpers
// check file existanse
bool CHtmlToXhtmlSax::FileExist (CString *sFilePath)
{
	if ( 0 == sFilePath->Find( _T("file://") ) )
		*sFilePath = sFilePath->Mid( 7/*sizeof( _T("file://") )*/ );

	return ( -1 != _waccess( sFilePath->GetBuffer(), 0 ) );
}

// set img height and width attr
void CHtmlToXhtmlSax::setAttrs(int & width, int & height, Gdiplus::Bitmap& img)
{
	if ( width == 0 && height == 0)
	{
		width = img.GetWidth();
		height = img.GetHeight();
	}
	else if ( height == 0 )
	{
		if ( img.GetWidth() != 0 )
			height = img.GetHeight() * width / img.GetWidth();
	}
	else if ( width == 0 )
	{
		if ( img.GetHeight() != 0 )
			width = img.GetWidth() * height / img.GetHeight();
	}
	//уменьшаем размеры картинки до размеров видимой области страницы
	if( width > m_cnPageWidthPx || height > m_cnPageHeightPx )
	{
		if( width * m_cnPageHeightPx > m_cnPageWidthPx * height )
		{
			height = (long)(height * m_cnPageWidthPx / width);
			width = m_cnPageWidthPx;
		}
		else
		{
			width = (long)(width * m_cnPageHeightPx / height);
			height = m_cnPageHeightPx;
		}
	}
}

// encoding
const std::wstring CHtmlToXhtmlSax::encoding(const std::wstring& line) 
{
	if (line.length() != 0)
	{
		wchar_t * new_line = new wchar_t[line.length() * 3];

		size_t j = 0;
		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] != L'%')
			{
				new_line[j++] = line[i];
			}
			else
			{				
				std::wstring hex = strings::substring_between( line, i + 1, i + 2);
				if (hex != L"")
					new_line[j++] = strings::strFromHex(hex);
				i += 2;
			}
		}
		new_line[j] = L'\0';

		std::wstring result(new_line);
		if (new_line != 0)
			delete []new_line;
		std::wstring new_text(result.size(), 0);
		utf8_decode( result.begin(), result.end(), new_text.begin() );		

		return new_text;
	}
	std::wstring p;
	return p;
}

// loading css
const std::wstring CHtmlToXhtmlSax::loadCSS( const CString& sCSS )
{
    std::wstring res = sCSS;

    while ( strings::substring_between( res, L"@import", L";" ).length() )
    {
        std::wstring url = strings::trim( strings::substring_between( strings::substring_between( res, L"@import", L";" ), L"url(", L")" ) );
        if ( url.length() )
        {
            MSXML2::IXMLHTTPRequestPtr   xml_http_ptr ( L"Msxml2.Xmlhttp" );
            HRESULT                      hr;
            std::wstring                 insertion;

            hr = xml_http_ptr->open( L"GET", url_by_path( url ).c_str(), false );
            if ( S_OK != hr )
                continue;
            hr = xml_http_ptr->send();
            if ( S_OK != hr )
                continue;
            insertion = xml_http_ptr->responseText;
            res = strings::replace( res, L"@import", L";", insertion );
        }
        else
        {
            strings::cut( res, L"@import", L";" );
        }
    }

    return res;
}

void CHtmlToXhtmlSax::parseCSS (const std::wstring & sCSS)
{
    std::wstring tmp = sCSS;
    std::replace( tmp.begin(), tmp.end(), ( wchar_t ) 13, L' ' );
    std::replace( tmp.begin(), tmp.end(), ( wchar_t ) 10, L' ' );
    while ( ( std::wstring::npos != tmp.find( L"{" ) ) && ( std::wstring::npos != tmp.find( L"}" ) ) )
    {
        int          cat   = 0;                                 // Generic styles
        std::wstring key   = strings::trim( strings::substring_before( tmp, L"{" ) );
        std::wstring val   = strings::trim( strings::substring_between( tmp, L"{", L"}" ) );

        if ( std::wstring::npos != key.find( L"#" ) )           // Styles by id
            cat = 3;
        else if ( ( std::wstring::npos != key.find( L" " ) ) &&
                  ( std::wstring::npos == key.find( L"," ) ) )  // Styles for descendants
            cat = 2;
        else if ( std::wstring::npos != key.find( L"." ) )      // Styles for classes
            cat = 1;
        if ( ( key.length() ) && ( val.length() ) )
        {
			InsertPair(m_mStyles[ cat ], key, val);
            m_mStyles[ cat ].insert( std::make_pair( key, val ) );
        }
        tmp = strings::cut( tmp, L"", L"}" );
    }
}

void CHtmlToXhtmlSax::InsertPair (wstrmap& styleMap, const std::wstring &key, const std::wstring &val)
{
	wstrmap::iterator itr;
	 for (itr = styleMap.begin(); itr != styleMap.end(); ++itr)
	 {
		 if ( itr->first == key)
		 {
			 itr->second += L"; ";
			 itr->second += val;
			 break;
		 }
	 }	
	 if (itr == styleMap.end())
		 styleMap.insert(std::make_pair(key, val));

}

std::wstring CHtmlToXhtmlSax::getElementCSS (CHTMLTagWrapper* pHtmlNode)
{
    std::wstring res;

	
    for (UINT i = 0; i < 4; ++i)
    {
        for (wstrmap::iterator itr = m_mStyles[i].begin(); itr != m_mStyles[i].end(); ++itr)
        {
			try
			{
				//if (pHtmlNode.test((const char *) _bstr_t(itr->first.c_str())))
				{
					res += itr->second + L" ; ";
				}
			}
			catch (...)
			{
				continue;
			}
        }
    }    
	
    res = strings::trim (res);
    return res;
}

bool CHtmlToXhtmlSax::LoadExternalCSS (CString sHref, std::wstring *pwsText, CString sPath, BOOL bWeb)
{
	if ( bWeb ) // sHref - интернет ссылка
	{
		//USES_CONVERSION;

		//WSADATA WsaData;
		//if ( SOCKET_ERROR == WSAStartup (0x0101, &WsaData) )
		//{
		//	return false;
		//}
		//hostent *pHost = gethostbyname( W2A( sHref.GetBuffer() ) );
		//DWORD dwError = GetLastError();

		//SOCKET pSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		//struct sockaddr_in oWebAddress;
		//oWebAddress.sin_family           = AF_INET;
		//oWebAddress.sin_port             = htons( 80 );
		//oWebAddress.sin_addr.S_un.S_addr = inet_addr( W2A( sHref.GetBuffer() ) );

		//int nReceived = 0;
		//if ( connect( pSocket, (struct sockaddr *)&oWebAddress, sizeof(struct sockaddr) ) < 0 )
		//{
		//	closesocket( pSocket );
		//	return false;
		//}
		//
		//FILE *pFile = ::fopen( W2A( sFilePath.GetBuffer() ), "wb" );
		//if ( !pFile )
		//{
		//	closesocket( pSocket );
		//	return false;
		//}

		//BYTE pRecvBuffer[1];
		//while( ( nReceived = recv( pSocket, (char *)pRecvBuffer, sizeof(pRecvBuffer), 0 ) ) != SOCKET_ERROR )
		//{
		//	::fwrite( (char *)pRecvBuffer, 1, nReceived, pFile );
		//}
		//::fclose( pFile );
		//closesocket( pSocket );

		CFileDownloader oDownloader( sHref );
		oDownloader.StartWork( 1 );
		while ( oDownloader.IsRunned() )
		{
			Sleep( 10 );
		}

		if ( !oDownloader.IsFileDownloaded() )
			return false;

		CStringW sContent = NUnicodeTextFile::ReadFile (oDownloader.GetFilePath());
		*pwsText += sContent.GetString();
		*pwsText = strings::cut_all( *pwsText, L"/*", L"*/" );

		
		//std::wstring wsPath( oDownloader.GetFilePath().GetBuffer() );
		//TxtFile oFile( wsPath );

		//std::list<std::wstring> oContent;

		//if ( oFile.isUtf8() )
		//	oContent = transform( oFile.readUtf8(), Encoding::utf82unicode );
		//else if ( oFile.isUnicode() )
		//	oContent = oFile.readUnicode();
		//else
		//	oContent = transform( oFile.readAnsi(), Encoding::ansi2unicode);
		//for(std::list<std::wstring>::iterator list_iter = oContent.begin(); 
		//	list_iter != oContent.end(); list_iter++)
		//{
		//	*pwsText += list_iter->c_str();
		//}
		//*pwsText = strings::cut_all( *pwsText, L"/*", L"*/" );
		

		return true;
	}
	else // sHref - относительный путь к файлу на диске
	{
		CString sFullPath;

		// Сначала проверим, возможно sHref содержит полный путь к файлу
		if ( FileExist( &sHref ) )
			sFullPath = sHref;
		else
		{
			wchar_t wsDrive[MAX_PATH] = {0}, wsDir[MAX_PATH] = {0}, wsFileName[MAX_PATH] = {0}, wsExt[MAX_PATH] = {0};
			_wsplitpath( sPath.GetBuffer(), wsDrive, wsDir, wsFileName, wsExt );

			sFullPath = CString(wsDrive) + CString(wsDir) + sHref;
			if ( !FileExist( &sFullPath ) )
				return false;
		}

		CStringW sContent = NUnicodeTextFile::ReadFile (sFullPath);
		*pwsText += sContent.GetString();
		*pwsText = strings::cut_all( *pwsText, L"/*", L"*/" );
		

		return true;
	}
}

CString CHtmlToXhtmlSax::delComment(CString& style)
{
	std::wstring new_style;
	std::wstring old_style = style.GetString();
	new_style = strings::cut_all(old_style, L"<!--", L"-->");
	new_style = strings::cut_all(new_style, L"/*", L"*/");
	new_style = strings::cut_all(new_style, L"@", L";");
	return new_style.c_str();
}

_bstr_t CHtmlToXhtmlSax::FindAttribute (CHTMLTagWrapper* pHtmlNode, const _bstr_t bsAttrName, const _bstr_t bsDefaultValue)
{
    _bstr_t bsResult = bsDefaultValue;

	const TCHAR *sAttrName = (const TCHAR *) bsAttrName;

    const wchar_t *wsResult = pHtmlNode->get_attribute (sAttrName);

    if (wsResult)
    {
        bsResult = wsResult;
    }

    return bsResult;
}


_bstr_t CHtmlToXhtmlSax::getFilePath() const
{
	std::wstring path(m_sHtmlPath);
	size_t right_slash = path.find_last_of(L"/");
	if (std::wstring::npos == right_slash )
		right_slash = 0;
	size_t left_slash = path.find_last_of(L"\\");
	if (std::wstring::npos == left_slash )
		left_slash = 0;
	
	path = strings::substring_before(path, max(right_slash, left_slash)) + L"/";
	return path.c_str();
}


void CHtmlToXhtmlSax::RemoveInvalidChars(BSTR *pbsString)
{
	CStringW sString( *pbsString );
	::SysFreeString( *pbsString );

	int nCurPos = 0;
	while ( nCurPos < sString.GetLength() )
	{
		unsigned int unChar = sString.GetAt( nCurPos );

		// Допустимые юникодные значения: x0009, x000A, x000D, x0020-xD7FF, xE000-xFFFD, x10000-x10FFFF
		if ( 0x0009 == unChar || 0x000A == unChar || 0x000D == unChar || ( 0x0020 <= unChar && 0xD7FF >= unChar ) || ( 0xE000 <= unChar && unChar <= 0xFFFD ) || ( 0x10000 <= unChar && unChar  ) )
			nCurPos++;
		else
			sString.Delete( nCurPos );
	}
	*pbsString = sString.AllocSysString();
}

void CHtmlToXhtmlSax::HtmlTagsToText(CStringA *psString )
{
	CStringA sTemp = *psString;
	// Special characters for HTML

	sTemp.Replace( "\x3C", "&#60;" ); // <
	sTemp.Replace( "\x3E", "&#62;" ); // >

	// Заменяем специальные символы(перенос строки, табуляция и т.д.)
	sTemp.Replace( "\x00", "&#00;" );
	sTemp.Replace( "\x01", "&#01;" );
	sTemp.Replace( "\x02", "&#02;" );
	sTemp.Replace( "\x03", "&#03;" );
	sTemp.Replace( "\x04", "&#04;" );
	sTemp.Replace( "\x05", "&#05;" );
	sTemp.Replace( "\x06", "&#06;" );
	sTemp.Replace( "\x07", "&#07;" );
	sTemp.Replace( "\x08", "&#08;" );
	sTemp.Replace( "\x09", "&#09;" );
	sTemp.Replace( "\x0A", "&#10;" );
	sTemp.Replace( "\x0B", "&#11;" );
	sTemp.Replace( "\x0C", "&#12;" );
	sTemp.Replace( "\x0D", "&#13;" );
	sTemp.Replace( "\x0E", "&#14;" );
	sTemp.Replace( "\x0F", "&#15;" );
	sTemp.Replace( "\x10", "&#16;" );
	sTemp.Replace( "\x11", "&#17;" );
	sTemp.Replace( "\x12", "&#18;" );
	sTemp.Replace( "\x13", "&#19;" );
	sTemp.Replace( "\x14", "&#20;" );
	sTemp.Replace( "\x15", "&#21;" );
	sTemp.Replace( "\x16", "&#22;" );
	sTemp.Replace( "\x17", "&#23;" );
	sTemp.Replace( "\x18", "&#24;" );
	sTemp.Replace( "\x19", "&#25;" );
	sTemp.Replace( "\x1A", "&#26;" );
	sTemp.Replace( "\x1B", "&#27;" );
	sTemp.Replace( "\x1C", "&#28;" );
	sTemp.Replace( "\x1D", "&#29;" );
	sTemp.Replace( "\x1E", "&#30;" );
	sTemp.Replace( "\x1F", "&#31;" );
	sTemp.Replace( "\x20", "&#32;" );
	sTemp.Replace( "\x7F", "&#127;" );

	*psString = sTemp;
}


int CHtmlToXhtmlSax::filter ( unsigned int code, struct _EXCEPTION_POINTERS *ep )
{
   if ( code == EXCEPTION_ACCESS_VIOLATION )
      return EXCEPTION_EXECUTE_HANDLER;
   else
      return EXCEPTION_CONTINUE_SEARCH; 
}

///

///


// Get root node of the document
MSXML2::IXMLDOMNodePtr CHtmlToXhtmlSax::getDoc()
{
	return m_pXmlDoc;
}

// Get html node of the document
MSXML2::IXMLDOMNodePtr CHtmlToXhtmlSax::getHtml()
{
	MSXML2::IXMLDOMNodeListPtr html;
	HRESULT hRes = m_pXmlDoc->raw_getElementsByTagName(L"html", &html);
	
	return html->nextNode();
}

// Get body node of the document
MSXML2::IXMLDOMNodePtr CHtmlToXhtmlSax::getBody()
{
	MSXML2::IXMLDOMNodeListPtr body;
	HRESULT hRes = m_pXmlDoc->raw_getElementsByTagName(L"body", &body);
	
	return body->nextNode();
}

// convert html to xml
bool CHtmlToXhtmlSax::Convert ()
{
	bool bRes = false;
	if (m_bMht)
	{
		CStringW wsMhtFileString = m_oMhtFile.getHtmlFile().c_str();
		bRes = S_OK == m_oConverter.ToXHTML (wsMhtFileString, this);
	}
	else
	{
		HANDLE hInputFile = ::CreateFile (m_sHtmlPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hInputFile)
		{
			ATLTRACE2 ("Can't open file!! (last error: 0x%x)", GetLastError());
			return false;	
		}

		// Create converter object
		bRes = S_OK == m_oConverter.ToXHTML (hInputFile, this, m_nDefaultCodePage);

		CloseHandle (hInputFile);
	}
	return bRes;
}

BOOL CHtmlToXhtmlSax::SaveXML ()
{
	BSTR bsXml = NULL;
	BOOL bRes = TRUE;

	try
	{
		if (m_sXhtmlPath.IsEmpty ())
			throw (std::string) "CHtmlToXhtmlSax::SaveXML (): invalid output folder";

		// delete invalid unicode characters
		if (FAILED (m_pXmlDoc->get_xml (&bsXml)))
			throw (std::string) "CHtmlToXhtmlSax::SaveXML (): failed to get xmlstring from msxml";

		// there are some htmls with invalid unicode symbols
		RemoveInvalidChars (&bsXml);

		VARIANT_BOOL vbSuccess = VARIANT_FALSE;

		// reload xml (test)
		if (FAILED (m_pXmlDoc->raw_loadXML (bsXml, &vbSuccess)) || VARIANT_TRUE != vbSuccess)
			throw (std::string) "CHtmlToXhtmlSax::SaveXML (): failed to load xmlstring to msxml";

		if (FAILED (m_pXmlDoc->raw_save ( (const _variant_t &) m_sXhtmlPath.GetBuffer())))
			throw (std::string) "CHtmlToXhtmlSax::SaveXML (): failed to load xmlstring to msxml";   

	}
	catch (std::string sErr)
	{
		ATLTRACE2 (sErr.c_str());
		bRes = FALSE;
	}
	if (NULL != bsXml)
		::SysFreeString (bsXml);

	return bRes;
}

// Event handlers from a SAX parser
// IXHTMLWriter implementation
BOOL CHtmlToXhtmlSax::BeginFile ()
{
	// Create body and frameset element
	m_pXmlHtmlNodePtr =		m_pXmlDoc->createNode (MSXML2::NODE_ELEMENT, L"html",  L"" );		// html
	m_pXmlBodyNodePtr =		m_pXmlDoc->createNode (MSXML2::NODE_ELEMENT, L"body",  L"" );		// body
	m_pXmlHeadNodePtr =		m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"head",  L"" );		// head
	m_pXmlTitleNodePtr =	m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"title", L"" );		// title
	m_pXmlStyleNodePtr =	m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"style", L"" );		// style

	return TRUE;	
}
BOOL CHtmlToXhtmlSax::EndFile ()
{
	// fill title and styles
	m_pXmlStyleNodePtr->put_text (m_bsStyle);
	m_pXmlTitleNodePtr->put_text (m_bsTitle);

	// write nodes to doc
	m_pXmlHeadNodePtr->appendChild (m_pXmlTitleNodePtr);
	m_pXmlHtmlNodePtr->appendChild (m_pXmlHeadNodePtr);

	if (NULL != m_pXmlFramesetNodePtr)
	{
		m_pXmlHtmlNodePtr->appendChild (m_pXmlFramesetNodePtr);
	}
	else
	{
		m_pXmlHtmlNodePtr->appendChild (m_pXmlBodyNodePtr);
	}

	m_pXmlDoc->appendChild (m_pXmlHtmlNodePtr);

	return TRUE;
}

// Start tag (event from SAX parser)
BOOL CHtmlToXhtmlSax::StartTag (CHTMLTagWrapper *pTagWrapper, CHTMLTagWrapper *pPrevTagWrapper)
{
	if (NULL == pTagWrapper)
			return FALSE;

		BOOL bRes = TRUE;

		try
		{
			CLiteHTMLTag *pTag = pTagWrapper->Tag();
			CString sTagName;
			if (NULL != pTag)
			{
			
				//ATLTRACE2 ("CHtmlToXhtmlSax::StartTag(): tagname = %s\n", pTag->getTagName());
				sTagName = pTag->getTagName();
				
				// edit tagname
				sTagName.Remove(':');	// on 'g:plusone' msxml fails
				sTagName = sTagName.MakeLower();
				if (sTagName == _T("noscript") || sTagName == _T("marquee") || sTagName == _T("blink"))
					sTagName = _T("div");

				// create new tag node
				BSTR bstrTagName = sTagName.AllocSysString ();
				pTagWrapper->pXmlTagNode = m_pXmlDoc->createNode (MSXML2::NODE_ELEMENT, bstrTagName, L"");
				SysFreeString (bstrTagName);
			}
			
			// close last tag prefix
			if (NULL != pPrevTagWrapper)
			{
				// handle all unprocessed texts
				if (pPrevTagWrapper->isUnprocessedTextExists())
				{
					Characters (pPrevTagWrapper, pPrevTagWrapper->getUnprocessedText(), false);
					pPrevTagWrapper->clearUnprocessedText ();
				}

				// close prefix in prev tag or close tag
				DWORD dwState = pPrevTagWrapper->State ();
				dwState |= TAG_STATE_HAS_CHILDTAG;
				pPrevTagWrapper->State (dwState);
				CloseTagPrefix (pPrevTagWrapper);

				if (NULL != pPrevTagWrapper->pXmlTagNode)
				{
					MSXML2::IXMLDOMNodePtr pParentNodePtr = pPrevTagWrapper->pXmlTagNode;
					BSTR bstrName;
					pParentNodePtr->get_baseName(&bstrName);
					SysFreeString (bstrName);

					if (sTagName != _T("style"))
					{
						// filter unvisible tags
						pParentNodePtr->appendChild (pTagWrapper->pXmlTagNode);
					}
				}			
			}

			if (NULL == pTag)
			{
				return FALSE;
			}

			DWORD dwState = TAG_STATE_OPENED;
			
			// write tag prefix
			CString sTagPrefix = _T("<");
			sTagPrefix += pTag->getTagName();
			//bRes &= WriteString (sTagPrefix, _tcslen (sTagPrefix));

			// write attributes
			CLiteHTMLAttributes* pAttrs = pTag->getAttributes();
			if (NULL != pAttrs)
			{
				const int iAttrCount = pAttrs->getCount();
				if (iAttrCount > 0)
				{
					sTagPrefix = _T(" ");
					//bRes &= WriteString (sTagPrefix, _tcslen (sTagPrefix));
					dwState |= TAG_STATE_HAS_ATTR;
				}
				for (int iAttr = 0; iAttr < iAttrCount; iAttr++)
				{
					// write attr
					CLiteHTMLElemAttr& oAttr = pAttrs->getAttribute(iAttr);
					CString& sAttrName = oAttr.getName();
					CString& sAttrValue = oAttr.getValue ();

					CString sAttrPair = sAttrName;
					sAttrPair += _T("=\"");
					sAttrPair += sAttrValue;
					sAttrPair += _T("\" ");
					//bRes &= WriteString (sAttrPair, _tcslen (sAttrPair));			

					// 
				}
			}
			pTagWrapper->State (dwState);

			

			// parse styles
#ifdef _DEBUG
			if (0 == sTagName.CompareNoCase(_T("a")))
			{
				ATLTRACE2 ("");
			}
#endif

			if ( ! (0 == sTagName.CompareNoCase(_T("meta"))
				|| 0 == sTagName.CompareNoCase(_T("style"))
				|| 0 == sTagName.CompareNoCase(_T("link"))
				|| 0 == sTagName.CompareNoCase(_T("head"))
				|| 0 == sTagName.CompareNoCase(_T("title"))
				))
			{
				//CString sParentStyle = (NULL == pPrevTagWrapper) ? _T("") : pPrevTagWrapper->getStyle().ToString();	// parent style
				CString sExternalStyle = m_oCssList.get_ElementCSS (pTagWrapper);										// external css
				CString sTagStyle = CLiteHTMLElemStyleAttr::GetStyleString (pAttrs);									// style from 'style' attribute

				CLiteHTMLElemStyleAttr &oTagStyle = pTagWrapper->getStyle();
				CString sMergedStyle = /*sParentStyle + */sExternalStyle + sTagStyle;

				// replacement of '! important' string
				sMergedStyle.Remove('!');
				sMergedStyle.Replace(_T("important"), _T(""));
				

				oTagStyle.AddStyleString (sMergedStyle);

				double dParentFontSize = (NULL == pPrevTagWrapper) ? 16.0 : pPrevTagWrapper->getFontSize();

				// convert relative text-size into real
				if (-1 != sMergedStyle.Find (_T("font")))
				{
					CString sTextSize = oTagStyle.getAttribute (_T("font-size"));
					sTextSize.Remove (' ');
					if (!sTextSize.IsEmpty())
					{
						double dFontSize = oTagStyle.ConvertFontSizeToPx (sTextSize, dParentFontSize);
						if (0 == dFontSize)
							dFontSize = 16.0; // default
						oTagStyle.setAttribute (_T("font-size"), sTextSize);
						pTagWrapper->setFontSize (dFontSize);
					}
					else
					{
						pTagWrapper->setFontSize (dParentFontSize);
					}

					// has 'font-size' and other tags
					oTagStyle.ConvertAttributesToPx (dParentFontSize);
				}
				else
				{
					/*
					// if font-size is not presents explicitly, setup it for some tags:
					if (0 == sTagName.CompareNoCase(_T("sup")))
					{
						CString sSmaller = _T("smaller");
						double dFontSize = oTagStyle.ConvertFontSizeToPx (sSmaller, dParentFontSize);
						if (0 == dFontSize)
							dFontSize = 16.0; // default
						oTagStyle.setAttribute (_T("font-size"), sSmaller);
						pTagWrapper->setFontSize (dFontSize);
					}
					else
					*/
					{
						// store font size for next generations :)
						pTagWrapper->setFontSize (dParentFontSize);
					}
				}
			}
		}
		catch (std::exception ex)
		{
			ATLTRACE2 ("CHtmlToXhtmlSax::StartTag() exception: %s\n", ex.what());
			bRes = FALSE;
		}
		return bRes;
}


// End tag (event from SAX parser)
BOOL CHtmlToXhtmlSax::EndTag (CHTMLTagWrapper *pTagWrapper)
{
	if (NULL == pTagWrapper)
			return FALSE;

	BOOL bRes = TRUE;

	// handle all unprocessed texts
	if (pTagWrapper->isUnprocessedTextExists())
	{
		Characters (pTagWrapper, pTagWrapper->getUnprocessedText(), false);
		pTagWrapper->clearUnprocessedText ();
	}
		

	// close tag
	bRes &= CloseTagPrefix (pTagWrapper, true);

	ProcessTag (pTagWrapper);
	return bRes;
}

// Characters (event from SAX parser)
void CHtmlToXhtmlSax::Characters (CHTMLTagWrapper *pTagWrapper, CString aText, bool aBeforeLastChild)
{
	if (NULL == pTagWrapper)
		return;
		
	DWORD dwState = pTagWrapper->State ();
	dwState |= TAG_STATE_HAS_TEXTS;
	pTagWrapper->State (dwState);

	CloseTagPrefix (pTagWrapper);

	// write text
	if (!aText.IsEmpty() && NULL != pTagWrapper->pXmlTagNode)	// pTagWrapper - parent tag
	{
		PreprocessText (pTagWrapper, aText, aBeforeLastChild);
	}
}

// Helper. it closes starttag prefix or closes whole tag
BOOL CHtmlToXhtmlSax::CloseTagPrefix (CHTMLTagWrapper *pPrevTagWrapper, bool aCloseTag)
{
	DWORD dwState = pPrevTagWrapper->State ();
	
	BOOL bRes = TRUE;

	// a tag is already closed
	if (TAG_STATE_CLOSED & dwState)
		return TRUE;

	// a tag has child tags - we must only close a prefix
	if (TAG_STATE_PREFIX_CLOSED & dwState)
	{
	}
	else
	{
		if ((TAG_STATE_HAS_CHILDTAG & dwState) || (TAG_STATE_HAS_TEXTS & dwState))
		{
			// close prefix (">")
			dwState |= TAG_STATE_PREFIX_CLOSED;
			pPrevTagWrapper->State (dwState);
			//bRes &= WriteString (pcTagEnd, _tcslen (pcTagEnd));
		}
	}

	// We need to close tag
	if (aCloseTag)
	{
		if ((TAG_STATE_HAS_CHILDTAG & dwState) || (TAG_STATE_HAS_TEXTS & dwState))
		{
			// write close tag
			//CString sTagPostFix = _T("</");
			//sTagPostFix += pPrevTagWrapper->Tag()->getTagName();
			//sTagPostFix += pcTagEnd;

			//bRes &= WriteString (sTagPostFix, _tcslen (sTagPostFix));
			dwState |= TAG_STATE_CLOSED;
		}
		else 
		{
			// inline close
			//CString sTagPostFix = _T("/>");
			//bRes &= WriteString (sTagPostFix, _tcslen (sTagPostFix));
			dwState |= TAG_STATE_CLOSED;
		}
	}


	// 
	return TRUE;
}


void CHtmlToXhtmlSax::SetOutput (CString &aOut)
{
}

DWORD CHtmlToXhtmlSax::ErrorCode ()
{
	return 0;//m_dwErrorCode;
}

BOOL CHtmlToXhtmlSax::PreprocessText (CHTMLTagWrapper *pTagWrapper, const CString aText, bool aBeforeLastChild)
{
	// delimit by newline symbol

	CString sText = aText;

	// create text node
	BSTR bstrText = sText.AllocSysString ();
	MSXML2::IXMLDOMTextPtr pTextPtr = m_pXmlDoc->createTextNode(bstrText);
	SysFreeString (bstrText);

	if (!aBeforeLastChild)
	{
		pTagWrapper->pXmlTagNode->appendChild (pTextPtr);
	}
	else
	{
		/*
		CHTMLTagWrapper *pParent = pTagWrapper->getParentTag();
		int nTagIndex = pParent->GetChildIndex (pTagWrapper) - 1;
		CHTMLTagWrapper *pBranch = pParent->GetChildAt (nTagIndex);
		if (NULL == pBranch ||  0 == pBranch->Tag()->getTextsCount())
		{
			pTagWrapper->pXmlTagNode->appendChild (pTextPtr);
		}
		else
		{
			pBranch->pXmlTagNode->appendChild (pTextPtr);
		}*/
		
		/*		
		// find sibling element with text
		CHTMLTagWrapper *pParent = pTagWrapper->getParentTag();
		int nTagIndex = pParent->GetChildIndex (pTagWrapper) - 1;
		CHTMLTagWrapper *pBranch = NULL;	// ptr to closest previous sibling tag (NULL if doesn't exists)

		bool bNewLine = false;
		bool bFound = false;

		while (NULL != pParent)
		{	
			pBranch = pParent->GetChildAt (nTagIndex);
			while (NULL != pBranch)
			{
				// check branch
				pBranch = pParent->GetChildAt (nTagIndex);

				if ((NULL != pBranch) && (0 != pBranch->Tag()->getTextsCount()))
				{
					// ok, parent has texts
					bFound = true;
					break;
				}

				// check line break
				bNewLine = CHTMLTagWrapper::IsNewLineStarting (pBranch);
				if (bNewLine)
					break;

				nTagIndex--;
			}

			// check parent
			if (0 != pParent->Tag()->getTextsCount())
			{
				// ok, parent has texts
				pBranch = pParent;
				bFound = true;
				break;
			}

			if (bNewLine || CHTMLTagWrapper::IsNewLineStarting (pParent))
			{
				break;
			}
			pParent = pParent->getParentTag();
			nTagIndex = 0;
		}

		if (bFound && (NULL != pBranch))
		{
			// add to the branch
			// TODO: find last text node
			pBranch->pXmlTagNode->appendChild (pTextPtr);
		}
		else
		{
			//we doesn't found branch :(. Add text to that tag
			pTagWrapper->pXmlTagNode->appendChild (pTextPtr);
		}
		*/
		


		
		MSXML2::IXMLDOMNodePtr pLastChild = pTagWrapper->pXmlTagNode->GetlastChild ();
		if (NULL != pLastChild)
		{
			CComVariant vLastChild = pLastChild.GetInterfacePtr ();

			// create new span element
			MSXML2::IXMLDOMElementPtr pSpan = m_pXmlDoc->createElement(_bstr_t (L"span"));
			pSpan->appendChild (pTextPtr);
			pTagWrapper->pXmlTagNode->insertBefore (pSpan, vLastChild);

			//pTagWrapper->pXmlTagNode->insertBefore (pTextPtr, vLastChild);
		}
		else
		{
			pTagWrapper->pXmlTagNode->appendChild (pTextPtr);
		}
		
	}
	return TRUE;
}

BOOL CHtmlToXhtmlSax::ProcessTag (CHTMLTagWrapper *pTagWrapper)
{
	ATLASSERT (pTagWrapper);
	if (NULL == pTagWrapper)
		return FALSE;
	
	CLiteHTMLTag* pLiteTag = pTagWrapper->Tag();
	ATLASSERT (pLiteTag);

	if (NULL == pLiteTag)
		return FALSE;

	CString sTagName = pLiteTag->getTagName();
	CString	sStyleCSS;
	CString sTagStyle;
	BOOL bVisibleElement = FALSE;
	
	// impl2: map: tag_name->tag_handles
	if (0 == sTagName.CompareNoCase (_T("title")))	// 
	{
		// store title and ignore node
		size_t nTextCount = pLiteTag->getTextsCount();
		for (size_t nText = 0; nText < nTextCount; nText++)
		{
			// merge text
			m_bsTitle += pLiteTag->getTextAt (nText);
		}
	}
	else if (0 == sTagName.CompareNoCase ( _T("meta")))	// meta
	{
	}
	else if (0 == sTagName.CompareNoCase ( _T("style")))	// styles
	{
		// store style and ignore node
		size_t nTextsCount = pLiteTag->getTextsCount();
		for (size_t nText = 0; nText < nTextsCount; nText++)
		{
			sStyleCSS += pLiteTag->getTextAt(nText);
		}
		// style parse
		if (!sStyleCSS.IsEmpty())
		{
			sStyleCSS = delComment (sStyleCSS);
			sStyleCSS = loadCSS (sStyleCSS).c_str();
				//strings::make_lower( style );
			sStyleCSS = strings::cut_all (sStyleCSS.GetString(), L"/*", L"*/").c_str();
			sStyleCSS = strings::cut_all (sStyleCSS.GetString(), L"<!--", L"-->").c_str();
				
			m_oCssList.Parse (sStyleCSS);
			m_bsStyle += sStyleCSS;
		}
	}
	else if (0 == sTagName.CompareNoCase (_T("link")))	// links
	{
		// 
		CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();

		if (NULL != pAttrs)
		{
			CString sRelAttrValue = pAttrs->getValueFromName (_T("rel"));
			CString sTypeAttrValue = pAttrs->getValueFromName (_T("type"));
			sRelAttrValue.MakeLower();
			sTypeAttrValue.MakeLower();
			
			if (_T("stylesheet") == sRelAttrValue)
			{
				CString sHrefAttrValue = pAttrs->getValueFromName (_T("href"));
				HtmlUtils::DecodeUrl (sHrefAttrValue);
				CString sPath (m_sHtmlPath);
				
				if (!m_bMht)
				{
					if (!sHrefAttrValue.IsEmpty() && (-1 != sHrefAttrValue.Find( _T("http") ) || -1 != sHrefAttrValue.Find( _T("ftp") ) ) )
					{
						std::wstring wsBuffer;
						if (LoadExternalCSS (sHrefAttrValue, &wsBuffer, sPath, TRUE))
						{
							sStyleCSS += wsBuffer.c_str();
						}
					}
					else if (!sHrefAttrValue.IsEmpty())
					{
						std::wstring wsBuffer;
						if ( LoadExternalCSS (sHrefAttrValue, &wsBuffer, sPath, FALSE) )
						{
							sStyleCSS += wsBuffer.c_str();
						}
					}
				}
				else
				{
					sStyleCSS += m_oMhtFile.getStyle(sHrefAttrValue.GetString()).c_str();
				}

				// style parse
				if (!sStyleCSS.IsEmpty())
				{
					sStyleCSS = delComment (sStyleCSS);
					sStyleCSS = loadCSS (sStyleCSS).c_str();
						//strings::make_lower( style );
					sStyleCSS = strings::cut_all (sStyleCSS.GetString(), L"/*", L"*/").c_str();
					sStyleCSS = strings::cut_all (sStyleCSS.GetString(), L"<!--", L"-->").c_str();
						
					m_oCssList.Parse (sStyleCSS);
					m_bsStyle += sStyleCSS;
				}
			}
		}
	}
	else if (0 == sTagName.CompareNoCase (_T("img")))
	{
		// get styles from list
		// get style from loaded css
		PreprocessImg (pTagWrapper, sTagStyle);	
	}
	else if (0 == sTagName.CompareNoCase (_T("a")))
	{
		PreprocessA (pTagWrapper, sTagStyle);	
	}
	else if (0 == sTagName.CompareNoCase (_T("p")))
	{
		PreprocessP (pTagWrapper, sTagStyle);	
	}
	else if (0 == sTagName.CompareNoCase (_T("table")))
	{
		PreprocessTable (pTagWrapper, sTagStyle);	
	}
	else if (0 == sTagName.CompareNoCase (_T("tr")))
	{
		PreprocessTable (pTagWrapper, sTagStyle);	
	}
	else if (0 == sTagName.CompareNoCase (_T("td")))
	{
		PreprocessTable (pTagWrapper, sTagStyle);	
	}
	else if (0 == sTagName.CompareNoCase (_T("body")))	// body node
	{
		// get style from loaded css
		PreprocessBody (pTagWrapper, sTagStyle);
		//if (NULL == m_pXmlBodyNodePtr)
			m_pXmlBodyNodePtr = pTagWrapper->pXmlTagNode;
	}
	else if (0 == sTagName.CompareNoCase (_T("frameset")))
	{
		PreprocessFrameset (pTagWrapper, sTagStyle);
		m_pXmlFramesetNodePtr = pTagWrapper->pXmlTagNode;
	}
	else if (0 == sTagName.CompareNoCase (_T("textarea")))
	{
		PreprocessTextarea (pTagWrapper, sTagStyle);
	}
	else if (0 == sTagName.CompareNoCase (_T("script")))
	{
		// skip script
	}
	else if (0 == sTagName.CompareNoCase (_T("colgroup")))
	{
		// skip colgroup
	}
	else if (0 == sTagName.CompareNoCase (_T("col")))
	{
		// skip col
	}
	else if (0 == sTagName.CompareNoCase (_T("frame")))
	{
		// frame
		PreprocessFrame (pTagWrapper, sTagStyle);
	}
	else if (0 == sTagName.CompareNoCase (_T("iframe")))
	{
		// iframe
		PreprocessFrame (pTagWrapper, sTagStyle);
	}
	else if (0 == sTagName.CompareNoCase (_T("font")))
	{
		// font
		PreprocessFontTag (pTagWrapper, sTagStyle);
	}
	else if (0 == sTagName.CompareNoCase (_T("p")))
	{
		// p
		PreprocessDefaultTag (pTagWrapper, sTagStyle);
	}
	else
	{
		// other tags
		PreprocessDefaultTag (pTagWrapper, sTagStyle);
	}

	// rework tag style string
	if (!sTagStyle.IsEmpty())
	{
		//CLiteHTMLElemStyleAttr oStyle (sTagStyle);

		sTagStyle.Replace ('\"', '\'');
		
		if (!sTagStyle.IsEmpty())
		{
			MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;

			if (NULL != xml_element_ptr)
			{
				BSTR bstrStyle = sTagStyle.AllocSysString();
				xml_element_ptr->setAttribute( _bstr_t( L"style" ), (const _variant_t &) bstrStyle);
				
				SysFreeString (bstrStyle);
			}
		}
	}
	return TRUE;	
}

// preprocess frame/iframe
BOOL CHtmlToXhtmlSax::PreprocessFrame (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	

	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;

	// iframe has no children - delete all children
	MSXML2::IXMLDOMNodePtr pFrameChild;
	do
	{
		xml_element_ptr->get_firstChild (&pFrameChild);
		if (NULL != pFrameChild)
		{
			xml_element_ptr->removeChild (pFrameChild);
			pFrameChild = NULL;
		}		
		else break;
	}
	while (true);

	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();

	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}


	// parse src attribute
	const int nSrcAttrIndex = pAttrs->getIndexFromName (_T("src"));

	if (-1 != nSrcAttrIndex)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute(nSrcAttrIndex);
		CString sValue = oAttr.getValue();

		HtmlUtils::DecodeUrl (sValue);
		std::wstring wsResultPath = _T("");
		
		// !!!! MHT has no m_sHtmlPath !!!!
		std::wstring wsFromRelativePath = full_from_relative ((const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

		if (m_bMht)
		{
			std::wstring frame_name = (const std::wstring) sValue.GetBuffer();
			std::wstring frame_content = m_oMhtFile.getFrame (frame_name);
			if (!frame_content.empty())
			{
				MhtFile frameMht = m_oMhtFile;

				// change frame and main content
				frameMht.removeFrameFile (frame_name);
				frameMht.setHtmlFile (frame_name, frame_content);

				CHtmlToXhtmlSax oHtmlToXml (L"", L"", &frameMht);
				bool bResult = oHtmlToXml.Convert ();
				if (bResult == true)
				{
					MSXML2::IXMLDOMNode *pTempNode = NULL;
					MSXML2::IXMLDOMNodePtr pFrameBody = oHtmlToXml.getBody();
					//MSXML2::IXMLDOMNodePtr pFrameBody = oHtmlToXml.getHtml();
					
					#ifdef _DEBUG
					BSTR bstrFrameBodyXML;
					pFrameBody->get_xml (&bstrFrameBodyXML);
					SysFreeString (bstrFrameBodyXML);
					#endif

					xml_element_ptr->raw_appendChild (pFrameBody, &pTempNode);
					AddTempImages (oHtmlToXml.GetTempImages());
					RELEASEINTERFACE (pTempNode);
				}
			}
		}
		else if (  -1 == sValue.Find( _T("www") ) && -1 == sValue.Find( _T("http") ) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) )
		{
			if ( (-1 != sValue.Find( _T(".htm") )) || (-1 != sValue.Find (_T(".html"))) || (-1 != sValue.Find (_T(".aspx"))))
			{
				CHtmlToXhtmlSax oHtmlToXml (wsFromRelativePath.c_str(), L"", NULL);

				bool bResult = oHtmlToXml.Convert ();
				if (bResult == true)
				{
					MSXML2::IXMLDOMNode *pTempNode = NULL;
					MSXML2::IXMLDOMNodePtr pFrameBody = oHtmlToXml.getBody();
					//MSXML2::IXMLDOMNodePtr pFrameBody = oHtmlToXml.getHtml();
					
					#ifdef _DEBUG
					BSTR bstrFrameBodyXML;
					pFrameBody->get_xml (&bstrFrameBodyXML);
					SysFreeString (bstrFrameBodyXML);
					#endif

					xml_element_ptr->raw_appendChild (pFrameBody, &pTempNode);
					AddTempImages (oHtmlToXml.GetTempImages());
					RELEASEINTERFACE (pTempNode);
				}
			}
		}
		else if (m_bMht)
		{
			/*
			std::wstring frame_name = (const std::wstring) sValue.GetBuffer();
			std::wstring frame_content = m_oMhtFile.getFrame (frame_name);
			if (!frame_content.empty())
			{
				MhtFile frameMht = m_oMhtFile;

				// change frame and main content
				frameMht.removeFrameFile (frame_name);
				frameMht.setHtmlFile (frame_name, frame_content);

				CHtmlToXhtmlSax oHtmlToXml (L"", L"", &frameMht);
				bool bResult = oHtmlToXml.Convert ();
				if (bResult == true)
				{
					MSXML2::IXMLDOMNode *pTempNode = NULL;
					MSXML2::IXMLDOMNodePtr pFrameBody = oHtmlToXml.getBody();
					//MSXML2::IXMLDOMNodePtr pFrameBody = oHtmlToXml.getHtml();
					
					#ifdef _DEBUG
					BSTR bstrFrameBodyXML;
					pFrameBody->get_xml (&bstrFrameBodyXML);
					SysFreeString (bstrFrameBodyXML);
					#endif

					xml_element_ptr->raw_appendChild (pFrameBody, &pTempNode);
					AddTempImages (oHtmlToXml.GetTempImages());
					RELEASEINTERFACE (pTempNode);
				}
			}
			*/
		}
	}

	
	// check all other attributes
	const int nAttrCount = pAttrs->getCount();
	for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
		
		const CString sName = oAttr.getName();
		if (sName != _T("style") && sName != _T("src")
			&& sName != _T("id") && sName != _T("class") && sName != _T("xmlns"))
		{
			AddAttributesToNode (oAttr, xml_element_ptr);
		}
	}
	

	sStyle = oStyle.ToString();

	return TRUE;
}

// preprocess default tag
BOOL CHtmlToXhtmlSax::PreprocessDefaultTag (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;


	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}

	// check all other attributes
	const int nAttrCount = pAttrs->getCount();
	for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
		
		const CString sName = oAttr.getName();
		if (sName != _T("style") 
			&& sName != _T("id") 
			&& sName != _T("class") 
			&& sName != _T("xmlns"))
		{
			AddAttributesToNode (oAttr, xml_element_ptr);
		}
	}
	
	sStyle = oStyle.ToString();
	return TRUE;
}

// preprocess Table-tag
BOOL CHtmlToXhtmlSax::PreprocessTable (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;


	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}

	CString sAlignValue = pAttrs->getValueFromName (_T("align"));
	if (!sAlignValue.IsEmpty())
	{
		CString sStyle = _T("text-align:");
		sStyle += sAlignValue.MakeLower();
		oStyle.AddStyleString (sStyle);
	}

	int nBgColorAttrIndex = pAttrs->getIndexFromName (_T("bgcolor"));
	if (-1 != nBgColorAttrIndex)
	{
		CLiteHTMLElemAttr oBgColorValue = pAttrs->getAttribute (nBgColorAttrIndex);
		CString sStyle = _T("background-color:");
		
		sStyle += oBgColorValue.getColorHexValue();
		oStyle.AddStyleString (sStyle);
	}

	CString sWidthValue = pAttrs->getValueFromName (_T("width"));
	if (!sWidthValue.IsEmpty())
	{
		CString sStyle = _T("width:");
		sStyle += sWidthValue.MakeLower();
		oStyle.AddStyleString (sStyle);
	}

	CString sHeightValue = pAttrs->getValueFromName (_T("height"));
	if (!sHeightValue.IsEmpty())
	{
		CString sStyle = _T("height:");
		sStyle += sHeightValue.MakeLower();
		oStyle.AddStyleString (sStyle);
	}
	CString sBorderValue = pAttrs->getValueFromName (_T("border"));
	if (!sBorderValue.IsEmpty())
	{
		CString sStyle = _T("border:");
		sStyle += sBorderValue.MakeLower();
		oStyle.AddStyleString (sStyle);
	}

	// check all other attributes
	const int nAttrCount = pAttrs->getCount();
	for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
		
		const CString sName = oAttr.getName();
		if (sName != _T("style") 
			&& sName != _T("id") 
			&& sName != _T("align") 
			&& sName != _T("bgcolor") 
			&& sName != _T("width") 
			&& sName != _T("class") 
			&& sName != _T("xmlns"))
		{
			AddAttributesToNode (oAttr, xml_element_ptr);
		}
	}
	
	sStyle = oStyle.ToString();
	return TRUE;
}

// preprocess p-tag
BOOL CHtmlToXhtmlSax::PreprocessP (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;


	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}

	CString sAlignValue = pAttrs->getValueFromName (_T("align"));
	if (!sAlignValue.IsEmpty())
	{
		CString sAlignStyle = _T("text-align:");
		sAlignStyle += sAlignValue.MakeLower();
		oStyle.AddStyleString (sAlignStyle);
	}

	// check all other attributes
	const int nAttrCount = pAttrs->getCount();
	for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
		
		const CString sName = oAttr.getName();
		if (sName != _T("style") 
			&& sName != _T("id") 
			&& sName != _T("align") 
			&& sName != _T("class") 
			&& sName != _T("xmlns"))
		{
			AddAttributesToNode (oAttr, xml_element_ptr);
		}
	}
	
	sStyle = oStyle.ToString();
	return TRUE;
}

// preprocess <frameset> tag
BOOL CHtmlToXhtmlSax::PreprocessFrameset (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;

	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}

	// check all other attributes
	const int nAttrCount = pAttrs->getCount();
	for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
		
		const CString sName = oAttr.getName();
		if (sName != _T("style") && sName != _T("id") && sName != _T("class") && sName != _T("xmlns"))
		{
			AddAttributesToNode (oAttr, xml_element_ptr);
		}
	}
	sStyle = oStyle.ToString();

	return TRUE;
}
// preprocess <body> tag
BOOL CHtmlToXhtmlSax::PreprocessBody (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;


	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}

		// process body background image
		CString sBgImage = oStyle.getAttribute (_T("background-image"));
		const int nImageNameQuoteStart = sBgImage.Find ('(');
		if (-1 != nImageNameQuoteStart)
		{
			const int nImageNameQuoteEnd = sBgImage.Find (')', nImageNameQuoteStart);
			if (-1 != nImageNameQuoteEnd)
			{
				sBgImage = sBgImage.Mid (nImageNameQuoteStart + 1, nImageNameQuoteEnd - nImageNameQuoteStart - 1);
				// delete quotas
				sBgImage.Remove('\'');
				sBgImage.Remove('\"');
			}
		}
		
		// cut " and ' symbols
		CString sBgImageName = sBgImage;

		if (!sBgImage.IsEmpty())
		{

			if (false == m_bMht)
			{
				HtmlUtils::DecodeUrl (sBgImage);
				
				std::wstring wsResultPath = _T("");
				std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sBgImage.GetBuffer() );

				// Путь к файлу может быть либо полным, либо относительным, либо ссылкой на скачивание в интернете
				if (FileExist(&sBgImage))
				{
					wsResultPath = std::wstring (sBgImage.GetBuffer());
				}
				else if (FileExist( &CString( wsFromRelativePath.c_str())))
				{
					wsResultPath = wsFromRelativePath;
				}
				else
				{
					CString sPath = sBgImage;
					if ( m_bDownloadImages && ( -1 != sPath.Find( _T("www") ) || -1 != sPath.Find( _T("http") ) || -1 != sPath.Find( _T("ftp") ) ) ) 
					{
						CFileDownloader oDownloader( sPath, FALSE );
						oDownloader.StartWork( 1 );
						while ( oDownloader.IsRunned() )
						{
							::Sleep( 10 );
						}

						if ( oDownloader.IsFileDownloaded() )
						{
							wsResultPath = std::wstring( oDownloader.GetFilePath().GetBuffer() );
							m_arrImageFileName.Add( oDownloader.GetFilePath() );
						}
						else
							wsResultPath = _T("");
					}
				}

				if (wsResultPath != L"")
				{
					CString sBgWidth = oStyle.getAttribute (_T("width"));
					CString sBgHeight = oStyle.getAttribute (_T("height"));

					int nWidth  = _ttoi (sBgWidth.GetBuffer());
					int nHeight = _ttoi (sBgHeight.GetBuffer());

					Gdiplus::GdiplusStartupInput  oInput;
					Gdiplus::GdiplusStartupOutput oOutput;
					ULONG dwToken = 0;
					Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
					if ((eStatus == Gdiplus::Ok) && (NULL != xml_element_ptr))
					{
						Gdiplus::Bitmap oImage( wsResultPath.c_str() );						
						setAttrs( nWidth, nHeight, oImage );

						BSTR bstrBgImgName = sBgImageName.AllocSysString();

						xml_element_ptr->setAttribute( _bstr_t( L"background-image" ), (const _variant_t &)bstrBgImgName);
						xml_element_ptr->setAttribute( _bstr_t( L"background-image-width" ), (const _variant_t &)nWidth );
						xml_element_ptr->setAttribute( _bstr_t( L"background-image-height" ), (const _variant_t &)nHeight );				

						SysFreeString (bstrBgImgName);
					}
					Gdiplus::GdiplusShutdown( dwToken );

					
				}
			}
			else
			{
				std::wstring strTempPath = CHtmlUtility::GetTempName();
				if (m_oMhtFile.savePicture ( (const std::wstring) sBgImage.GetBuffer(), strTempPath))
				{
					int nWidth = 0;
					int nHeight = 0;

					Gdiplus::GdiplusStartupInput  oInput;
					Gdiplus::GdiplusStartupOutput oOutput;
					ULONG dwToken = 0;
					Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
					if ((eStatus == Gdiplus::Ok) && (NULL != xml_element_ptr))
					{
						Gdiplus::Bitmap oImage( strTempPath.c_str() );
						setAttrs( nWidth, nHeight, oImage );
						//style += L"background-image:";
						//style += image;
						//style += L"; background-image-width:100";
						//style += nWidth
						//style += L"; background-image-height:100";
						//style += nHeight;
						//style += L";";
						BSTR bstrBgImgName = sBgImageName.AllocSysString();

						xml_element_ptr->setAttribute( _bstr_t( L"background-image" ), (const _variant_t &) bstrBgImgName);
						xml_element_ptr->setAttribute( _bstr_t( L"background-image-width" ), (const _variant_t &)nWidth );
						xml_element_ptr->setAttribute( _bstr_t( L"background-image-height" ), (const _variant_t &)nHeight );

						SysFreeString (bstrBgImgName);
					}
					Gdiplus::GdiplusShutdown( dwToken );
				}
			}
		}	// if image src exists

		sStyle = oStyle.ToString();

		// check all other attributes
		const int nAttrCount = pAttrs->getCount();
		for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
		{
			CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
			
			const CString sName = oAttr.getName();
			if (sName != _T("width") && sName != _T("height") && sName != _T("background-image") && sName != _T("background-image-width") && sName != _T("background-image-height")
				&& sName != _T("style") 
				&& sName != _T("id") && sName != _T("class") && sName != _T("xmlns"))
			{
				AddAttributesToNode (oAttr, xml_element_ptr);
			}
		}

	return TRUE;
}
// preprocess <a> tag
BOOL CHtmlToXhtmlSax::PreprocessA (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();

	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;

	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}


	// parse href & name sattribute
	const int nHrefAttrIndex = pAttrs->getIndexFromName(_T("href"));
	const int nNameAttrIndex = pAttrs->getIndexFromName(_T("name"));

	if (false == m_bMht)
	{
		// is it has 'href' attribute?
		if (-1 != nHrefAttrIndex)
		{
			CLiteHTMLElemAttr oAttr = pAttrs->getAttribute(nHrefAttrIndex);
			CString sValue = oAttr.getValue();

			HtmlUtils::DecodeUrl (sValue);

			bool bFlag = false;
			const int cnHashIndex = sValue.Find ('#');
			if (-1 != cnHashIndex)
			{
				// 
				CString sTemp      = sValue.Mid (0, cnHashIndex);
				CString wsFullPath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sTemp.GetBuffer() ).c_str();
				CString wsTempHtml = m_sHtmlPath;

				wsFullPath.Replace (_T("/"), _T(""));
				wsFullPath.Replace (_T("\\"), _T(""));
				wsFullPath.Replace (_T(":"), _T(""));
				wsFullPath.MakeLower ();

				wsTempHtml.Replace (_T("/"), _T(""));
				wsTempHtml.Replace (_T("\\"), _T(""));
				wsTempHtml.Replace (_T(":"), _T(""));
				wsTempHtml.MakeLower ();

				if (wsTempHtml == wsFullPath)
					bFlag = true;
			}

			std::wstring wsResultPath = _T("");
			if (!bFlag)
			{
				std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

				// add 'file:///' prefix, if it is local
				if (-1 == sValue.Find (_T("www")) && -1 == sValue.Find(_T("http")) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) ) 				
					wsResultPath = L"file:///" + wsFromRelativePath;				
				else
					wsResultPath = sValue;
			}
			else
			{
				int nPos = sValue.Find( '#' );
				wsResultPath = std::wstring( sValue.Mid( nPos,  sValue.GetLength() - nPos ).GetBuffer() );
			}

			if ((wsResultPath != L"") && (NULL != xml_element_ptr))
			{
				wsResultPath = strings::replace_all(wsResultPath, L" ", L"%20");
				CString sResultPath = wsResultPath.c_str();

				BSTR bstrAttrName = oAttr.getName().MakeLower().AllocSysString();
				BSTR bstrResPath = sResultPath.AllocSysString();

				xml_element_ptr->setAttribute(bstrAttrName, (const _variant_t &) bstrResPath);

				SysFreeString (bstrAttrName);
				SysFreeString (bstrResPath);			
			}
		}
		// is it has 'name' attribute?
		if (-1 != nNameAttrIndex)
		{
			CLiteHTMLElemAttr oAttr = pAttrs->getAttribute(nHrefAttrIndex);
			CString sResultName = oAttr.getValue();

			if (!sResultName.IsEmpty() && (NULL != xml_element_ptr))
			{
				sResultName.Replace (_T(" "), _T("%20"));

				BSTR bstrAttrName = oAttr.getName().MakeLower().AllocSysString();
				BSTR bstrResName = sResultName.AllocSysString();

				xml_element_ptr->setAttribute (bstrAttrName, (const _variant_t &) bstrResName);

				SysFreeString (bstrAttrName);
				SysFreeString (bstrResName);	
			}
		}

		// check all other attributes
		const int nAttrCount = pAttrs->getCount();
		for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
		{
			CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
			
			const CString sName = oAttr.getName();
			if (sName != _T("href") && sName != _T("name") 
				&& sName != _T("style") 
				&& sName != _T("id") && sName != _T("class") && sName != _T("xmlns"))
			{
				AddAttributesToNode (oAttr, xml_element_ptr);
			}
		}

	}	// !m_bMht

	sStyle = oStyle.ToString();

	


	return TRUE;
}

// copy atributes from our class to msxml node
BOOL CHtmlToXhtmlSax::AddAttributesToNode (CLiteHTMLElemAttr &oAttr, MSXML2::IXMLDOMElement *xml_element)
{
	ATLASSERT (xml_element);
	BOOL bRes = TRUE;

	CString sName = oAttr.getName();
	CString sValue = oAttr.getValue();

	sName.Remove (':');
	/*sName.Remove (' ');
	sValue.Remove (':');
	sValue.Remove (' ');
	sValue.Remove ('-');
	sValue.Remove ('.');
	*/
	
	if (!sName.IsEmpty() && !sValue.IsEmpty())
	{
		BSTR bstrAttrName = sName.AllocSysString();
		BSTR bstrResName = sValue.AllocSysString();

		xml_element->setAttribute (bstrAttrName, (const _variant_t &) bstrResName);

		SysFreeString (bstrAttrName);
		SysFreeString (bstrResName);

	}
	return bRes;
}
CString CHtmlToXhtmlSax::convertBase64ToImage (CString &pBase64)
	{
		CString sImage;
		char sTempPath[MAX_PATH], sTempFile[MAX_PATH];
		if ( 0 == GetTempPathA( MAX_PATH, sTempPath ) )
			return _T("");

		if ( 0 == GetTempFileNameA( sTempPath, "CSS", 0, sTempFile ) )
			return _T("");

		sImage = sTempFile;
		HANDLE hFileWriteHandle;
		// Открываем на диске файл и получаем на него указатель
		hFileWriteHandle = ::CreateFile (sImage, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE != hFileWriteHandle)
		{
			INT nUTF8Len = WideCharToMultiByte (CP_UTF8, 0, pBase64, pBase64.GetLength (), NULL, NULL, NULL, NULL);
			CHAR *pUTF8String = new CHAR [nUTF8Len + 1];
			::ZeroMemory (pUTF8String, sizeof (CHAR) * (nUTF8Len + 1));

			WideCharToMultiByte (CP_UTF8, 0, pBase64, -1, pUTF8String, nUTF8Len, NULL, NULL);
			CStringA sUnicode; sUnicode = pUTF8String;
			delete[] pUTF8String;

			//Убираем "data:image/jpg;base64,"
			int nShift = 0;
			int nIndex = sUnicode.Find("base64,");
			if(-1 != nIndex)
			{
				nShift = nIndex + 7;
			}
			// Получаем размер файла
			LONG lFileSize = sUnicode.GetLength () - nShift;
			INT nDstLength = lFileSize;
			BYTE *pBuffer = new BYTE [lFileSize];
			::ZeroMemory (pBuffer, lFileSize);
			Base64::Base64Decode ((LPCSTR)sUnicode.GetBuffer () + nShift, lFileSize, pBuffer, &nDstLength);

			// Пишем в файл
			DWORD dwBytesWrite = 0;
			::WriteFile (hFileWriteHandle, pBuffer, nDstLength, &dwBytesWrite, 0);

			RELEASEARRAYOBJECTS (pBuffer);

			CloseHandle (hFileWriteHandle);
		}
		return sImage;
	}
// preprocess <img> tag
BOOL CHtmlToXhtmlSax::PreprocessImg (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();

	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;

	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();

	if (NULL == pAttrs)
	{
		return TRUE;
	}

		const int nSrcAttrIndex = pAttrs->getIndexFromName(_T("src"));
		const int nRecIndexAttrIndex = pAttrs->getIndexFromName(_T("recindex"));	// MOBI format of img tag

		if (-1 != nSrcAttrIndex || -1 != nRecIndexAttrIndex)
		{
			CString sAttrValue;
			CString sAttrName = _T("src");
			if (-1 != nSrcAttrIndex)
			{
				// HTML
				CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nSrcAttrIndex);
				sAttrValue = oAttr.getValue ();
			}
			else
			{
				// MOBI
				CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nRecIndexAttrIndex);
				const int nImageIndex = _ttoi (oAttr.getValue());
				sAttrValue.Format(_T(".\\media\\image%i.jpg"), nImageIndex);				
			}
			//CString sAttrValue = oAttr.getValue();

			// src attr
			int nWidth (0), nHeight(0);
			if (!m_bMht)
			{
				CString sValueEncode = sAttrValue;
				HtmlUtils::DecodeUrl (sAttrValue);
				CString sResultPath;
				CString sFromRelativePath = HtmlUtils::full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sAttrValue.GetBuffer() ).c_str();

				if (HtmlUtils::FileExist (&sAttrValue))
				{
					sResultPath = sAttrValue;
				}
				else if (HtmlUtils::FileExist(&sFromRelativePath) )
				{
					sResultPath = sFromRelativePath;
				}
				else
				{
					CString sPath = sValueEncode;
					if(0 == sPath.Find(_T("data:")))
					{
						CString sPath = convertBase64ToImage(sValueEncode);
						if (!sPath.IsEmpty())
						{
							sResultPath = sPath;
							m_arrImageFileName.Add (sResultPath);
						}
						else
							sResultPath.Empty ();
					}
					else if ( m_bDownloadImages && ( -1 != sPath.Find( _T("www") ) || -1 != sPath.Find( _T("http") ) || -1 != sPath.Find( _T("ftp") ) ) ) 
					{
						CFileDownloader oDownloader (sPath, FALSE);
						oDownloader.StartWork (1);
						while ( oDownloader.IsRunned() )
						{
							::Sleep (10);
						}
						//
						if ( oDownloader.IsFileDownloaded() )
						{
							sResultPath = oDownloader.GetFilePath().GetBuffer();
							m_arrImageFileName.Add (oDownloader.GetFilePath());
						}
						else
							sResultPath.Empty ();
					}
				}
				if (!sResultPath.IsEmpty() && (NULL != xml_element_ptr))
				{
					nWidth  = _ttoi (oStyle.getAttribute (_T("width")));
					nHeight = _ttoi (oStyle.getAttribute (_T("height")));

					Gdiplus::GdiplusStartupInput  oInput;
					Gdiplus::GdiplusStartupOutput oOutput;
					ULONG dwToken = 0;
					Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
					if ( eStatus == Gdiplus::Ok )
					{
						Gdiplus::Bitmap oImage (sResultPath);						

						int nImgWidth = oImage.GetWidth();
						int nImgHeight = oImage.GetHeight();

						nWidth  = CLiteHTMLElemStyleAttr::ConvertValueToPx (oStyle.getAttribute (_T("width")), nImgWidth);
						nHeight = CLiteHTMLElemStyleAttr::ConvertValueToPx (oStyle.getAttribute (_T("height")), nImgHeight);

						setAttrs (nWidth, nHeight, oImage);
						xml_element_ptr->setAttribute( _bstr_t( L"width" ), (const _variant_t &) nWidth );
						xml_element_ptr->setAttribute( _bstr_t( L"height" ), (const _variant_t &) nHeight );
					}
					Gdiplus::GdiplusShutdown (dwToken);

					BSTR bstrAttrName = sAttrName.AllocSysString();
					BSTR bstrResPath = sResultPath.AllocSysString();

					xml_element_ptr->setAttribute (bstrAttrName, (const _variant_t &) bstrResPath);

					SysFreeString (bstrAttrName);
					SysFreeString (bstrResPath);
				}
			}
			else // mht
			{
				std::wstring strTempPath = CHtmlUtility::GetTempName();
				const bool bSaved = m_oMhtFile.savePicture( (const std::wstring) sAttrValue.GetBuffer(), strTempPath );
				if (NULL != xml_element_ptr)
				{
					if (bSaved)
					{
						nWidth  = _ttoi (oStyle.getAttribute (_T("width")));
						nHeight = _ttoi (oStyle.getAttribute (_T("height")));

						Gdiplus::GdiplusStartupInput  oInput;
						Gdiplus::GdiplusStartupOutput oOutput;
						ULONG dwToken = 0;
						Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
						if ( eStatus == Gdiplus::Ok )
						{
							Gdiplus::Bitmap oImage (strTempPath.c_str());
							int nImgWidth = oImage.GetWidth();
							int nImgHeight = oImage.GetHeight();

							nWidth  = CLiteHTMLElemStyleAttr::ConvertValueToPx (oStyle.getAttribute (_T("width")), nImgWidth);
							nHeight = CLiteHTMLElemStyleAttr::ConvertValueToPx (oStyle.getAttribute (_T("height")), nImgHeight);

							setAttrs (nWidth, nHeight, oImage);

							xml_element_ptr->setAttribute( _bstr_t( L"width" ), (const _variant_t &)nWidth );
							xml_element_ptr->setAttribute( _bstr_t( L"height" ), (const _variant_t &)nHeight );
						}
						Gdiplus::GdiplusShutdown( dwToken );

						// TODO: rename temp image (extension) and write local path to an img 'src' attribute value
						BSTR bstrAttrName = sAttrName.AllocSysString();	// src
						BSTR bstrResPath = sAttrValue.AllocSysString();						// web link

						xml_element_ptr->setAttribute (bstrAttrName, (const _variant_t &) bstrResPath);

						SysFreeString (bstrAttrName);
						SysFreeString (bstrResPath);
					}
					else
					{
						BSTR bstrAttrName = sAttrName.AllocSysString();	// src
						BSTR bstrResPath = sAttrValue.AllocSysString();					// web link

						xml_element_ptr->setAttribute (bstrAttrName, (const _variant_t &) bstrResPath);

						SysFreeString (bstrAttrName);
						SysFreeString (bstrResPath);
					}
				}

			} // mht

			// work with img
			bool bCheckWidth = false, bCheckHeight = false, bCheckMaxWidth = false;

			CString sWidth = oStyle.getAttribute (_T("width"));
			CString sHeight = oStyle.getAttribute (_T("height"));
			CString sMaxWidth = oStyle.getAttribute (_T("max-width"));
			CString sMaxHeight = oStyle.getAttribute (_T("max-width"));

			// translate 'auto' and 'XX%' style to 'XX.XXpt' style
			// 'width' style attribute
			bCheckWidth = (TRUE == CLiteHTMLElemStyleAttr::SizeValueToPt (sWidth, nWidth));
			if (bCheckWidth)
				oStyle.setAttribute (_T("width"), sWidth);

			// 'max-width' style attribute
			bCheckMaxWidth = (TRUE == CLiteHTMLElemStyleAttr::SizeValueToPt (sMaxWidth, nWidth));
			if (bCheckMaxWidth)
				oStyle.setAttribute (_T("max-width"), sWidth);

			// 'height' style attribute
			bCheckHeight = (TRUE == CLiteHTMLElemStyleAttr::SizeValueToPt (sHeight, nHeight));
			if (bCheckHeight)
				oStyle.setAttribute (_T("height"), sWidth);

			// margins
			oStyle.FillMargins();

			//
			// Проходим по всем CSS-кам для этой ноды
			CHtmlCSSList oTempList;
			CSimpleArray<CHtmlCSS *> oCSSs = m_oCssList.get_CSS (pTagWrapper);
			for ( int nIndex = oCSSs.GetSize() - 1; nIndex >= 0; nIndex-- )
			{
				// если размеры высоты и ширины проверены, то ничего не делаем
				if ( bCheckHeight && bCheckWidth )
					break;

				CHtmlCSS *pCSS = oCSSs[nIndex];

				CHtmlCSS oTempCss (pCSS->get_Selector(), pCSS->get_RawProperties());
				CHtmlCSSProperty *pProperty = NULL;

				if (!bCheckWidth && NULL != ( pProperty = oTempCss.get_Property( ptWidth)))
				{
					if (-1 != pProperty->get_Value().Find (_T("auto")))
					{
						CString sTemp;
						double dWidth = 72.0 / 96.0 * nWidth;
						sTemp.Format (_T("%.2fpt"), dWidth);
						pProperty->set_Value (sTemp);
						bCheckWidth = true;
					}
					if (!bCheckWidth && -1 != pProperty->get_Value().Find (_T("%")))
					{
						CString sTemp = pProperty->get_Value ();
						sTemp.Remove ('%');

						int nPerc = _wtoi (sTemp.GetBuffer ());
						sTemp.Format( _T("%.2fpt"), double (nWidth * 72.0 / 96.0 * (nPerc / 100.0)));

						bCheckWidth = true;
					}
				}

				if (!bCheckHeight && NULL != (pProperty = oTempCss.get_Property (ptHeight)))
				{
					if (-1 != pProperty->get_Value().Find (_T("auto")))
					{
						CString sTemp;
						double dHeight = 72.0 / 96.0 * nHeight;
						sTemp.Format (_T("%.2fpt"), dHeight);
						pProperty->set_Value (sTemp);
						bCheckHeight = true;
					}
					if (!bCheckHeight && -1 != pProperty->get_Value().Find (_T("%")))
					{
						CString sTemp = pProperty->get_Value ();
						sTemp.Remove ('%');

						int nPerc = _wtoi (sTemp.GetBuffer ());
						sTemp.Format (_T("%.2fpt"), double (nHeight * 72.0 / 96.0 * (nPerc / 100.0)));
						bCheckHeight = true;
					}
				}
				oTempList.Add (oTempCss);
			}

			// merge styles from tag and from css list
			CString sMergedStyle = oTempList.get_ElementCSS (pTagWrapper) + oStyle.ToString ();
			double dNewEmValue = dEmValue;
			CHtmlCSS oCss( _T(""), sMergedStyle);
			for ( int nIndex = 0; nIndex < oCss.get_PropertiesCount(); nIndex++ )
			{
				// Переводим параметры из 'em' в пиксели
				CHtmlCSSProperty *pProperty = oCss.get_Property( nIndex );
				if ( NULL == pProperty )
					continue;

				pProperty->ConvertValueToPx( dEmValue );
			}
			
			CHtmlCSSProperty *pProperty = oCss.get_Property( ptFontSize );
			if ( NULL != pProperty )
			{
				CString sValue = pProperty->get_Value();
				int nPos (0);
			
				if (-1 != ( nPos = sValue.Find( _T("%"))))
				{
					double dValue = _tstof (sValue.Mid (0, nPos));
					dValue *= (dEmValue / 100);
					sValue.Format( _T("%.3fpx"), dValue );
					pProperty->set_Value (sValue);
					dNewEmValue = dValue;
				}
			}
			
			sStyle = oCss.get_Properties();

			// check all other attributes
			const int nAttrCount = pAttrs->getCount();
			for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
			{
				CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
				
				const CString sName = oAttr.getName();
				if (sName != _T("href") && sName != _T("name") 
					&& sName != _T("src") 
					&& sName != _T("recindex")
					&& sName != _T("style") 
					&& sName != _T("id") && sName != _T("class") && sName != _T("xmlns"))
				{
					AddAttributesToNode (oAttr, xml_element_ptr);
				}
			}
			
		} // if 'src' attr exists

		return TRUE;
}

// preprocess textarea tag
BOOL CHtmlToXhtmlSax::PreprocessTextarea (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;

	// NON STANDARD HACK: our textarea has no children - delete all children
	MSXML2::IXMLDOMNodePtr pFrameChild;
	do
	{
		xml_element_ptr->get_firstChild (&pFrameChild);
		if (NULL != pFrameChild)
		{
			xml_element_ptr->removeChild (pFrameChild);
			pFrameChild = NULL;
		}		
		else break;
	}
	while (true);

	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();

	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();


	if (NULL == pAttrs)
	{
		return TRUE;
	}

	const int nAttrCount = pAttrs->getCount();
	for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
		
		const CString sName = oAttr.getName();
		if (0 != sName.CompareNoCase (_T("style"))
			&& 0 != sName.CompareNoCase (_T("id")) 
			&& 0 != sName.CompareNoCase (_T("class")) 
			&& 0 != sName.CompareNoCase (_T("xmlns"))
			&& 0 != sName.CompareNoCase (_T("value"))
			)
		{
			AddAttributesToNode (oAttr, xml_element_ptr);
		}
	}

	CString sText;
	const int nTexts = pLiteTag->getTextsCount();
	for (int nText = 0; nText < nTexts; nText++)
	{
		sText += pLiteTag->getTextAt (nText);
	}
	if (!sText.IsEmpty())
	{
		const CString sName = _T("value");
		BSTR bstrAttrName = sName.AllocSysString();
		BSTR bstrResName = sText.AllocSysString();

		xml_element_ptr->setAttribute (bstrAttrName, (const _variant_t &) bstrResName);

		SysFreeString (bstrAttrName);
		SysFreeString (bstrResName);
	}

	sStyle = oStyle.ToString();
	return TRUE;
}

// preprocess font tag
BOOL CHtmlToXhtmlSax::PreprocessFontTag (CHTMLTagWrapper *pTagWrapper, CString &sStyle, const double dEmValue)
{
	CLiteHTMLTag *pLiteTag = pTagWrapper->Tag();
	CLiteHTMLAttributes *pAttrs = pLiteTag->getAttributes();
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pTagWrapper->pXmlTagNode;

	CLiteHTMLElemStyleAttr& oStyle = pTagWrapper->getStyle();
	sStyle = oStyle.ToString();


	if (NULL == pAttrs)
	{
		return TRUE;
	}
		

	// check all other attributes
	const int nAttrCount = pAttrs->getCount();
	for (int nAttr = 0; nAttr < nAttrCount; nAttr++)
	{
		CLiteHTMLElemAttr oAttr = pAttrs->getAttribute (nAttr);
		
		const CString sName = oAttr.getName();
		if (0 != sName.CompareNoCase (_T("style"))
			&& 0 != sName.CompareNoCase (_T("id")) 
			&& 0 != sName.CompareNoCase (_T("class")) 
			&& 0 != sName.CompareNoCase (_T("xmlns"))
			)
		{
			if (0 == sName.CompareNoCase(_T("color")) && (oAttr.isNamedColorValue()))
				oAttr.putValue (oAttr.getColorHexValue());

			AddAttributesToNode (oAttr, xml_element_ptr);
		}
	}

	sStyle = oStyle.ToString();

	return TRUE;
}