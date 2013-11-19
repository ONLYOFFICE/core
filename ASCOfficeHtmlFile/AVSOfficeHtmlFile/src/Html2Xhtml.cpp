#include "stdafx.h"

#include <exdisp.h>
#include <string>
#include <iostream>
#include <locale>
#include <locale.h>
#include <fstream>
#include <vector>
#include <algorithm>

#include "utils.h"
#include "strings.h"
#include <atlenc.h>
#include <atlcoll.h>
#include "gdiplus.h"
#include "path.h"
#include "Html2Xhtml.h"
#include "htmlite.h"
#include "utf8.h"
#include "Utility.h"
#include "CHtmlUtility.h"

#include "FileDownloader.h"
#include "OfficeDrawing/File.h"

HtmlToXhtml::HtmlToXhtml( CString sHtmlPath, CString sXhtmlPath, const MhtFile* pMhtFile) :
    m_pBuffer( NULL ),
    m_sHtmlPath( sHtmlPath ),
    m_sXhtmlPath( sXhtmlPath ),
    m_pHtmlDoc( CLSID_HTMLDocument ),
    m_pXmlDoc( L"Msxml2.DOMDocument" ),
    m_oHtmLite()
{
	if( NULL != pMhtFile )
	{
		m_bMht     = true;
		m_oMhtFile = *pMhtFile;
	}
	else
	{
		m_bMht = false;
		m_dwBufferSize = 0;

		PrepareHTML();
	}
}


HtmlToXhtml::~HtmlToXhtml()
{
	if ( m_pBuffer )
	{
		m_dwBufferSize = 0;
		delete []m_pBuffer;
	}
}


MSXML2::IXMLDOMNodePtr HtmlToXhtml::getDoc()
{
	return m_pXmlDoc;
}

MSXML2::IXMLDOMNodePtr HtmlToXhtml::getBody()
{
	MSXML2::IXMLDOMNodeListPtr body;
	MSXML2::IXMLDOMNodePtr body_ptr = m_pXmlDoc->raw_getElementsByTagName(L"body", &body);
	
	return body->nextNode();
}


void HtmlToXhtml::nodeHtmlToXhtml( htmlayout::dom::element pHtmlNode, MSXML2::IXMLDOMNodePtr pXmlNode, double dEmValue)
{
	if ( !pHtmlNode || !pXmlNode )
		return;

    _bstr_t el_type = _bstr_t( pHtmlNode.get_element_type() );
	MSXML2::IXMLDOMElementPtr xml_element_ptr = pXmlNode;
    uint    l;
    std::vector<std::wstring> text_elems;
    std::wstring tmp;

	for ( unsigned int unAttrIndex = 0; unAttrIndex < pHtmlNode.get_attribute_count(); ++unAttrIndex )
	{	
		bool bSkip = false;
		
		CString sAttrName( pHtmlNode.get_attribute_name(unAttrIndex));
		CString sValue( pHtmlNode.get_attribute(unAttrIndex));
		int p = sAttrName.Find(_T(":"));
		if (sAttrName.GetLength() <= 0 || sAttrName.Find(_T(":")) != -1 || sValue.GetLength() <= 0)
			continue;
		sAttrName.MakeLower();		

		if ((el_type == _bstr_t(L"img")) && (sAttrName == _T("src")))
		{
			if (false == m_bMht)
			{
				CString sValueEncode = sValue;
				DecodeUrl( &sValue );
				std::wstring wsResultPath = _T("");
				std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

				// Путь к файлу может быть либо полным, либо относительным, либо ссылкой на скачивание в интернете
				if ( FileExist( &sValue ) )
				{
					wsResultPath = std::wstring( sValue.GetBuffer() );
				}
				else if ( FileExist( &CString( wsFromRelativePath.c_str() ) ) )
				{
					wsResultPath = wsFromRelativePath;
				}
				else
				{
					CString sPath = sValueEncode;
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

				bSkip = ( wsResultPath == L"" );

				if ( !bSkip )
				{
					int nWidth  = _wtoi( pHtmlNode.get_style_attribute( "width" ) );
					int nHeight = _wtoi( pHtmlNode.get_style_attribute( "height" ) );

					Gdiplus::GdiplusStartupInput  oInput;
					Gdiplus::GdiplusStartupOutput oOutput;
					ULONG dwToken = 0;
					Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
					if ( eStatus == Gdiplus::Ok )
					{
						Gdiplus::Bitmap oImage( wsResultPath.c_str() );						
						setAttrs( nWidth, nHeight, oImage );
						xml_element_ptr->setAttribute( _bstr_t( L"width" ), (const _variant_t &) nWidth );
						xml_element_ptr->setAttribute( _bstr_t( L"height" ), (const _variant_t &) nHeight );
					}
					Gdiplus::GdiplusShutdown( dwToken );

					xml_element_ptr->setAttribute( _bstr_t( sAttrName.GetBuffer() ), (const _variant_t &) _bstr_t( wsResultPath.c_str() ) );
					bSkip = true;
				}
			}
			else
			{
				std::wstring strTempPath = CHtmlUtility::GetTempName();
				bSkip = !m_oMhtFile.savePicture( (const std::wstring) sValue.GetBuffer(), strTempPath );
				if ( !bSkip )
				{

					int nWidth  = _wtoi( pHtmlNode.get_style_attribute( "width" ) );
					int nHeight = _wtoi( pHtmlNode.get_style_attribute( "height" ) );

					Gdiplus::GdiplusStartupInput  oInput;
					Gdiplus::GdiplusStartupOutput oOutput;
					ULONG dwToken = 0;
					Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
					if ( eStatus == Gdiplus::Ok )
					{
						Gdiplus::Bitmap oImage( strTempPath.c_str() );						
						setAttrs( nWidth, nHeight, oImage );
						xml_element_ptr->setAttribute( _bstr_t( L"width" ), (const _variant_t &)nWidth );
						xml_element_ptr->setAttribute( _bstr_t( L"height" ), (const _variant_t &)nHeight );
					}
					Gdiplus::GdiplusShutdown( dwToken );
				}
			}
		}
		else if ((el_type == _bstr_t(L"img")) && ((sAttrName == _T("height")) || (sAttrName == _T("width"))))
		{
			bSkip = true;
		}
		else if ( (el_type == _bstr_t( L"a" )) && ( sAttrName == _T("href") ) )
		{
			if ( false == m_bMht )
			{
				DecodeUrl( &sValue );

				bool bFlag = false;
				if ( -1 != sValue.Find( '#' ) )
				{
					CString sTemp      = sValue.Mid( 0, sValue.Find( '#' ) );
					CString wsFullPath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sTemp.GetBuffer() ).c_str();
					CString wsTempHtml = m_sHtmlPath;

					wsFullPath.Replace( _T("/"),   _T("") );
					wsFullPath.Replace(  _T("\\"), _T("") );
					wsFullPath.Replace(  _T(":"),  _T("") );
					wsFullPath.MakeLower();

					wsTempHtml.Replace(  _T("/"),  _T("") );
					wsTempHtml.Replace(  _T("\\"), _T("") );
					wsTempHtml.Replace(  _T(":"),  _T("") );
					wsTempHtml.MakeLower();

					if ( wsTempHtml == wsFullPath )
						bFlag = true;
				}

				std::wstring wsResultPath = _T("");
				if ( !bFlag )
				{
					std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

					if (  -1 == sValue.Find( _T("www") ) && -1 == sValue.Find( _T("http") ) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) ) 				
						wsResultPath = L"file:///" + wsFromRelativePath;				
					else
						wsResultPath = sValue;
				}
				else
				{
					int nPos = sValue.Find( '#' );
					wsResultPath = std::wstring( sValue.Mid( nPos,  sValue.GetLength() - nPos ).GetBuffer() );
				}

				bSkip = ( wsResultPath == L"");

				wsResultPath = strings::replace_all(wsResultPath, L" ", L"%20");

				if ( !bSkip )
				{
					xml_element_ptr->setAttribute( _bstr_t( sAttrName.GetBuffer() ), (const _variant_t &) _bstr_t( wsResultPath.c_str() ) );
					bSkip = true;
				}
			}
		}
		else if ( (el_type == _bstr_t( L"a" )) && ( sAttrName == _T("name") ) )
		{
			if ( false == m_bMht )
			{
				std::wstring wsResultName = sValue.GetBuffer();

				bSkip = ( wsResultName == L"");

				wsResultName = strings::replace_all(wsResultName, L" ", L"%20");

				if ( !bSkip )
				{
					xml_element_ptr->setAttribute( _bstr_t( sAttrName.GetBuffer() ), (const _variant_t &) _bstr_t( wsResultName.c_str() ) );
					bSkip = true;
				}
			}
		}
		else if ( (el_type == _bstr_t( L"frame" )) && ( sAttrName == _T("src") ) )
		{
			DecodeUrl( &sValue );
			std::wstring wsResultPath = _T("");
			std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

			if (  -1 == sValue.Find( _T("www") ) && -1 == sValue.Find( _T("http") ) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) )
			{
				if ( (-1 != sValue.Find( _T(".htm") )) ||(-1 != sValue.Find( _T(".html") )) )
				{
					HtmlToXhtml oHtmlToXhtml( wsFromRelativePath.c_str(), L"", NULL);

					bool bResult = oHtmlToXhtml.Convert_htmlayout();
					if (bResult == true)
					{
						MSXML2::IXMLDOMNode *pTempNode = NULL;
						pXmlNode->raw_appendChild( oHtmlToXhtml.getBody(), &pTempNode );
						AddTempImages(oHtmlToXhtml.GetTempImages());
						RELEASEINTERFACE(pTempNode);
					}
				}
			}
			break;
		}
		else if ( (el_type == _bstr_t( L"iframe" )) && ( sAttrName == _T("src") ) )
		{
			DecodeUrl( &sValue );
			std::wstring wsResultPath = _T("");
			std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

			if (  -1 == sValue.Find( _T("www") ) && -1 == sValue.Find( _T("http") ) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) )
			{
				if ( (-1 != sValue.Find( _T(".htm") )) ||(-1 != sValue.Find( _T(".html") )) )
				{
					HtmlToXhtml oHtmlToXhtml( wsFromRelativePath.c_str(), L"", NULL );

					bool bResult = oHtmlToXhtml.Convert_htmlayout();
					if (bResult == true)
					{
						MSXML2::IXMLDOMNode *pTempNode = NULL;
						pXmlNode->raw_appendChild( oHtmlToXhtml.getBody(), &pTempNode );
						AddTempImages(oHtmlToXhtml.GetTempImages());
						RELEASEINTERFACE(pTempNode);
					}
				}
			}
			break;
		}

		if ( ( sAttrName == _T("style") ) || ( sAttrName == _T("class") ) || ( sAttrName == _T("xmlns") ) || ( sAttrName == _T("id") ) || ( sValue == _T("") ) )
			bSkip = true;
		if ( !bSkip )
		{
			HRESULT hRes = xml_element_ptr->raw_setAttribute( _bstr_t( sAttrName.GetBuffer() ), (const _variant_t &)sValue.GetBuffer() );
		}
	}

	_bstr_t bStyle = L"style";
    _bstr_t styleAttr = FindAttribute( pHtmlNode, bStyle);

	_bstr_t styleCss;


	// TO DO: специальная заглушка для нашего вьювера. Как только мы будем 
	// отображать картинки нормально с любыми параметрами, это нужно будет убрать
	if ( el_type == _bstr_t(L"img") )
	{
		VARIANT vValue;
		
		if ( FAILED( xml_element_ptr->raw_getAttribute( _bstr_t( L"width" ), &vValue ) ) || vValue.vt != VT_BSTR || vValue.bstrVal == bstr_t(L"") )
			return;

		int nWidth = _wtoi( vValue.bstrVal );
		::SysFreeString( vValue.bstrVal );

		if ( FAILED( xml_element_ptr->raw_getAttribute( _bstr_t( L"height" ), &vValue ) ) || vValue.vt != VT_BSTR || vValue.bstrVal == bstr_t(L"") )
			return;

		int nHeight = _wtoi( vValue.bstrVal );
		::SysFreeString( vValue.bstrVal );

		bool bCheckWidth = false, bCheckHeight = false, bCheckMaxWidth = false;

		CHtmlCSS oTempCss( _T(""), styleAttr.GetBSTR() );
		CHtmlCSSProperty *pProperty = NULL;

		if ( NULL != ( pProperty = oTempCss.get_Property( ptWidth ) ) )
		{
			if ( -1 != pProperty->get_Value().Find( _T("auto") )  )
			{
				CString sTemp;
				double dWidth = 72.0 / 96.0 * nWidth;
				sTemp.Format( _T("%.2fpt"), dWidth  );
				pProperty->set_Value( sTemp );
				bCheckWidth = true;
			}
			if ( !bCheckWidth && -1 != pProperty->get_Value().Find( _T("%") ) )
			{
				CString sTemp = pProperty->get_Value();
				sTemp.Remove( '%' );

				int nPerc = _wtoi( sTemp.GetBuffer() );
				sTemp.Format( _T("%.2fpt"), double(nWidth * 72.0 / 96.0 * (nPerc / 100.0)) );
				pProperty->set_Value( sTemp );

				bCheckWidth = true;
			}
		}

		if ( NULL != ( pProperty = oTempCss.get_Property( ptMaxWidth ) ) )
		{
			if ( -1 != pProperty->get_Value().Find( _T("auto") )  )
			{
				CString sTemp;
				double dWidth = 72.0 / 96.0 * nWidth;
				sTemp.Format( _T("%.2fpt"), dWidth  );
				pProperty->set_Value( sTemp );
				bCheckMaxWidth = true;
			}
			if ( !bCheckMaxWidth && -1 != pProperty->get_Value().Find( _T("%") ) )
			{
				CString sTemp = pProperty->get_Value();
				sTemp.Remove( '%' );

				int nPerc = _wtoi( sTemp.GetBuffer() );
				sTemp.Format( _T("%.2fpt"), double(nWidth * 72.0 / 96.0 * (nPerc / 100.0)) );
				pProperty->set_Value( sTemp );

				bCheckMaxWidth = true;
			}
		}


		if ( NULL != ( pProperty = oTempCss.get_Property( ptHeight ) ) )
		{
			if ( -1 != pProperty->get_Value().Find( _T("auto") )  )
			{
				CString sTemp;
				double dHeight = 72.0 / 96.0 * nHeight;
				sTemp.Format( _T("%.2fpt"), dHeight  );
				pProperty->set_Value( sTemp );
				bCheckHeight = true;
			}
			if ( !bCheckHeight && -1 != pProperty->get_Value().Find( _T("%") ) )
			{
				CString sTemp = pProperty->get_Value();
				sTemp.Remove( '%' );

				int nPerc = _wtoi( sTemp.GetBuffer() );
				sTemp.Format( _T("%.2fpt"), double(nHeight * 72.0 / 96.0 * (nPerc / 100.0)) );
				pProperty->set_Value( sTemp );

				bCheckHeight = true;
			}
		}

		if ( NULL != ( pProperty = oTempCss.get_Property( ptMargin ) ) )
		{
			CString sValue = pProperty->get_Value();

			int nCount = 4;
			CString sVal1, sVal2, sVal3, sVal4;

			int nPos = 0;
			int nLen = sValue.GetLength();

			while ( nPos < nLen && ' ' == sValue[nPos] )
				nPos++;

			int nPos2 = sValue.Find( _T(" "), nPos );
			if ( -1 == nPos2 )
				nPos2 = nLen;

			sVal1 = sValue.Mid( nPos, nPos2 - nPos );
			nPos = nPos2;

			while ( nPos < nLen && ' ' == sValue[nPos] )
				nPos++;

			nPos2 = sValue.Find( _T(" "), nPos );

			if ( -1 == nPos2 )
				nPos2 = nLen;

			sVal2 = sValue.Mid( nPos, nPos2 - nPos );
				nPos = nPos2;

			while ( nPos < nLen && ' ' == sValue[nPos] )
				nPos++;

			nPos2 = sValue.Find( _T(" "), nPos );
			if ( -1 == nPos2 )
				nPos2 = nLen;

			sVal3 = sValue.Mid( nPos, nPos2 - nPos );
				nPos = nPos2;

			while ( nPos < nLen && ' ' == sValue[nPos] )
				nPos++;

			nPos2 = sValue.Find( _T(" "), nPos );
			if ( -1 != nPos2 )
				nPos2 = nLen;

			sVal4 = sValue.Mid( nPos, nPos2 - nPos );
				nPos = nPos2;

			if ( _T("") == sVal4 )
				nCount--;
			if ( _T("") == sVal3 )
				nCount--;
			if ( _T("") == sVal2 )
				nCount--;
			if ( _T("") == sVal1 )
				nCount--;

			switch(nCount)
			{
			case 1:
				{
					CHtmlCSSProperty oProp1( CString(_T("margin-left:")) + sVal1 );
					CHtmlCSSProperty oProp2( CString(_T("margin-top:")) + sVal1 );
					CHtmlCSSProperty oProp3( CString(_T("margin-right:")) + sVal1 );
					CHtmlCSSProperty oProp4( CString(_T("margin-bottom:")) + sVal1 );

					oTempCss.add_Property( oProp1 );
					oTempCss.add_Property( oProp2 );
					oTempCss.add_Property( oProp3 );
					oTempCss.add_Property( oProp4 );
					break;
				}
			case 2:
				{
					CHtmlCSSProperty oProp1( CString(_T("margin-left:")) + sVal2 );
					CHtmlCSSProperty oProp2( CString(_T("margin-top:")) + sVal1 );
					CHtmlCSSProperty oProp3( CString(_T("margin-right:")) + sVal2 );
					CHtmlCSSProperty oProp4( CString(_T("margin-bottom:")) + sVal1 );

					oTempCss.add_Property( oProp1 );
					oTempCss.add_Property( oProp2 );
					oTempCss.add_Property( oProp3 );
					oTempCss.add_Property( oProp4 );
					break;
				}
			case 3:
				{
					CHtmlCSSProperty oProp1( CString(_T("margin-left:")) + sVal2 );
					CHtmlCSSProperty oProp2( CString(_T("margin-top:")) + sVal1 );
					CHtmlCSSProperty oProp3( CString(_T("margin-right:")) + sVal2 );
					CHtmlCSSProperty oProp4( CString(_T("margin-bottom:")) + sVal3 );

					oTempCss.add_Property( oProp1 );
					oTempCss.add_Property( oProp2 );
					oTempCss.add_Property( oProp3 );
					oTempCss.add_Property( oProp4 );
					break;
				}
			case 4:
				{
					CHtmlCSSProperty oProp1( CString(_T("margin-left:")) + sVal4 );
					CHtmlCSSProperty oProp2( CString(_T("margin-top:")) + sVal1 );
					CHtmlCSSProperty oProp3( CString(_T("margin-right:")) + sVal2 );
					CHtmlCSSProperty oProp4( CString(_T("margin-bottom:")) + sVal3 );

					oTempCss.add_Property( oProp1 );
					oTempCss.add_Property( oProp2 );
					oTempCss.add_Property( oProp3 );
					oTempCss.add_Property( oProp4 );
					break;
				}
			}
		}

		styleAttr = _bstr_t( oTempCss.get_Properties() );

		CHtmlCSSList oTempList;
		CSimpleArray<CHtmlCSS *> oCSSs = m_oCssList.get_CSS( pHtmlNode );
		for ( int nIndex = oCSSs.GetSize() - 1; nIndex >= 0; nIndex-- )
		{
			if ( bCheckHeight && bCheckWidth )
				break;

			CHtmlCSS *pCSS = oCSSs[nIndex];

			CHtmlCSS oTempCss( pCSS->get_Selector(), pCSS->get_RawProperties() );
			CHtmlCSSProperty *pProperty = NULL;

			if ( !bCheckWidth && NULL != ( pProperty = oTempCss.get_Property( ptWidth ) ) )
			{
				if ( -1 != pProperty->get_Value().Find( _T("auto") )  )
				{
					CString sTemp;
					double dWidth = 72.0 / 96.0 * nWidth;
					sTemp.Format( _T("%.2fpt"), dWidth  );
					pProperty->set_Value( sTemp );
					bCheckWidth = true;
				}
				if ( !bCheckWidth && -1 != pProperty->get_Value().Find( _T("%") ) )
				{
					CString sTemp = pProperty->get_Value();
					sTemp.Remove( '%' );

					int nPerc = _wtoi( sTemp.GetBuffer() );
					sTemp.Format( _T("%.2fpt"), double(nWidth * 72.0 / 96.0 * (nPerc / 100.0)) );

					bCheckWidth = true;
				}
			}

			if ( !bCheckHeight && NULL != ( pProperty = oTempCss.get_Property( ptHeight ) ) )
			{
				if ( -1 != pProperty->get_Value().Find( _T("auto") )  )
				{
					CString sTemp;
					double dHeight = 72.0 / 96.0 * nHeight;
					sTemp.Format( _T("%.2fpt"), dHeight  );
					pProperty->set_Value( sTemp );
					bCheckHeight = true;
				}
				if ( !bCheckHeight && -1 != pProperty->get_Value().Find( _T("%") ) )
				{
					CString sTemp = pProperty->get_Value();
					sTemp.Remove( '%' );

					int nPerc = _wtoi( sTemp.GetBuffer() );
					sTemp.Format( _T("%.2fpt"), double(nHeight * 72.0 / 96.0 * (nPerc / 100.0)) );
					bCheckHeight = true;
				}
			}
			oTempList.Add( oTempCss );
		}
		styleCss = oTempList.get_ElementCSS( pHtmlNode ).GetBuffer();		
	}
	else
	{
		styleCss = m_oCssList.get_ElementCSS( pHtmlNode ).GetBuffer();
	}
	
	//_bstr_t styleCss = getElementCSS( pHtmlNode ).c_str();
	
	std::wstring style = strings::trim(( styleCss + styleAttr ).GetBSTR());

	// обработка размеров в формате 'em'
	double dNewEmValue = dEmValue;
	CHtmlCSS oCss( _T(""), CString(style.c_str()) );
	for ( int nIndex = 0; nIndex < oCss.get_PropertiesCount(); nIndex++ )
	{
		// Переводим параметры из 'em' в пиксели
		CHtmlCSSProperty *pProperty = oCss.get_Property( nIndex );
		if ( NULL == pProperty )
			continue;

		pProperty->ConvertValueToPx( dEmValue );
	}
	style = std::wstring( oCss.get_Properties().GetBuffer() );

	CHtmlCSSProperty *pProperty = oCss.get_Property( ptFontSize );
	if ( NULL != pProperty )
	{
		CString sValue = pProperty->get_Value();
		int nPos = sValue.Find( _T("px") );
		if ( -1 != nPos )
		{
			dNewEmValue = _wtof( sValue.Mid( 0, nPos ) );
		}
	}
	

	//style = strings::make_lower( _bstr_t( style.c_str() ) ).GetBSTR();
	style = (_bstr_t((SetLowerStyleNames(style)).c_str())).GetBSTR();
	//style = convertElementCSS( pHtmlNode );
	bool hasFont	  = (std::wstring::npos != style.find(L"font"));
	bool hasBodyImage = (el_type == _bstr_t(L"body") && std::wstring::npos != style.find(L"background-image"));
	//if (std::wstring::npos != style.find(L"font"))
	if (hasFont || hasBodyImage)
	{
		pHtmlNode.set_attribute("style", style.c_str());
		if(hasFont)
			convertElementFont(pHtmlNode);
		if (hasBodyImage)
			convertElementBodyBackgroundImage(pHtmlNode, xml_element_ptr);
		style = FindAttribute(pHtmlNode, _bstr_t(L"style"));
	}	

	style = strings::replace_all( style, L"\"", L"'" );
	if ( style.length() )
		xml_element_ptr->setAttribute( _bstr_t( L"style" ), (const _variant_t &) _bstr_t( style.c_str() ) );


	HTMLayoutGetElementText( pHtmlNode, NULL, &l );
	if ( 0 != l )
	{
		wchar_t *wsText = new wchar_t[l];
		if ( !wsText )
			return;

		HTMLayoutGetElementText( pHtmlNode, wsText , &l );

		for ( uint i = 0; i < l; ++i )
		{
			if ( wsText[ i ] )
			{
				tmp += wsText[ i ];
			}
			else
			{
				text_elems.push_back( tmp );
				tmp = L"";
			}
		}
		delete []wsText;
	}

	//BSTR el_text = (BSTR)pHtmlNode.text();
	//HTMLayoutGetElementText( pHtmlNode, el_text, &l );
	//for ( uint i = 0; i < l; ++i )
	//{
	//	if ( el_text[ i ] )
	//	{
	//		tmp += el_text[ i ];
	//	}
	//	else
	//	{
	//		text_elems.push_back( tmp );
	//		tmp = L"";
	//	}
	//}
	if ( tmp.length() )
		text_elems.push_back( tmp );

	int k = pHtmlNode.children_count();

	for ( uint i = 0; i < max( text_elems.size(), pHtmlNode.children_count() ); ++i )
	{
		if ( i < text_elems.size() )
		{
			std::wstring            tmp = text_elems[ i ];
			std::wstring::size_type br  = tmp.find( ( wchar_t ) 13 );
			while ( std::wstring::npos != br )
			{
				MSXML2::IXMLDOMNode *pXmlNewTextNode = NULL;
				m_pXmlDoc->raw_createNode( (const _variant_t &) int( 3 ), L"", L"", &pXmlNewTextNode );

				pXmlNewTextNode->put_nodeValue( (const _variant_t &) _bstr_t( tmp.substr( 0, br ).c_str() ) );

				MSXML2::IXMLDOMNode *pTempNode = NULL;
				pXmlNode->raw_appendChild( pXmlNewTextNode, &pTempNode );
				RELEASEINTERFACE( pXmlNewTextNode );
				RELEASEINTERFACE( pTempNode );

				MSXML2::IXMLDOMNode *pXmlNewBrNode = NULL; 
				m_pXmlDoc->raw_createNode( (const _variant_t &) int( 1 ), L"br", L"", &pXmlNewBrNode );

				pXmlNode->raw_appendChild( pXmlNewBrNode, &pTempNode );
				RELEASEINTERFACE( pXmlNewBrNode );
				RELEASEINTERFACE( pTempNode );

				tmp = tmp.substr( br + 1 );
				br  = tmp.find( ( wchar_t ) 13 );
			}			
			int p = tmp.size();
			if (tmp.size() != 0)
			{
				MSXML2::IXMLDOMNode *pXmlNewNode = NULL;
				m_pXmlDoc->raw_createNode( (const _variant_t &) int( 3 ), L"", L"", &pXmlNewNode );
				pXmlNewNode->put_nodeValue( (const _variant_t &) _bstr_t( tmp.c_str() ) );

				MSXML2::IXMLDOMNode *pTempNode = NULL;
				pXmlNode->raw_appendChild( pXmlNewNode, &pTempNode );

				RELEASEINTERFACE( pXmlNewNode );
				RELEASEINTERFACE( pTempNode );
			}
		}

		if ( pHtmlNode.child( i ) != NULL )
		{
			htmlayout::dom::element html_new_node_ptr( pHtmlNode.child( i ) );
			_bstr_t new_el_type = strings::make_lower( _bstr_t( html_new_node_ptr.get_element_type() ) );

			if ( new_el_type == _bstr_t( L"text" ) )
				new_el_type = _bstr_t( L"div" );

			int k = html_new_node_ptr.children_count();
			//TODO когда будет поддерживаться colgroup, form и style исправить
			if ( new_el_type != _bstr_t( L"script" ) 
				&& new_el_type != _bstr_t( L"colgroup")	
				&& new_el_type != _bstr_t( L"col")
				&& new_el_type != _bstr_t( L"style") 
				&& new_el_type.length() != 0)
			{
				MSXML2::IXMLDOMNode *pXmlNewNode = NULL;
				m_pXmlDoc->raw_createNode( (const _variant_t &) int( 1 ), new_el_type, L"", &pXmlNewNode );
				
				nodeHtmlToXhtml( html_new_node_ptr, pXmlNewNode, dNewEmValue );

				MSXML2::IXMLDOMNode *pTempNode = NULL;
				pXmlNode->raw_appendChild( pXmlNewNode, &pTempNode );

				RELEASEINTERFACE( pXmlNewNode );
				RELEASEINTERFACE( pTempNode );
			}
		}
	}
}


void HtmlToXhtml::nodeHtmlToXhtml( MSHTML::IHTMLDOMNodePtr pHtmlNode, MSXML2::IXMLDOMNodePtr pXmlNode )
{
	HRESULT hr;
	if ( !pHtmlNode || !pXmlNode )
		return;

	_bstr_t el_name = pHtmlNode->GetnodeName();

	MSXML2::IXMLDOMElementPtr xmlElementPtr = pXmlNode;
	IDispatchPtr as;
	MSHTML::IHTMLAttributeCollectionPtr attrs;

	IDispatchPtr a;
	MSHTML::IHTMLDOMAttribute2Ptr attr;	
	MSHTML::IHTMLDOMAttributePtr attribute;	

	IDispatchPtr chs;	
	MSHTML::IHTMLDOMChildrenCollectionPtr childs;

	IDispatchPtr ch;
	MSHTML::IHTMLDOMNodePtr child;
	MSHTML::IHTMLDOMTextNodePtr textPtr;
		
	as = pHtmlNode->Getattributes();
	hr = as->QueryInterface(IID_IHTMLAttributeCollection, (void**) &attrs);

	
	if (SUCCEEDED(hr))
	{		
		VARIANT vACIndex;
		vACIndex.vt = VT_I4;	
		
		for (int i = 0; i < attrs->Getlength(); i++)
		{
			vACIndex.lVal = i;
			//_variant_t index = i;
			a = attrs->item(&vACIndex);
			hr = a->QueryInterface(IID_IHTMLDOMAttribute2, (void**) &attr);
			
			if (SUCCEEDED(hr))
			{
				hr = attr->QueryInterface(IID_IHTMLDOMAttribute, (void**) &attribute);
				if (attribute->Getspecified())
				{					
					bool bSkip = false;
					CString sAttrName	= ((std::wstring)attr->Getname()).c_str();
					CString sValue		= ((std::wstring)attr->Getvalue()).c_str();
					if (el_name == _bstr_t(L"IMG"))
					{
						int k = 0;
					}
					if ((el_name == _bstr_t(L"IMG")) && ((sAttrName == _T("src"))))
					{
						if ( false == m_bMht )
						{
							DecodeUrl( &sValue );
							std::wstring wsResultPath = _T("");
							std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

							// Путь к файлу может быть либо полным, либо относительным, либо ссылкой на скачивание в интернете
							if ( FileExist( &sValue ) )
							{
								wsResultPath = std::wstring( sValue.GetBuffer() );
							}
							else if ( FileExist( &CString( wsFromRelativePath.c_str() ) ) )
							{
								wsResultPath = wsFromRelativePath;
							}
							else
							{
								CString sPath = sValue;
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

							bSkip = ( wsResultPath == L"" );

							if ( !bSkip )
							{
								MSHTML::IHTMLElement4Ptr p;
								int nWidth  = ((MSHTML::IHTMLElementPtr)pHtmlNode)->getAttribute(_bstr_t(L"width"), 0);
								int nHeight = ((MSHTML::IHTMLElementPtr)pHtmlNode)->getAttribute(_bstr_t(L"height"), 0);

								Gdiplus::GdiplusStartupInput  oInput;
								Gdiplus::GdiplusStartupOutput oOutput;
								ULONG dwToken = 0;
								Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
								if ( eStatus == Gdiplus::Ok )
								{
									Gdiplus::Bitmap oImage( wsResultPath.c_str() );						
									setAttrs( nWidth, nHeight, oImage );
									xmlElementPtr->setAttribute( _bstr_t( L"width" ), (const _variant_t &) nWidth );
									xmlElementPtr->setAttribute( _bstr_t( L"height" ), (const _variant_t &) nHeight );
								}
								Gdiplus::GdiplusShutdown( dwToken );

								xmlElementPtr->setAttribute( _bstr_t( sAttrName.GetBuffer() ), (const _variant_t &) _bstr_t( wsResultPath.c_str() ) );
								bSkip = true;
							}
						}
						else
						{
							//std::wstring strTempPath = CHtmlUtility::GetTempName();
							//bSkip = !m_oMhtFile.savePicture( (const std::wstring) sValue.GetBuffer(), strTempPath );
							//if ( !bSkip )
							//{
							//	int nWidth  = _wtoi( pHtmlNode.get_style_attribute( "width" ) );
							//	int nHeight = _wtoi( pHtmlNode.get_style_attribute( "height" ) );

							//	Gdiplus::GdiplusStartupInput  oInput;
							//	Gdiplus::GdiplusStartupOutput oOutput;
							//	ULONG dwToken = 0;
							//	Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
							//	if ( eStatus == Gdiplus::Ok )
							//	{
							//		Gdiplus::Bitmap oImage( strTempPath.c_str() );						
							//		setAttrs( nWidth, nHeight, oImage );
							//		xml_element_ptr->setAttribute( _bstr_t( L"width" ), (const _variant_t &)nWidth );
							//		xml_element_ptr->setAttribute( _bstr_t( L"height" ), (const _variant_t &)nHeight );
							//	}
							//	Gdiplus::GdiplusShutdown( dwToken );
							//}
						}
					}
					else if ( (el_name == _bstr_t( L"A" )) && ( sAttrName == _T("href") ) )
					{
						if ( false == m_bMht )
						{
							DecodeUrl( &sValue );
							std::wstring wsResultPath = _T("");
							std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

							if (  -1 == sValue.Find( _T("www") ) && -1 == sValue.Find( _T("http") ) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) ) 				
								wsResultPath = L"file:///" + wsFromRelativePath;				
							else
								wsResultPath = sValue;

							bSkip = ( wsResultPath == L"");
							wsResultPath = strings::replace_all(wsResultPath, L" ", L"%20");

							if ( !bSkip )
							{
								xmlElementPtr->setAttribute( _bstr_t( sAttrName.GetBuffer() ), (const _variant_t &) _bstr_t( wsResultPath.c_str() ) );
								bSkip = true;
							}
						}
					}
					else if ( (el_name == _bstr_t( L"frame" )) && ( sAttrName == _T("src") ) )
					{
						//DecodeUrl( &sValue );
						//std::wstring wsResultPath = _T("");
						//std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

						//if (  -1 == sValue.Find( _T("www") ) && -1 == sValue.Find( _T("http") ) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) )
						//{
						//	if ( (-1 != sValue.Find( _T(".htm") )) ||(-1 != sValue.Find( _T(".html") )) )
						//	{
						//		HtmlToXhtml oHtmlToXhtml( wsFromRelativePath.c_str(), L"", NULL );

						//		bool bResult = oHtmlToXhtml.Convert();
						//		if (bResult == true)
						//		{
						//			MSXML2::IXMLDOMNode *pTempNode = NULL;
						//			pXmlNode->raw_appendChild( oHtmlToXhtml.getBody(), &pTempNode );
						//			AddTempImages(oHtmlToXhtml.GetTempImages());
						//		}
						//	}
						//}
						break;
					}
					else if ( (el_name == _bstr_t( L"iframe" )) && ( sAttrName == _T("src") ) )
					{
						//DecodeUrl( &sValue );
						//std::wstring wsResultPath = _T("");
						//std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) sValue.GetBuffer() );

						//if (  -1 == sValue.Find( _T("www") ) && -1 == sValue.Find( _T("http") ) && -1 == sValue.Find( _T("ftp") ) && -1 == sValue.Find( _T(":") ) )
						//{
						//	if ( (-1 != sValue.Find( _T(".htm") )) ||(-1 != sValue.Find( _T(".html") )) )
						//	{
						//		HtmlToXhtml oHtmlToXhtml( wsFromRelativePath.c_str(), L"", NULL );

						//		bool bResult = oHtmlToXhtml.Convert();
						//		if (bResult == true)
						//		{
						//			MSXML2::IXMLDOMNode *pTempNode = NULL;
						//			pXmlNode->raw_appendChild( oHtmlToXhtml.getBody(), &pTempNode );
						//			AddTempImages(oHtmlToXhtml.GetTempImages());
						//		}
						//	}
						//}
						break;
					}
					if ( ( sAttrName == _T("style") ) || ( sAttrName == _T("class") ) || ( sAttrName == _T("xmlns") ) || ( sValue == _T("") ) )
						bSkip = true;
					if ( !bSkip )
					{
						HRESULT hRes = xmlElementPtr->setAttribute( _bstr_t( sAttrName.GetBuffer() ), (const _variant_t &)sValue.GetBuffer() );
					}
					//xmlElementPtr->setAttribute(_bstr_t( sAttrName.GetBuffer(), (const _variant_t &)_bstr_t( sValue.GetBuffer());
				}					
			}
		}
	}		
	
	chs = pHtmlNode->GetchildNodes();
	hr = chs->QueryInterface(IID_IHTMLDOMChildrenCollection, (void**) &childs);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < childs->Getlength(); i++)	
		{
			ch = childs->item(i);
			hr = ch->QueryInterface(IID_IHTMLDOMNode, (void**) &child);			
			
			if (SUCCEEDED(hr))
			{
				long child_type = child->GetnodeType();
				if(child_type == 1)
				{
					std::wstring child_name = child->GetnodeName();
					MSXML2::IXMLDOMNode *pXmlNewNode = NULL;
					m_pXmlDoc->raw_createNode((const _variant_t &) int(1), (_bstr_t)(ToLower(child_name).c_str()), L"", &pXmlNewNode );
					
					nodeHtmlToXhtml( child, pXmlNewNode );

					MSXML2::IXMLDOMNode *pTempNode = NULL;
					pXmlNode->raw_appendChild( pXmlNewNode, &pTempNode );
					RELEASEINTERFACE( pXmlNewNode );
					RELEASEINTERFACE( pTempNode );
				}
				else if (child_type == 3)
				{
					hr = ch->QueryInterface(IID_IHTMLDOMTextNode, (void**) &textPtr);

					MSXML2::IXMLDOMNode *pXmlNewNode = NULL;
					m_pXmlDoc->raw_createNode((const _variant_t &) int(3), L"", L"", &pXmlNewNode );
					pXmlNewNode->put_nodeValue((const _variant_t &) textPtr->toString());

					MSXML2::IXMLDOMNode *pTempNode = NULL;
					pXmlNode->raw_appendChild( pXmlNewNode, &pTempNode );

					RELEASEINTERFACE( pXmlNewNode );
					RELEASEINTERFACE( pTempNode );
				}
			}
		}
	}
}


void HtmlToXhtml::setAttrs(int & width, int & height, Gdiplus::Bitmap& img)
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



const std::wstring HtmlToXhtml::encoding(const std::wstring& line) const 
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


std::wstring HtmlToXhtml::loadCSS( const std::wstring & sCSS )
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


void HtmlToXhtml::parseCSS( const std::wstring & sCSS )
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


void HtmlToXhtml::InsertPair( wstrmap& styleMap, const std::wstring &key, const std::wstring &val )
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


std::wstring HtmlToXhtml::getElementCSS( htmlayout::dom::element pHtmlNode )
{
    std::wstring res;

    for (uint i = 0; i < 4; ++i)
    {
        for (wstrmap::iterator itr = m_mStyles[i].begin(); itr != m_mStyles[i].end(); ++itr)
        {
			try
			{
				if (pHtmlNode.test((const char *) _bstr_t(itr->first.c_str())))
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
    res = strings::trim( res );
    return res;
}


std::wstring HtmlToXhtml::convertElementAttr(htmlayout::dom::element pHtmlNode, const std::wstring & attribute)
{
    std::wstring res;

    const wchar_t * val = pHtmlNode.get_style_attribute((const char *) _bstr_t(attribute.c_str()));
    if ( val )
    {
        res += attribute;
        res += L": ";
        res += val;
        res += L"; ";
    }

    return res;
}


std::wstring HtmlToXhtml::SetLowerStyleNames(const std::wstring& style)
{
	std::wstring new_style;
	std::wstring copy_style = style;
	std::wstring name;
	std::wstring value;
 	while (copy_style.find(L":") != std::wstring::npos)
	{
		name       = strings::substring_before(copy_style, L":");
		copy_style = strings::substring_after(copy_style, L":");
		if (copy_style.find(L";") != std::wstring::npos)
		{
			value  = strings::substring_before(copy_style, L";");
			copy_style = strings::substring_after(copy_style, L";");
		}
		else
		{
			value = copy_style;
		}
		new_style += ToLower(name);
		new_style += L":";
		new_style += value;
		new_style += L";";
	}
	return new_style;
}
void HtmlToXhtml::convertElementFont(htmlayout::dom::element pHtmlNode)
{
    const wchar_t * style_raw = pHtmlNode.get_attribute( "style" );
    if (style_raw)
    {
        std::wstring style = style_raw;
        style += L";";
        std::wstring res;
        while (style.length())
        {
            std::wstring tmp = strings::substring_before(style, L";") + L";";
            if (L"font" == strings::trim(strings::substring_before(tmp, L":")))
            {
                pHtmlNode.set_attribute("style", tmp.c_str());
                tmp = convertElementAttr(pHtmlNode, L"font-family");
                tmp += convertElementAttr(pHtmlNode, L"font-size");
                tmp += convertElementAttr(pHtmlNode, L"font-style");
                tmp += convertElementAttr(pHtmlNode, L"font-variant");
                tmp += convertElementAttr(pHtmlNode, L"font-weight");
            }
            res += tmp;
            style = strings::substring_after(style, L";");
        }
        pHtmlNode.set_attribute("style", res.c_str());
    }
}


void  HtmlToXhtml::convertElementBodyBackgroundImage( htmlayout::dom::element  pHtmlNode, MSXML2::IXMLDOMElementPtr pXmlNode )
{
	const wchar_t * style_raw = pHtmlNode.get_attribute( "style" );
	if (style_raw)
    {
		bool bSkip = false;
        std::wstring style = style_raw;
        style += L";";
		std::wstring image = strings::substring_after(style, L"background-image");
		image = strings::substring_between(image, L"(", L")");
		if (image.find(L"'") != std::wstring::npos)
		{			
			image = strings::substring_after(image, L"'");	
			image = strings::substring_before(image, L"'");		
		}
		else if (image.find(L"\"") != std::wstring::npos)
		{			
			image = strings::substring_after(image, L"\"");
			image = strings::substring_before(image, L"\"");
		}
		CString imageName = image.c_str();
		if (false == m_bMht)
		{
			DecodeUrl( &imageName );
			std::wstring wsResultPath = _T("");
			std::wstring wsFromRelativePath = full_from_relative( (const std::wstring) m_sHtmlPath.GetBuffer(), (const std::wstring) imageName.GetBuffer() );

			// Путь к файлу может быть либо полным, либо относительным, либо ссылкой на скачивание в интернете
			if ( FileExist( &imageName ) )
			{
				wsResultPath = std::wstring( imageName.GetBuffer() );
			}
			else if ( FileExist( &CString( wsFromRelativePath.c_str() ) ) )
			{
				wsResultPath = wsFromRelativePath;
			}
			else
			{
				CString sPath = imageName;
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

			bSkip = ( wsResultPath == L"" );

			if ( !bSkip )
			{
				int nWidth  = _wtoi( pHtmlNode.get_style_attribute( "width" ) );
				int nHeight = _wtoi( pHtmlNode.get_style_attribute( "height" ) );

				Gdiplus::GdiplusStartupInput  oInput;
				Gdiplus::GdiplusStartupOutput oOutput;
				ULONG dwToken = 0;
				Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
				if ( eStatus == Gdiplus::Ok )
				{
					Gdiplus::Bitmap oImage( wsResultPath.c_str() );						
					setAttrs( nWidth, nHeight, oImage );
					pXmlNode->setAttribute( _bstr_t( L"background-image" ), (const _variant_t &)image.c_str() );
					pXmlNode->setAttribute( _bstr_t( L"background-image-width" ), (const _variant_t &)nWidth );
					
				}
				Gdiplus::GdiplusShutdown( dwToken );

				pXmlNode->setAttribute( _bstr_t( L"background-image-height" ), (const _variant_t &)nHeight );
			}
		}
		else
		{
			std::wstring strTempPath = CHtmlUtility::GetTempName();
			bSkip = !m_oMhtFile.savePicture( (const std::wstring) imageName.GetBuffer(), strTempPath );
			if ( !bSkip )
			{
				int nWidth = 0;
				int nHeight = 0;

				Gdiplus::GdiplusStartupInput  oInput;
				Gdiplus::GdiplusStartupOutput oOutput;
				ULONG dwToken = 0;
				Gdiplus::Status eStatus = Gdiplus::GdiplusStartup( &dwToken, &oInput, &oOutput );
				if ( eStatus == Gdiplus::Ok )
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
					pXmlNode->setAttribute( _bstr_t( L"background-image" ), (const _variant_t &)image.c_str() );
					pXmlNode->setAttribute( _bstr_t( L"background-image-width" ), (const _variant_t &)nWidth );
					pXmlNode->setAttribute( _bstr_t( L"background-image-height" ), (const _variant_t &)nHeight );
				}
				Gdiplus::GdiplusShutdown( dwToken );
			}
		}
    }
}


std::wstring HtmlToXhtml::convertElementCSS( htmlayout::dom::element pHtmlNode )
{
    std::wstring res;

    // Currently method is not used.

    res += convertElementAttr( pHtmlNode, L"font-family" );
    res += convertElementAttr( pHtmlNode, L"font-size" );
    res += convertElementAttr( pHtmlNode, L"font-style" );
    res += convertElementAttr( pHtmlNode, L"font-variant" );
    res += convertElementAttr( pHtmlNode, L"font-weight" );
    res += convertElementAttr( pHtmlNode, L"line-height" );
    res += convertElementAttr( pHtmlNode, L"text-align" );
    res += convertElementAttr( pHtmlNode, L"text-decoration	" );
    res += convertElementAttr( pHtmlNode, L"text-indent" );
    res += convertElementAttr( pHtmlNode, L"text-transform" );
    res += convertElementAttr( pHtmlNode, L"vertical-align" );
    res += convertElementAttr( pHtmlNode, L"white-space" );
    res += convertElementAttr( pHtmlNode, L"background-attachment" );
    res += convertElementAttr( pHtmlNode, L"background-color" );
    res += convertElementAttr( pHtmlNode, L"background-image" );
    res += convertElementAttr( pHtmlNode, L"background-position-left" );
    res += convertElementAttr( pHtmlNode, L"background-position-right" );
    res += convertElementAttr( pHtmlNode, L"background-position-top" );
    res += convertElementAttr( pHtmlNode, L"background-position-bottom" );
    res += convertElementAttr( pHtmlNode, L"background-repeat" );
    res += convertElementAttr( pHtmlNode, L"color" );
    res += convertElementAttr( pHtmlNode, L"border-bottom-color" );
    res += convertElementAttr( pHtmlNode, L"border-bottom-style" );
    res += convertElementAttr( pHtmlNode, L"border-bottom-width" );
    res += convertElementAttr( pHtmlNode, L"border-left-color" );
    res += convertElementAttr( pHtmlNode, L"border-left-style" );
    res += convertElementAttr( pHtmlNode, L"border-left-width" );
    res += convertElementAttr( pHtmlNode, L"border-right-color" );
    res += convertElementAttr( pHtmlNode, L"border-right-style" );
    res += convertElementAttr( pHtmlNode, L"border-right-width" );
    res += convertElementAttr( pHtmlNode, L"border-top-color" );
    res += convertElementAttr( pHtmlNode, L"border-top-style" );
    res += convertElementAttr( pHtmlNode, L"border-top-width" );
    res += convertElementAttr( pHtmlNode, L"border-collapse" );
    res += convertElementAttr( pHtmlNode, L"clear" );
    res += convertElementAttr( pHtmlNode, L"float" );
    res += convertElementAttr( pHtmlNode, L"margin-bottom" );
    res += convertElementAttr( pHtmlNode, L"margin-left" );
    res += convertElementAttr( pHtmlNode, L"margin-right" );
    res += convertElementAttr( pHtmlNode, L"margin-top" );
    res += convertElementAttr( pHtmlNode, L"padding-bottom" );
    res += convertElementAttr( pHtmlNode, L"padding-left" );
    res += convertElementAttr( pHtmlNode, L"padding-right" );
    res += convertElementAttr( pHtmlNode, L"padding-top" );
    res += convertElementAttr( pHtmlNode, L"list-style-image" );
    res += convertElementAttr( pHtmlNode, L"list-style-position" );
    res += convertElementAttr( pHtmlNode, L"list-style-type" );
    res += convertElementAttr( pHtmlNode, L"position" );
    res += convertElementAttr( pHtmlNode, L"left" );
    res += convertElementAttr( pHtmlNode, L"right" );
    res += convertElementAttr( pHtmlNode, L"top" );
    res += convertElementAttr( pHtmlNode, L"bottom" );
    res += convertElementAttr( pHtmlNode, L"height" );
    res += convertElementAttr( pHtmlNode, L"z-index" );
    res += convertElementAttr( pHtmlNode, L"overflow-x" );
    res += convertElementAttr( pHtmlNode, L"overflow-y" );
    res += convertElementAttr( pHtmlNode, L"width" );
    res += convertElementAttr( pHtmlNode, L"min-height" );
    res += convertElementAttr( pHtmlNode, L"min-width" );
    res += convertElementAttr( pHtmlNode, L"max-height" );
    res += convertElementAttr( pHtmlNode, L"max-width" );
    res += convertElementAttr( pHtmlNode, L"outline-width" );
    res += convertElementAttr( pHtmlNode, L"outline-style" );
    res += convertElementAttr( pHtmlNode, L"outline-color" );
    res += convertElementAttr( pHtmlNode, L"outline-offset" );
    res += convertElementAttr( pHtmlNode, L"cursor" );
    res += convertElementAttr( pHtmlNode, L"display" );
    res += convertElementAttr( pHtmlNode, L"visibility" );
    res += convertElementAttr( pHtmlNode, L"page-break-before" );
    res += convertElementAttr( pHtmlNode, L"page-break-after" );

    return res;
}


bool HtmlToXhtml::LoadExternalCSS( CString sHref, std::wstring *pwsText, CString sPath, BOOL bWeb )
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

		std::wstring wsPath( oDownloader.GetFilePath().GetBuffer() );
		TxtFile oFile( wsPath );

		std::list<std::wstring> oContent;

		if ( oFile.isUtf8() )
			oContent = transform( oFile.readUtf8(), Encoding::utf82unicode );
		else if ( oFile.isUnicode() )
			oContent = oFile.readUnicode();
		else
			oContent = transform( oFile.readAnsi(), Encoding::ansi2unicode);

		for(std::list<std::wstring>::iterator list_iter = oContent.begin(); 
			list_iter != oContent.end(); list_iter++)
		{
			*pwsText += list_iter->c_str();
		}

		*pwsText = strings::cut_all( *pwsText, L"/*", L"*/" );

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

		TxtFile oFile( std::wstring( sFullPath.GetBuffer() ) );

		std::list<std::wstring> oContent;

		if ( oFile.isUtf8() )
			oContent = transform( oFile.readUtf8(), Encoding::utf82unicode );
		else if ( oFile.isUnicode() )
			oContent = oFile.readUnicode();
		else
			oContent = transform( oFile.readAnsi(), Encoding::ansi2unicode);

		for(std::list<std::wstring>::iterator list_iter = oContent.begin(); 
			list_iter != oContent.end(); list_iter++)
		{
			*pwsText += list_iter->c_str();
		}

		*pwsText = strings::cut_all( *pwsText, L"/*", L"*/" );

		return true;
	}
}


bool HtmlToXhtml::LoadHtml_htmlayout()
{
	if ( false == m_bMht )
	{
		//IWebBrowser2 *pBrowser2;
		//::CoCreateInstance( CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pBrowser2);
		//if (pBrowser2)
		//{
		//	VARIANT vEmpty;
		//	VariantInit(&vEmpty);       
		//	BSTR bsPath = CString( m_sHtmlPath.c_str() ).AllocSysString();
		//	HRESULT hr = pBrowser2->Navigate( bsPath, &vEmpty, &vEmpty, &vEmpty, &vEmpty );
		//	::SysFreeString( bsPath );

		//	if ( SUCCEEDED(hr) )
		//	{
		//		pBrowser2->get_Document( (IDispatch **)&m_pHtmlDoc );
		//	}
		//	RELEASEINTERFACE( pBrowser2 );
		//}

		if ( !m_pBuffer || m_dwBufferSize <= 0 )
			return false;

		bool bResult;		
		bResult = loadFile((LPCBYTE)m_pBuffer, m_dwBufferSize );

		return bResult;    
	}
	else
	{
		std::wstring wsFileText = m_oMhtFile.getHtmlFile();
		return m_oHtmLite.load( ( LPCBYTE )wsFileText.c_str(), wsFileText.length() * 2 );
	} 
}



bool HtmlToXhtml::LoadHtml_mshtml()
{
	IWebBrowser2 *pBrowser2;
	HRESULT hr = ::CoCreateInstance( CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pBrowser2);
	if (pBrowser2)
	{
		VARIANT vEmpty;
		VariantInit(&vEmpty);  

		BSTR bsPath = m_sHtmlPath.AllocSysString();			
		HRESULT hr = pBrowser2->Navigate( bsPath, &vEmpty, &vEmpty, &vEmpty, &vEmpty );
		::SysFreeString( bsPath );

		bool res;
		if (SUCCEEDED(hr))
		{
			IDispatchPtr spDisp;
			res = pBrowser2->get_Document(&spDisp) == S_OK && spDisp!= 0;			
			IDispatchPtr ptr = m_pHtmlDoc->open(bsPath);
			if(res)
			{	
				spDisp->QueryInterface(IID_IHTMLDocument2, (void**) &m_pHtmlDoc);				
			}
		}
		RELEASEINTERFACE( pBrowser2 );
		return res;
	}
	return false;
}



const std::wstring HtmlToXhtml::file2ansi(const std::vector<std::string>& file, const bool flag) const
{
	std::wstring ansiFile;

	std::string code;
	for(std::vector<std::string>::const_iterator i = file.begin(); i != file.end(); )
	{
		std::string lowerLine = ToLower(*i);
		std::string::size_type meta = lowerLine.find("<meta");
		std::string::size_type xmlEncode = lowerLine.find("<?xml");
		if (xmlEncode != std::string::npos )
		{
			std::string xmlTeg = lowerLine.substr(xmlEncode + 5, i->length() - xmlEncode - 5);
			std::string::size_type ugl = xmlTeg.find(">");
			while( ugl == std::string::npos)
			{
				xmlTeg += ToLower(*i++);
				ugl = xmlTeg.find(">");
			}
			xmlTeg = xmlTeg.substr(0, ugl - 1);
			std::string::size_type encoding = xmlTeg.find("encoding=");
			if (encoding != std::string::npos)
			{
				code = xmlTeg.substr(encoding + 10, xmlTeg.length() - encoding - 10);
				break;
			}
			i++;
		}
		else if (meta != std::string::npos)
		{
			std::string metaTeg = lowerLine.substr(meta + 5, i->length() - meta - 5);
			std::string::size_type ugl = metaTeg.find(">");
			while( ugl == std::string::npos)
			{
				metaTeg += ToLower(*i++);
				ugl = metaTeg.find(">");
			}
			metaTeg = metaTeg.substr(0, ugl - 1);
			std::string::size_type charset = metaTeg.find("charset=");
			if (charset != std::string::npos)
			{
				code = metaTeg.substr(charset + 8, metaTeg.length() - charset - 8);
				break;
			}
			i++;
		}
		else if (ToLower(*i).find("</head>") == std::string::npos)
		{
			i++;
		}
		else
		{
			break;
		}
	}
	
	if ( code.find("utf-8") != std::string::npos && flag == false)
		BOOST_FOREACH(const std::string& line, file)
		{
			std::wstring new_line(line.size(), 0);
			utf8_decode( line.begin(), line.end(), new_line.begin() );
						
			ansiFile += L" " + new_line;
		}
	else
		BOOST_FOREACH(const std::string& line, file)
		{
			ansiFile += Encoding::ansi2unicode(line);
		}	

	return ansiFile;
}


_bstr_t HtmlToXhtml::delComment(_bstr_t style)
{
	std::wstring new_style;
	std::wstring old_style = style.GetBSTR();
	new_style = strings::cut_all(old_style, L"<!--", L"-->");
	new_style = strings::cut_all(new_style, L"/*", L"*/");
	new_style = strings::cut_all(new_style, L"@", L";");
	return new_style.c_str();
}


_bstr_t HtmlToXhtml::FindAttribute(const htmlayout::dom::element oElement, const _bstr_t bsAttrName, const _bstr_t bsDefaultValue)
{
    _bstr_t bsResult = bsDefaultValue;

	const char *sAttrName = (const char *)bsAttrName;

    const wchar_t *wsResult = oElement.get_attribute( sAttrName );

    if ( wsResult )
    {
        bsResult = wsResult;
    }

    return bsResult;
}


_bstr_t HtmlToXhtml::getFilePath()
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


bool HtmlToXhtml::Convert_htmlayout()
{
	_bstr_t bsStyle;
	_bstr_t bsTitle;
	
	if ( !LoadHtml_htmlayout() )
		return false;

	HELEMENT pElement = m_oHtmLite.getRootElement();
	htmlayout::dom::element oHtmlRoot( pElement );

	pElement = oHtmlRoot.find_first( "head" );

	if ( pElement )
	{
		htmlayout::dom::element oHtmlHead( pElement );

		pElement = oHtmlHead.find_first( "title" );
		if ( pElement )
		{
			htmlayout::dom::element oHtmlTitle( pElement );
			bsTitle = _bstr_t( oHtmlTitle.text() );
		}

		std::vector< htmlayout::dom::element > arrHtmlStyles;
		oHtmlHead.find_all( arrHtmlStyles, "style" );
		for ( std::vector< htmlayout::dom::element >::iterator pIter = arrHtmlStyles.begin(); pIter != arrHtmlStyles.end(); ++pIter )
		{
			bsStyle += _bstr_t( pIter->text() );
		}

		//// TO DO: К сожалению, тэг Base Htmlayout не видит
		//oHtmlHead.find_all( arrHtmlStyles, "base" );
		//for ( std::vector< htmlayout::dom::element >::iterator pIter = arrHtmlStyles.begin(); pIter != arrHtmlStyles.end(); ++pIter )
		//{
		//	_bstr_t bsHref = FindAttribute( *pIter, _bstr_t( _T("href") ) );
		//	if ( _T("") != bsHref )
		//	{
		//		m_sBase = CString( bsHref.GetBSTR() );
		//		DecodeUrl( &m_sBase );
		//		break;
		//	}
		//}

		oHtmlHead.find_all( arrHtmlStyles, "link" );
		for ( std::vector< htmlayout::dom::element >::iterator pIter = arrHtmlStyles.begin(); pIter != arrHtmlStyles.end(); ++pIter )
		{
			_bstr_t bsRel  = FindAttribute( *pIter, _bstr_t( _T("rel") ) );
			_bstr_t bsType = FindAttribute( *pIter, _bstr_t( _T("type") ) );

			CString sRel( bsRel.GetBSTR() ), sType( bsType.GetBSTR() );
			sRel.MakeLower();
			sType.MakeLower();

			if ( _T("stylesheet") == sRel /*&& _T("text/css") == sType*/ )
			{
				BSTR bsHref = FindAttribute( *pIter, _bstr_t( L"href" ) ).GetBSTR();
				CString sHref( bsHref );
				DecodeUrl( &sHref );
				CString sPath( m_sHtmlPath );

				if ( false == m_bMht )
				{
					if ( _T("") != sHref && ( -1 != sHref.Find( _T("http") ) || -1 != sHref.Find( _T("ftp") ) ) )
					{
						std::wstring wsBuffer;
						if ( LoadExternalCSS( sHref, &wsBuffer, sPath, TRUE ) )
						{
							bsStyle += _bstr_t( wsBuffer.c_str() );
						}
					}
					else if ( _T("") != sHref )
					{
						std::wstring wsBuffer;
						if ( LoadExternalCSS( sHref, &wsBuffer, sPath, FALSE ) )
						{
							bsStyle += _bstr_t( wsBuffer.c_str() );
						}
					}
				}
				else
				{
					//std::wstring style_wstr = m_mhtFile.getStyle(href).c_str();
					//style = _bstr_t(href.c_str());
					bsStyle += _bstr_t( m_oMhtFile.getStyle( bsHref ).c_str() );
				}
			}
		}
		if ( bsStyle.length() )
		{
			bsStyle = delComment( bsStyle );
			bsStyle = _bstr_t( loadCSS( bsStyle.GetBSTR() ).c_str() );
			//strings::make_lower( style );
			bsStyle = _bstr_t( strings::cut_all( bsStyle.GetBSTR(), L"/*", L"*/" ).c_str() );
			bsStyle = _bstr_t( strings::cut_all( bsStyle.GetBSTR(), L"<!--", L"-->" ).c_str() );
			
			m_oCssList.Parse( CString( bsStyle.GetBSTR() ) );
			//parseCSS( bsStyle.GetBSTR() );
		}
	}

	MSXML2::IXMLDOMNodePtr     xml_body_ptr;
	pElement = oHtmlRoot.find_first( "frameset" );
	if (pElement)
	{
		xml_body_ptr = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"frameset",  L"" );
	}	
	else
	{
		pElement = oHtmlRoot.find_first( "body" );
		if (pElement)
		{
			xml_body_ptr = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"body",  L"" );			
		}
		else
			return false;
	}

	htmlayout::dom::element html_body( pElement );

	MSXML2::IXMLDOMNodePtr     xml_html_ptr    = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"html",  L"" );
	MSXML2::IXMLDOMNodePtr     xml_head_ptr    = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"head",  L"" );
	MSXML2::IXMLDOMNodePtr     xml_title_ptr   = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"title", L"" );
	MSXML2::IXMLDOMNodePtr     xml_style_ptr   = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"style", L"" );

	xml_style_ptr->text = bsStyle;
	xml_title_ptr->text = bsTitle;

	nodeHtmlToXhtml( html_body, xml_body_ptr );

	xml_head_ptr->appendChild( xml_title_ptr );
	//xml_head_ptr->appendChild( xml_style_ptr );
	xml_html_ptr->appendChild( xml_head_ptr );
	xml_html_ptr->appendChild( xml_body_ptr );

	m_pXmlDoc->appendChild( xml_html_ptr );

	return true;
/*
	// В тексте могут встретиться плохие символы(Invalid unicode characters), поэтому перед записью Xml мы должны от них избавиться
	BSTR bsXml = NULL;
	if ( FAILED( m_pXmlDoc->get_xml( &bsXml ) ) )
		return false;

	// Удаляем неюникодные символы
	RemoveInvalidChars( &bsXml );

	VARIANT_BOOL vbSuccess = VARIANT_FALSE;
	if ( FAILED( m_pXmlDoc->raw_loadXML( bsXml, &vbSuccess ) ) || VARIANT_TRUE != vbSuccess )
	{
		::SysFreeString( bsXml );
		return false;
	}
	::SysFreeString( bsXml );

	if ( FAILED( m_pXmlDoc->raw_save( (const _variant_t &) m_sXhtmlPath.GetBuffer() ) ) )
		return false;    

	return true;*/
}


bool HtmlToXhtml::Convert_mshtml()
{
	_bstr_t bsStyle;
	_bstr_t bsTitle;
	HRESULT hr;

	if ( !LoadHtml_mshtml() )
		return false;

	//TODO добавить Frameset
	IDispatchPtr node;
	MSHTML::IHTMLDOMNodePtr bodeNode;

	MSHTML::IHTMLElementPtr headElem = NULL;
	MSHTML::IHTMLElementPtr framesetElem = NULL;
	MSHTML::IHTMLElementPtr bodyElem = m_pHtmlDoc->Getbody();
		
	IDispatchPtr lpItem;
	MSHTML::IHTMLElementPtr element;
	
	IDispatchPtr cols;
	MSHTML::IHTMLElementCollectionPtr headCollection;

	MSHTML::IHTMLElementCollectionPtr htmlCollection = m_pHtmlDoc->Getall();
	for(int i = 0; i < htmlCollection->Getlength(); i++)
	{
		lpItem = htmlCollection->item(CComVariant(i), CComVariant(i));		
		hr = lpItem->QueryInterface(IID_IHTMLElement, (void**) &element);
		if(SUCCEEDED(hr))
		{		  
			_bstr_t name  = element->GettagName();
			if (name == (_bstr_t)L"HEAD" )
			{
				headElem = element;
				break;
			}			
		}
	}



	if (headElem != NULL)
	{
		cols = headElem->Getchildren();
		hr = cols->QueryInterface(IID_IHTMLElementCollection, (void**) &headCollection);

		for(int i = 0; i < headCollection->Getlength(); i++)
		{
			lpItem = headCollection->item(CComVariant(i), CComVariant(i));			
			hr = lpItem->QueryInterface(IID_IHTMLElement, (void**) &element);
			if(SUCCEEDED(hr))
			{		  
				_bstr_t name  = element->GettagName();
				if (name == (_bstr_t)L"TITLE")
					bsTitle = element->GetinnerHTML();
				else if (name == (_bstr_t)L"STYLE")
					bsStyle = element->GetinnerHTML();
				else if (name == (_bstr_t)L"LINK")
				{
					_bstr_t bsRel  = element->getAttribute((_bstr_t)L"rel", 0);
					_bstr_t bsType = element->getAttribute((_bstr_t)L"type", 0);
					CString sRel( bsRel.GetBSTR() );
					CString sType( bsType.GetBSTR() );
					sRel.MakeLower();
					sType.MakeLower();
					if ( _T("stylesheet") == sRel && _T("text/css") == sType )
					{
						BSTR bsHref = ((_bstr_t)element->getAttribute((_bstr_t)L"href", 0)).GetBSTR();
						CString sHref(bsHref);
						DecodeUrl(&sHref);
						CString sPath(m_sHtmlPath);

						if (false == m_bMht)
						{
							if ( _T("") != sHref && ( -1 != sHref.Find( _T("http") ) || -1 != sHref.Find( _T("ftp") ) ) )
							{
								std::wstring wsBuffer;
								if ( LoadExternalCSS( sHref, &wsBuffer, sPath, TRUE ) )
								{
									bsStyle += _bstr_t( wsBuffer.c_str() );
								}
							}
							else if ( _T("") != sHref )
							{
								std::wstring wsBuffer;
								if ( LoadExternalCSS( sHref, &wsBuffer, sPath, FALSE ) )
								{
									bsStyle += _bstr_t( wsBuffer.c_str() );
								}
							}
						}
						else
						{
							//std::wstring style_wstr = m_mhtFile.getStyle(href).c_str();
							//style = _bstr_t(href.c_str());
							bsStyle += _bstr_t(m_oMhtFile.getStyle(bsHref).c_str());
						}
					}
				}				
			}			
		}
		if ( bsStyle.length() )
		{
			bsStyle = delComment( bsStyle );
			bsStyle = _bstr_t( loadCSS( bsStyle.GetBSTR() ).c_str() );
			//strings::make_lower( style );			
			parseCSS( bsStyle.GetBSTR() );
		}
	}	

	MSXML2::IXMLDOMNodePtr     xmlHtmlPtr    = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"html",  L"" );
	MSXML2::IXMLDOMNodePtr     xmlHeadPtr    = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"head",  L"" );
	MSXML2::IXMLDOMNodePtr     xmlBodyPtr    = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"body",  L"" );
	MSXML2::IXMLDOMNodePtr     xmlTitlePtr   = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"title", L"" );
	MSXML2::IXMLDOMNodePtr     xmlStylePtr   = m_pXmlDoc->createNode( MSXML2::NODE_ELEMENT, L"style", L"" );

	xmlStylePtr->text = bsStyle;
	xmlTitlePtr->text = bsTitle;
	
	hr = bodyElem->QueryInterface(IID_IHTMLDOMNode, (void**) &bodeNode);

	nodeHtmlToXhtml( bodeNode, xmlBodyPtr );

	xmlHeadPtr->appendChild( xmlTitlePtr );
	xmlHeadPtr->appendChild( xmlStylePtr );
	xmlHtmlPtr->appendChild( xmlHeadPtr );
	xmlHtmlPtr->appendChild( xmlBodyPtr );

	m_pXmlDoc->appendChild( xmlHtmlPtr );	 

	return true;
}

bool HtmlToXhtml::WriteXHTML()
{
	// В тексте могут встретиться плохие символы(Invalid unicode characters), поэтому перед записью Xml мы должны от них избавиться
	BSTR bsXml = NULL;
	if ( FAILED( m_pXmlDoc->get_xml( &bsXml ) ) )
		return false;

	// Удаляем неюникодные символы
	RemoveInvalidChars( &bsXml );

	VARIANT_BOOL vbSuccess = VARIANT_FALSE;
	if ( FAILED( m_pXmlDoc->raw_loadXML( bsXml, &vbSuccess ) ) || VARIANT_TRUE != vbSuccess )
	{
		::SysFreeString( bsXml );
		return false;
	}
	::SysFreeString( bsXml );

	if ( FAILED( m_pXmlDoc->raw_save( (const _variant_t &) m_sXhtmlPath.GetBuffer() ) ) )
		return false;    

	return true;
}



void HtmlToXhtml::PrepareHTML()
{
	// В данной функции мы загружаем весь файл в буффер m_pBuffer, после чего очищаем его от записей <script>..</script> и разбираем кодировку

	CString sPath( m_sHtmlPath );
	if ( !FileExist( &sPath ) )
		return;

	int nType = 0;
	TxtFile oTxtFile( std::wstring( sPath.GetBuffer() ) );

	if ( oTxtFile.isUtf8() )
		nType = 1;
	else if ( oTxtFile.isUnicode() )
		nType = 2;
	else if ( oTxtFile.isBigEndian() )
		nType = 3;
	else if ( oTxtFile.isUnicodeWithOutBOM() )
		nType = 4;

	// открываем файл
	HANDLE hFile = CreateFileW( sPath.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( INVALID_HANDLE_VALUE == hFile )
		return;

	// мапим этот файл в память - так быстрее читаются данные из файла
	DWORD  nFileSize = GetFileSize( hFile, NULL );
	HANDLE hMapFile  = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL );
	if ( NULL == hMapFile )
	{
		CloseHandle( hFile );
		return; // Невозможно создать отображение файла
	}

	// создаем view of file
	void* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
	if ( !pBaseAddress )
	{
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return;
	}

	// Если кодировка задавалась изначально в первых байтах, тогда ничего не делаем
	/*if ( 0 != nType ) 
	{
		BYTE *pBuffer = new BYTE[nFileSize];
		if ( !pBuffer )
		{
			UnmapViewOfFile( pBaseAddress );
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			return;
		}
		::memcpy( (void*)pBuffer, (const BYTE*)pBaseAddress, nFileSize );

		UnmapViewOfFile( pBaseAddress );
		CloseHandle( hMapFile );
		CloseHandle( hFile );

		m_pBuffer = pBuffer;
		m_dwBufferSize = nFileSize;

		return;
	}*/

	// Избавляемся от записей <script>...</script>

	DWORD dwSize  = nFileSize;
	//BYTE *pBuffer = (BYTE*)pBaseAddress;
	//BYTE *pTemp = new BYTE[dwSize];

	//DWORD dwCurPos = 0;
	//DWORD dwTempPos = 0;
	//while ( dwCurPos < dwSize )
	//{
	//	if ( dwCurPos + 7 < dwSize )
	//	{
	//		if ( pBuffer[dwCurPos + 0] == '<' && ( pBuffer[dwCurPos + 1] == 's' || pBuffer[dwCurPos + 1] == 'S' ) && ( pBuffer[dwCurPos + 2] == 'c' || pBuffer[dwCurPos + 2] == 'C' ) && ( pBuffer[dwCurPos + 3] == 'r' || pBuffer[dwCurPos + 3] == 'R' ) && ( pBuffer[dwCurPos + 4] == 'i' || pBuffer[dwCurPos + 4] == 'I' ) && ( pBuffer[dwCurPos + 5] == 'p' || pBuffer[dwCurPos + 5] == 'P' ) && ( pBuffer[dwCurPos + 6] == 't' || pBuffer[dwCurPos + 6] == 'T' ) )
	//		{
	//			while ( dwCurPos < dwSize )
	//			{
	//				if ( dwCurPos + 9 < dwSize )
	//				{
	//					if ( pBuffer[dwCurPos + 0] == '<' && pBuffer[dwCurPos + 1] == '/' && ( pBuffer[dwCurPos + 2] == 's' || pBuffer[dwCurPos + 2] == 'S' ) && ( pBuffer[dwCurPos + 3] == 'c' || pBuffer[dwCurPos + 3] == 'C' ) && ( pBuffer[dwCurPos + 4] == 'r' || pBuffer[dwCurPos + 4] == 'R' ) && ( pBuffer[dwCurPos + 5] == 'i' || pBuffer[dwCurPos + 5] == 'I' ) && ( pBuffer[dwCurPos + 6] == 'p' || pBuffer[dwCurPos + 6] == 'P' ) && ( pBuffer[dwCurPos + 7] == 't' || pBuffer[dwCurPos + 7] == 'T' ) && pBuffer[dwCurPos + 8] == '>')
	//					{
	//						dwCurPos = dwCurPos + 9;
	//						break;
	//					}
	//					else 
	//						dwCurPos++;
	//				}
	//				else
	//				{
	//					dwCurPos = dwSize;
	//					break;
	//				}
	//			}
	//			if ( dwCurPos >= dwSize )
	//				break;
	//		}
	//		else
	//		{
	//			pTemp[dwTempPos] = pBuffer[dwCurPos];
	//			dwTempPos++;
	//			dwCurPos++;
	//		}
	//	}
	//	else
	//	{
	//		for ( ; dwCurPos < dwSize; dwCurPos++  )
	//		{
	//			pTemp[dwTempPos] = pBuffer[dwCurPos];
	//			dwTempPos++;
	//		}
	//		break;
	//	}
	//}

	//delete []pTemp;

	CStringA sResult( (const char*)pBaseAddress,nFileSize );
	CStringA sBuffer( (const char*)pBaseAddress,nFileSize );

	// TO DO: Потом надо будет оформить это отдельной функцией
	int nLength = sBuffer.GetLength();
	LPSTR lpstrBuffer = sBuffer.GetBuffer( nLength );
	CharLowerBuffA( lpstrBuffer, nLength + 1 );
	sBuffer.ReleaseBufferSetLength( nLength );
	//sBuffer.MakeLower();

	CStringA sNewCharset( "" );
	BOOL bFindCharset = FALSE;

	int nStartPos = -1, nEndPos = -1, nEndLen = 0;
	while ( -1 != ( nStartPos = sBuffer.Find( "<script" ) )  )
	{
		if ( -1 == ( nEndPos = sBuffer.Find( "</script>", nStartPos ) ) )
		{
			// Тэг <script> не закрыт, тогда удалим только его
			if ( -1 == ( nEndPos = sBuffer.Find( ">", nStartPos ) )  )
				break;

			nEndLen = 1;
		}
		else
		{
			nEndLen = 9;
		}

		sBuffer.Delete( nStartPos, nEndPos - nStartPos + nEndLen );
		sResult.Delete( nStartPos, nEndPos - nStartPos + nEndLen );

		dwSize -= nEndPos - nStartPos + nEndLen;
	}

	nStartPos = -1, nEndPos = -1, nEndLen = 0;
	while ( -1 != ( nStartPos = sBuffer.Find( "<colgroup" ) ) )
	{
		// Есть ли еще такой тэг
		int nAnotherTagPos = sBuffer.Find( "<colgroup", nStartPos + 1 );
		int nCloseSlashPos = sBuffer.Find( ">", nStartPos );
		int nCloseTagPos   = sBuffer.Find( "</colgroup>", nStartPos );

		if ( ( nAnotherTagPos > 0 && ( nCloseTagPos > 0 && nCloseTagPos < nAnotherTagPos ) ) || ( -1 == nAnotherTagPos && nCloseTagPos > 0 ) )
		{
			nEndPos = nCloseTagPos;
			nEndLen = 11;
		}
		else if ( nCloseSlashPos > 0 )
		{
			nEndPos = nCloseSlashPos;
			nEndLen = 1;
		}
		else
		{
			break;
		}

		sBuffer.Delete( nStartPos, nEndPos - nStartPos + nEndLen );
		sResult.Delete( nStartPos, nEndPos - nStartPos + nEndLen );

		dwSize -= nEndPos - nStartPos + nEndLen;
	}

	nStartPos = -1, nEndPos = -1, nEndLen = 0;
	while ( -1 != ( nStartPos = sBuffer.Find( "<xmp>" ) ) )
	{
		// Есть ли еще такой тэг
		int nAnotherTagPos = sBuffer.Find( "<xmp", nStartPos + 1 );
		int nCloseSlashPos = sBuffer.Find( ">", nStartPos );
		int nCloseTagPos   = sBuffer.Find( "</xmp>", nStartPos );

		if ( ( nAnotherTagPos > 0 && ( nCloseTagPos > 0 && nCloseTagPos < nAnotherTagPos ) ) || ( -1 == nAnotherTagPos && nCloseTagPos > 0 ) )
		{
			nEndPos = nCloseTagPos;
			nEndLen = 6;
		}
		else if ( nCloseSlashPos > 0 )
		{
			nEndPos = nCloseSlashPos;
			nEndLen = 1;
		}
		else
		{
			break;
		}

		CStringA sBufferTemp = sBuffer;
		CStringA sResultTemp = sResult;

		sBufferTemp.Delete( nEndPos, sBuffer.GetLength() - nEndPos );
		sBufferTemp.Delete( 0, nStartPos + 5 );

		int nOldLen = sBufferTemp.GetLength() + 5 + nEndLen;

		HtmlTagsToText( &sBufferTemp );
		sBufferTemp = "<tt>" + sBufferTemp + "</tt>";

		int nNewLen = sBufferTemp.GetLength();

		sResultTemp.Delete( nEndPos, sResult.GetLength() - nEndPos );
		sResultTemp.Delete( 0, nStartPos + 5 );

		HtmlTagsToText( &sResultTemp );
		sResultTemp = "<tt>" + sResultTemp + "</tt>";

		sBuffer.Delete( nStartPos, nEndPos - nStartPos + nEndLen );
		sBuffer.Insert( nStartPos, sBufferTemp );
		sResult.Delete( nStartPos, nEndPos - nStartPos + nEndLen );
		sResult.Insert( nStartPos, sResultTemp );

		dwSize = dwSize - nOldLen + nNewLen;
	}
	//пробуем найти кодировку в <!xml
	nStartPos = -1, nEndPos = -1, nEndLen = 0;
	if ( -1 != ( nEndPos = sBuffer.Find( ">" ) ) )
	{
		CStringA sXmlHeader = sBuffer.Left( nEndPos + 1 ) + "<root/>";
		HRESULT hr;
		MSXML2::IXMLDOMDocumentPtr xmlDomDocument( L"Msxml2.DOMDocument" );
		if( NULL != xmlDomDocument )
		{
			try{
				VARIANT_BOOL bSuccessful;
				BSTR bstrXmlHeader = sXmlHeader.AllocSysString();
				bSuccessful = xmlDomDocument->loadXML(bstrXmlHeader);
				SysFreeString( bstrXmlHeader );
				if( VARIANT_FALSE != bSuccessful )
				{
					MSXML2::IXMLDOMNode* piRootNode = NULL;
					if( SUCCEEDED( xmlDomDocument->get_firstChild( &piRootNode) ) && NULL != piRootNode )
					{
						BSTR bstrNodename = NULL;
						piRootNode->get_nodeName( &bstrNodename );
						if( CString(bstrNodename) == _T("xml") )
						{
							MSXML2::IXMLDOMNamedNodeMap* piNodeMap = NULL;
							if( SUCCEEDED( piRootNode->get_attributes( &piNodeMap ) ) && NULL != piNodeMap )
							{
								MSXML2::IXMLDOMNode* piDomNode = NULL;
								if( SUCCEEDED( piNodeMap->raw_getNamedItem( L"encoding", &piDomNode ) ) && NULL != piDomNode )
								{
									BSTR bstrEncAttr;
									piDomNode->get_text( &bstrEncAttr );
									sNewCharset = bstrEncAttr;
									SysFreeString( bstrEncAttr );
									RELEASEINTERFACE( piDomNode );
								}
								RELEASEINTERFACE( piNodeMap );
							}
						}
						SysFreeString( bstrNodename );
						RELEASEINTERFACE( piRootNode );
					}
				}
			}
			catch(...)
			{
			}
		}
	}

	int nLen = sResult.GetLength();
	if ( dwSize != sResult.GetLength() )
	{
		UnmapViewOfFile( pBaseAddress );
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return;
	}


	BYTE *pBuffer = new BYTE[dwSize];
	if ( !pBuffer )
	{
		UnmapViewOfFile( pBaseAddress );
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return;
	}
	::memcpy( (void*)pBuffer, (const BYTE*)sResult.GetBuffer(), dwSize );

	UnmapViewOfFile( pBaseAddress );
	CloseHandle( hMapFile );
	CloseHandle( hFile );

	// Кодировки могут придти в виде следующих строк:
	//	<meta charset="utf-8">
	//  <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
	// Htmlayout воспринимает только второй вариант, поэтому нам надо привести первый ко второму
	

	//if ( !m_oHtmLite.load( pBuffer, dwSize ) )
	if (!loadFile(pBuffer, dwSize))
		return;
	
    HELEMENT pElement = m_oHtmLite.getRootElement();
    htmlayout::dom::element oHtmlRoot( pElement );
	
	// Ищем заголовок
	pElement = oHtmlRoot.find_first( "head" );
	if ( pElement )
	{
        htmlayout::dom::element oHtmlHead( pElement );
        std::vector< htmlayout::dom::element > arrHtmlMeta;

        oHtmlHead.find_all( arrHtmlMeta, "meta" );
        for ( std::vector< htmlayout::dom::element >::iterator pIter = arrHtmlMeta.begin(); pIter != arrHtmlMeta.end(); ++pIter )
        {
			_bstr_t bsCharset = FindAttribute( *pIter, _bstr_t( _T("charset") ) );
			_bstr_t bsContent = FindAttribute( *pIter, _bstr_t( _T("content") ) );

			CStringA sCharset( bsCharset.GetBSTR() ), sContent( bsContent.GetBSTR() );
			sCharset.MakeLower();
			sContent.MakeLower();

			if ( sCharset != "")
			{
				// Нашли кодировку в строке <meta charset="utf-8">
				sNewCharset = sCharset;
				bFindCharset = TRUE;
				break;
			}
			else if ( sContent != "" )
			{
				if ( -1 != sContent.Find( "charset" ) && -1 != sContent.Find( "text/html" ) )
				{
					sNewCharset = _T("");
					// Нашли кодировку в строке <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
					if ( -1 != sContent.Find("utf-8") || -1 != sContent.Find("utf8") )
						bFindCharset = FALSE;
					else
						bFindCharset = TRUE;

					break;
				}
			}
        }
	}

	if ( pElement && ( sNewCharset != "" || !bFindCharset ) )
	{
		CStringA sMetaCharset = "\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=";
		if ( sNewCharset != "" )
			sMetaCharset += sNewCharset + "\" />\n";
		else
			sMetaCharset += "cp1251\" />\n";
		CString sTemp( sResult );
		sTemp.MakeLower();

		int nStartTagHead = sTemp.Find( _T("<head") );
		if ( -1 == nStartTagHead )
		{
			delete []pBuffer;
			return;
		}
		int nEndTagHead   = sTemp.Find( _T(">"), nStartTagHead ) + 1;
		if ( -1 == nEndTagHead )
		{
			delete []pBuffer;
			return;
		}

		BYTE *pTemp = new BYTE[ dwSize + sMetaCharset.GetLength() + 1 ];
		if ( !pTemp )
		{
			delete []pBuffer;
			return;
		}
		pTemp[dwSize + sMetaCharset.GetLength()] = '\0';

		::memcpy( pTemp, pBuffer, nEndTagHead );
		::memcpy( pTemp + nEndTagHead, (const BYTE *)sMetaCharset.GetBuffer(), sMetaCharset.GetLength() );
		::memcpy( pTemp + nEndTagHead + sMetaCharset.GetLength(), pBuffer + nEndTagHead, dwSize - nEndTagHead );

		delete []pBuffer;

		dwSize += sMetaCharset.GetLength();
		pBuffer = pTemp;
	}

	m_pBuffer = pBuffer;
	m_dwBufferSize = /*550*/dwSize;

	return;
}

bool HtmlToXhtml::loadFile(LPCBYTE dataptr, DWORD datasize)
{
	__try
	{
		__try
		{
			return m_oHtmLite.load( dataptr, datasize );
		}
		__finally
		{
			return true;
		}
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
		return false;
	}
}

void HtmlToXhtml::RemoveInvalidChars(BSTR *pbsString)
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
void HtmlToXhtml::HtmlTagsToText(CStringA *psString )
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

void HtmlToXhtml::DecodeUrl(CString *psString)
{
	// В некоторых версиях Htmlayout строка Test%20test может прийти как Test%2520test
	psString->Replace( _T("%25"), _T("%") );

	CStringA sTemp;
	CString sResult;
	int nLen = psString->GetLength();

	bool bPreviousPerc = false;
	for ( int nPos = 0; nPos < nLen; nPos++ )
	{
		if ( '%' != psString->GetAt(nPos) )
		{
			if ( bPreviousPerc )
			{
				sResult += CString( Encoding::utf82unicode( sTemp.GetBuffer() ).c_str() );
				sTemp.Empty();
			}

			sResult += psString->GetAt(nPos);
			bPreviousPerc = false;
		}
		else
		{
			bPreviousPerc = true;
			if ( nPos + 2 < nLen )
			{
				unsigned char unChar1 = HexChar2Int( psString->GetAt(nPos + 1) );
				unsigned char unChar2 = HexChar2Int( psString->GetAt(nPos + 2) );

				unsigned char unChar = (unChar1 << 4) + unChar2;
				sTemp += (unsigned char)unChar;
			}
			else
				break;

			nPos += 2;
		}
	}

	if ( bPreviousPerc && !sTemp.IsEmpty() )
		sResult += CString( Encoding::utf82unicode( sTemp.GetBuffer() ).c_str() );

	*psString = sResult;
}

int HtmlToXhtml::filter ( unsigned int code, struct _EXCEPTION_POINTERS *ep )
{
   if ( code == EXCEPTION_ACCESS_VIOLATION )
      return EXCEPTION_EXECUTE_HANDLER;
   else
      return EXCEPTION_CONTINUE_SEARCH; 
}
