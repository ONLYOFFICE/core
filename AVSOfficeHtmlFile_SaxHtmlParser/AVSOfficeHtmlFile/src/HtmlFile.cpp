// HtmlFile.cpp : Implementation of CHtmlFile

#include "stdafx.h"
#include "HtmlFile.h"
#include "strings.h"
#include "uniteoox.h"
#include "transxml.h"
#include "../prj/HtmlToXhtmlSax.h"
#include "Html2Mht.h"
#include <windows.h>
#include <algorithm>
#include <string>
#include "path.h"
#include "utils.h"
#include "gdiplus.h"
//#include "Encoding.h"
//#include "TxtFile.h"
#include "MhtFile.h"
#include "XmlUtils.h"
#include "./../../../Common/OfficeFileFormats.h"
#include "DateTime.h"


const wchar_t CHtmlFile::m_sLibPath[] = L"AVSOfficeHtmlFile.dll";

HRESULT CHtmlFile::FinalConstruct()
{	
	m_oInit.Init();
	return S_OK;
}


void CHtmlFile::FinalRelease()
{
	if ( m_bDownloadImages )
	{
		for ( int nIndex = 0; nIndex < m_arrImageFilePath.GetSize(); ++nIndex )
		{
			DeleteFileW( m_arrImageFilePath.GetData()[nIndex].GetBuffer() );
		}
		m_arrImageFilePath.RemoveAll();
	}
}


bool CHtmlFile::HtmlToXHtml_htmlayout( const wchar_t *wsHtmlPath, const wchar_t *wsXHtmlPath, const MhtFile* pMhtFile, BOOL bDownloadImages, int nDefaultCodePage)
{	
	CHtmlToXhtmlSax oHtmlToXhtml ( wsHtmlPath, wsXHtmlPath, pMhtFile);

	oHtmlToXhtml.SetDownloadImages (bDownloadImages);
	oHtmlToXhtml.SetDefaultCodePage (nDefaultCodePage);
	
	bool bResult = oHtmlToXhtml.Convert ();

	bResult = (TRUE == oHtmlToXhtml.SaveXML ());
	m_arrImageFilePath = oHtmlToXhtml.GetTempImages();

	return bResult;
}
bool CHtmlFile::HtmlToXHtml_mshtml( const wchar_t *wsHtmlPath, const wchar_t *wsXHtmlPath, const MhtFile* pMhtFile, BOOL bDownloadImages )
{
	/*
	HtmlToXhtml oHtmlToXhtml( wsHtmlPath, wsXHtmlPath, pMhtFile );

	oHtmlToXhtml.SetDownloadImages( bDownloadImages );
	bool bResult = oHtmlToXhtml.Convert_mshtml();
	bResult = oHtmlToXhtml.WriteXHTML();
	m_arrImageFilePath = oHtmlToXhtml.GetTempImages();
	

	return bResult;
	*/
	return true;
}


void CHtmlFile::OoxPostProcessorCopyImages( BSTR sFileName, BSTR sSrcFileName, BSTR sDstPath )
{
    MSXML2::IXMLDOMDocumentPtr xml_doc( L"Msxml2.DOMDocument" );
    if ( xml_doc->load( sFileName ) == VARIANT_TRUE )
    {
        MSXML2::IXMLDOMNodeListPtr ins_elems  = xml_doc->selectNodes( L"Relationships/Relationship" );
        MSXML2::IXMLDOMElementPtr  rels_node  = xml_doc->createElement( L"Relationships" );

        rels_node->setAttribute( L"xmlns", L"http://schemas.openxmlformats.org/package/2006/relationships" );
        while( MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode() )
        {
            MSXML2::IXMLDOMNamedNodeMapPtr attrs      = child->Getattributes();
            MSXML2::IXMLDOMNodePtr         src_attr   = attrs->getNamedItem( L"Source" );
            MSXML2::IXMLDOMNodePtr         trg_attr   = attrs->getNamedItem( L"Target" );
            MSXML2::IXMLDOMNodePtr         tmode_attr = attrs->getNamedItem( L"TargetMode" );
            MSXML2::IXMLDOMNodePtr         id_attr    = attrs->getNamedItem( L"Id" );
            MSXML2::IXMLDOMNodePtr         type_attr  = attrs->getNamedItem( L"Type" );
            bool                           skip       = !( trg_attr && id_attr && type_attr );

            if ( src_attr && !skip )
            {
                std::wstring source = src_attr->GetnodeValue().bstrVal;
                std::wstring target = sDstPath;

                if ( ( target[ target.length() - 1 ] != L'\\' ) && ( target[ target.length() - 1 ] != L'/' ) )
                {
                    target += L'\\';
                }
                target += L"word\\";
                target += trg_attr->GetnodeValue().bstrVal;

                if ( target.find( L":" ) == std::wstring::npos )
                {
                    wchar_t buff [ 1024 ] = { 0 };

                    GetFullPathNameW( target.c_str(), 1024, buff, NULL );
                    target = buff;
                }

				CString sPath( sSrcFileName );
				if ( !CHtmlToXhtmlSax::FileExist( &sPath ) )
					source = full_from_relative( sSrcFileName, source );

                skip = ( source == L"" );
                if ( !skip )
                {
                    std::replace( target.begin(), target.end(), L'/', L'\\' );
                    skip = !CopyFileW( source.c_str(), target.c_str(), FALSE );
                }
            }
            if ( !skip )
            {
                MSXML2::IXMLDOMElementPtr rel_node = xml_doc->createElement( L"Relationship" );
                rel_node->setAttribute( L"Id", id_attr->GetnodeValue().bstrVal );
                rel_node->setAttribute( L"Type", type_attr->GetnodeValue().bstrVal );
                rel_node->setAttribute( L"Target", trg_attr->GetnodeValue().bstrVal );
                if ( tmode_attr )
                {
                    rel_node->setAttribute( L"TargetMode", tmode_attr->GetnodeValue().bstrVal );
                }
                rels_node->appendChild( rel_node );
            }
        }
        xml_doc->removeChild( xml_doc->selectSingleNode( L"Relationships" ) );
        xml_doc->appendChild( rels_node );
        xml_doc->save( sFileName );
    }
}



void CHtmlFile::OoxPostProcessorCopyImagesFromMht( BSTR sFileName, BSTR sSrcFileName, BSTR sDstPath, const MhtFile& mhtFile )
{
    MSXML2::IXMLDOMDocumentPtr xml_doc( L"Msxml2.DOMDocument" );
    if ( xml_doc->load( sFileName ) == VARIANT_TRUE )
    {
        MSXML2::IXMLDOMNodeListPtr ins_elems  = xml_doc->selectNodes( L"Relationships/Relationship" );
        MSXML2::IXMLDOMElementPtr  rels_node  = xml_doc->createElement( L"Relationships" );

        rels_node->setAttribute( L"xmlns", L"http://schemas.openxmlformats.org/package/2006/relationships" );
        while( MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode() )
        {
            MSXML2::IXMLDOMNamedNodeMapPtr attrs      = child->Getattributes();
            MSXML2::IXMLDOMNodePtr         src_attr   = attrs->getNamedItem( L"Source" );
            MSXML2::IXMLDOMNodePtr         trg_attr   = attrs->getNamedItem( L"Target" );
            MSXML2::IXMLDOMNodePtr         tmode_attr = attrs->getNamedItem( L"TargetMode" );
            MSXML2::IXMLDOMNodePtr         id_attr    = attrs->getNamedItem( L"Id" );
            MSXML2::IXMLDOMNodePtr         type_attr  = attrs->getNamedItem( L"Type" );
            bool                           skip       = !( trg_attr && id_attr && type_attr );

            if ( src_attr && !skip )
            {
                std::wstring source = src_attr->GetnodeValue().bstrVal;
                std::wstring target = sDstPath;

                if ( ( target[ target.length() - 1 ] != L'\\' ) && ( target[ target.length() - 1 ] != L'/' ) )
                {
                    target += L'\\';
                }
                target += L"word\\";
                target += trg_attr->GetnodeValue().bstrVal;

                if ( target.find( L":" ) == std::wstring::npos )
                {
                    wchar_t buff [ 1024 ] = { 0 };

                    GetFullPathNameW( target.c_str(), 1024, buff, NULL );
                    target = buff;
                }

                skip = ( source == L"" );
                if ( !skip )
                {
                    std::replace( target.begin(), target.end(), L'/', L'\\' );
					skip = !mhtFile.savePicture(source, target);
                }
            }
            if ( !skip )
            {
                MSXML2::IXMLDOMElementPtr rel_node = xml_doc->createElement( L"Relationship" );
                rel_node->setAttribute( L"Id", id_attr->GetnodeValue().bstrVal );
                rel_node->setAttribute( L"Type", type_attr->GetnodeValue().bstrVal );
                rel_node->setAttribute( L"Target", trg_attr->GetnodeValue().bstrVal );
                if ( tmode_attr )
                {
                    rel_node->setAttribute( L"TargetMode", tmode_attr->GetnodeValue().bstrVal );
                }
                rels_node->appendChild( rel_node );
            }
        }
        xml_doc->removeChild( xml_doc->selectSingleNode( L"Relationships" ) );
        xml_doc->appendChild( rels_node );
        xml_doc->save( sFileName );
    }
}


void CHtmlFile::HtmPostProcessorRemoveXmlns( BSTR sFileName )
{
    MSXML2::IXMLDOMDocumentPtr xml_doc( L"Msxml2.DOMDocument" );
    if ( xml_doc->load( sFileName ) == VARIANT_TRUE )
    {
        MSXML2::IXMLDOMElementPtr html_node = xml_doc->createElement( L"html" );
		html_node->setAttribute(L"xmlns", L"http://www.w3.org/1999/xhtml");
        MSXML2::IXMLDOMNodeListPtr ins_elems = xml_doc->selectNodes( L"html/*" );
        while( MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode() )
        {
            html_node->appendChild( child );
        }
        xml_doc->removeChild( xml_doc->selectSingleNode( L"html" ) );
        xml_doc->appendChild( html_node );

		
        xml_doc->save( sFileName );
    }
}


void CHtmlFile::OoxPostProcessorCutStubChildren( MSXML2::IXMLDOMDocumentPtr xml_doc, MSXML2::IXMLDOMNodePtr pNode )
{
    MSXML2::IXMLDOMNodeListPtr del_elems  = pNode->selectNodes( L"w:p[ @pcut:cut ]" );
	BSTR predNode (NULL);
	while( MSXML2::IXMLDOMNodePtr child = del_elems->nextNode() )
	{
		MSXML2::IXMLDOMNodeListPtr sub_children = child->selectNodes( L"*" );
		BSTR bstrText (NULL);
		child->firstChild->firstChild->get_nodeName (&bstrText);
		BSTR nextNode (NULL);
		if(child->nextSibling)
		{
			if(child->nextSibling->firstChild)
			{
				if(child->nextSibling->firstChild->firstChild)
				{
					child->nextSibling->firstChild->firstChild->get_nodeName (&nextNode);
				}		
			}
		}
		bool check;
		if (_bstr_t (L"w:pageBreakBefore") == _bstr_t (bstrText) 
			&& _bstr_t (nextNode) != _bstr_t (bstrText)
			&& _bstr_t (predNode) != _bstr_t (bstrText))
			check = true;
		else
			check = false;

		if (NULL != bstrText)
		{
			SysFreeString(bstrText);
			bstrText = NULL;
		}
		if (NULL != predNode)
		{
			SysFreeString(predNode);
			predNode = NULL;
		}
		if (NULL != nextNode)
		{
			SysFreeString(nextNode);
			nextNode = NULL;
		}

		child->firstChild->firstChild->get_nodeName (&predNode);
		if ( sub_children->length <= 1 && false == check)
		{
			pNode->removeChild( child );
		}
	}
	MSXML2::IXMLDOMNodeListPtr mod_elems  = pNode->selectNodes( L"*" );
    while( MSXML2::IXMLDOMNodePtr child = mod_elems->nextNode() )
    {
        OoxPostProcessorCutStubChildren( xml_doc, child );
    }
    if ( pNode->nodeName == _bstr_t( L"w:tc" ) )
    {
        MSXML2::IXMLDOMNodePtr last_child = pNode->lastChild;
        if ( ! last_child || ( last_child->nodeName != _bstr_t( L"w:p" ) ) )
        {
            pNode->appendChild( xml_doc->createElement( L"w:p" ) );
        }
    }
}


void CHtmlFile::OoxPostProcessorCutStubs( BSTR sFileName )
{
    MSXML2::IXMLDOMDocumentPtr xml_doc( L"Msxml2.DOMDocument" );
    if ( xml_doc->load( sFileName ) == VARIANT_TRUE )
    {
        MSXML2::IXMLDOMElementPtr doc_node = xml_doc->selectSingleNode( L"w:document" );
        OoxPostProcessorCutStubChildren( xml_doc, doc_node );
        xml_doc->save( sFileName );
    }
}


void CHtmlFile::HtmPostProcessorReplaceAttrValue( BSTR sFileName, BSTR sFind, BSTR sReplace )
{
    MSXML2::IXMLDOMDocumentPtr xml_doc( L"Msxml2.DOMDocument" );

    if ( xml_doc->load( sFileName ) == VARIANT_TRUE )
    {
        MSXML2::IXMLDOMNodeListPtr find_elems  = xml_doc->selectNodes( L"//@*" );
        while( MSXML2::IXMLDOMNodePtr child = find_elems->nextNode() )
        {			
            std::wstring value = child->nodeValue.bstrVal;
            std::wstring::size_type pos = value.find( sFind );
            if ( pos != std::wstring::npos )
            {
                value = value.replace( pos, wcslen( sFind ), sReplace );
                child->nodeValue = value.c_str();
            }			
        }
		xml_doc->save( sFileName );
    }
}


void CHtmlFile::ImagePath(const std::wstring& mediaPath, const BSTR& sDstFileName)
{	
	wchar_t  files_dir [ 1024 ] = { 0 };
	int		 count = 0;	

	std::wstring wstrFileName = sDstFileName;
	std::wstring file_ext = strings::substring_after(wstrFileName, wstrFileName.find_last_of(L"."));
	if (file_ext == L"htm" || file_ext == L"html")
		swprintf( files_dir, L"%s_files", strings::substring_before(wstrFileName, wstrFileName.find_last_of(L".")).c_str());
	else
		swprintf( files_dir, L"%s_files", wstrFileName.c_str());
	bool bDirCreated = false;	

	wchar_t  mediaFolder[ 1024 ] = { 0 };
	swprintf( mediaFolder, L"%s/", files_dir);
	
	MSXML2::IXMLDOMDocumentPtr xml_doc( L"Msxml2.DOMDocument" );
    if ( xml_doc->load( sDstFileName ) == VARIANT_TRUE )
    { 
		MSXML2::IXMLDOMNodeListPtr image_elems_list;
		xml_doc->raw_getElementsByTagName(L"img", &image_elems_list);
		while( MSXML2::IXMLDOMNodePtr image_elem = image_elems_list->nextNode())
        {			
			MSXML2::IXMLDOMNamedNodeMapPtr attrs;
			image_elem->get_attributes(&attrs);
			if (MSXML2::IXMLDOMNodePtr attr = attrs->getNamedItem(L"src"))
			{
				std::wstring value = attr->nodeValue.bstrVal;
				std::wstring newValue;

				if (value.find(L"EXTERNAL_") != std::wstring::npos)
				{
					std::wstring::size_type p = value.find(L"EXTERNAL_");
					newValue = value.substr(p + 9, value.length() - p - 9);
				}
				else
				{
					std::wstring file_ext = strings::substring_after(value, value.find_last_of(L"."));
					newValue = getName(files_dir) + L"/";
					if( false == bDirCreated )
					{
						bDirCreated = true;
						CreateDirectoryW( files_dir, NULL );
					}
					if (file_ext == L"emf" || file_ext == L"wmf")
					{
						int height = getSize(attrs, L"height");
						int width  = getSize(attrs, L"width");
						wchar_t file_to [1024] = {0};
						std::wstring newFileName = strings::substring_before(value, value.find_last_of(L"."));
						swprintf( file_to, L"%s/%s_%d%s", files_dir, newFileName.c_str(), count++, L".png");
						convertFile(mediaPath + value, file_to, height, width);
						newValue += getName(file_to);
					}
					else
					{
						copyFile(mediaPath + value, mediaFolder + value);
						newValue += value;
					}
				}
				attr->nodeValue = newValue.c_str();
			}
        }        

		// write xml with BOM
		BSTR bstrXml = NULL;
		xml_doc->get_xml(&bstrXml);

		//Convert Unicode to Utf-8
		const int nUft8Length = WideCharToMultiByte (CP_UTF8, 0, bstrXml, -1, NULL, 0, NULL, NULL);
		if (0 == nUft8Length)
		{
			ATLTRACE2 ("WideCharToMultiByte failed! (error 0x%x)\n", GetLastError ());
			SysFreeString (bstrXml);
			return;
		}
		CStringA sUtf8Xml;
		LPSTR pBuffer = sUtf8Xml.GetBufferSetLength (nUft8Length);
		if (NULL == pBuffer)
		{
			ATLTRACE2 ("String allocation failed! (error 0x%x)\n", GetLastError ());
			SysFreeString (bstrXml);
			return;
		}
		const int nConvResult = WideCharToMultiByte (CP_UTF8, 0, bstrXml, -1, pBuffer, nUft8Length, NULL, NULL);
		SysFreeString (bstrXml);

		if (NULL == nConvResult)
		{
			ATLTRACE2 ("WideCharToMultiByte failed! (error 0x%x)\n", GetLastError ());
			sUtf8Xml.ReleaseBuffer ();
			return;
		}

		HANDLE hXmlFile = ::CreateFileW (wstrFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hXmlFile)
		{
			// write UFT-8 BOM
			BYTE aBOM[3] = {0xEF, 0xBB, 0xBF};
			DWORD dwWritten (0);
			WriteFile (hXmlFile, aBOM, sizeof (aBOM), &dwWritten, NULL);
			ATLASSERT (sizeof (aBOM) == dwWritten);

			WriteFile (hXmlFile, pBuffer, nUft8Length, &dwWritten, NULL);
			ATLASSERT (nUft8Length == dwWritten);

			CloseHandle (hXmlFile);
		}
		sUtf8Xml.ReleaseBuffer ();
		
    }
	//RemoveDirectoryW( files_dir );
}


int CHtmlFile::getSize(MSXML2::IXMLDOMNamedNodeMapPtr attributs, LPCWSTR nameSize)
{
	if (attributs->getNamedItem(nameSize) != 0)
	{
		return _wtoi (attributs->getNamedItem (nameSize)->nodeValue.bstrVal);
	}
	else
	{
		std::wstring size = attributs->getNamedItem(L"style")->nodeValue.bstrVal;
		wchar_t  name [ 1024 ] = { 0 };
		swprintf( name, L"%s:", nameSize);
		return _wtoi (strings::substring_between(size, name, L"px;").c_str());
	}
}


void CHtmlFile::copyFile(const std::wstring& outFilePath, const std::wstring& inFilePath)
{	
	CopyFileW( outFilePath.c_str(), inFilePath.c_str(), FALSE );
}


void CHtmlFile::convertFile(const std::wstring& outFilePath, const std::wstring& inFilePath, const int height, const int width)
{	
	Gdiplus::Image* oMeta = Gdiplus::Metafile::FromFile(outFilePath.c_str());

	if (oMeta != 0)
	{

		Gdiplus::Bitmap* oPNG = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);

		Gdiplus::Graphics* oGr = Gdiplus::Graphics::FromImage((Gdiplus::Image*)oPNG);
		oGr->Clear(Gdiplus::Color(0, 255, 255, 255));
		oGr->SetSmoothingMode(SmoothingModeAntiAlias);
		oGr->SetTextRenderingHint(TextRenderingHintAntiAlias);		
		oGr->DrawImage((Gdiplus::Image*)oMeta, Gdiplus::Rect(0, 0, width, height));		

		CLSID oEncoder = getSaveFormat();
		Gdiplus::Status nStatus = oPNG->Save(inFilePath.c_str(), &oEncoder);

		delete oPNG;
	}
}


const CLSID CHtmlFile::getSaveFormat()
{
	CLSID oEncoder;
	std::wstring strFormat = L"image/png";

	UINT nEncoders = 0;
	UINT nSize = 0;
	ImageCodecInfo* pImageCodecInfo = 0;
	GetImageEncodersSize(&nEncoders, &nSize);
	if (!nSize)
		throw 0;
	pImageCodecInfo = (ImageCodecInfo*)(malloc(nSize));
	if (!pImageCodecInfo)
		throw 0;
	GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

	for (UINT nEncoder = 0; nEncoder < nEncoders; ++nEncoder)
	{
		std::wstring strNew = pImageCodecInfo[nEncoder].MimeType;			
		if (strNew == strFormat)
		{				
			oEncoder = pImageCodecInfo[nEncoder].Clsid;				
		}    
	}
	free(pImageCodecInfo);

	return oEncoder;
}


const std::wstring CHtmlFile::getPath(const std::wstring& fileName)
{
	std::wstring path(fileName);
	size_t right_slash = path.find_last_of(L"/");
	if (std::wstring::npos == right_slash )
		right_slash = 0;
	size_t left_slash = path.find_last_of(L"\\");
	if (std::wstring::npos == left_slash )
		left_slash = 0;
	
	path = strings::substring_before(path, max(right_slash, left_slash)) + L"/";

	return path;
}


const std::wstring CHtmlFile::getName(const std::wstring& fileName)
{
	std::wstring path(fileName);
	size_t right_slash = path.find_last_of(L"/");
	if (std::wstring::npos == right_slash )
		right_slash = 0;
	size_t left_slash = path.find_last_of(L"\\");
	if (std::wstring::npos == left_slash )
		left_slash = 0;
	
	path = strings::substring_after(path, max(right_slash, left_slash));

	return path;
}


const int CHtmlFile::getHtmFormatFromOptions(BSTR sXMLOptions)
{
	MSXML2::IXMLDOMDocumentPtr options( L"Msxml2.DOMDocument" );
	
    if ( options->loadXML( sXMLOptions ) == VARIANT_TRUE )
    {
		MSXML2::IXMLDOMNodeList *pNode = NULL;
		
		if ( SUCCEEDED( options->raw_getElementsByTagName( L"FileType", &pNode ) ) && NULL != pNode )
		{
			MSXML2::IXMLDOMNode *pDomNode = NULL;
			if ( SUCCEEDED( pNode->raw_nextNode( &pDomNode ) ) && NULL != pDomNode )
				return atoi( pDomNode->text );
		}
	}
	return 0;
}

const int CHtmlFile::getDefaultCodePageFromOptions(BSTR sXMLOptions)
{
	MSXML2::IXMLDOMDocumentPtr options( L"Msxml2.DOMDocument" );
	
    if ( options->loadXML( sXMLOptions ) == VARIANT_TRUE )
    {
		MSXML2::IXMLDOMNodeList *pNode = NULL;
		
		if ( SUCCEEDED( options->raw_getElementsByTagName( L"DefaultCodePage", &pNode ) ) && NULL != pNode )
		{
			MSXML2::IXMLDOMNode *pDomNode = NULL;
			if ( SUCCEEDED( pNode->raw_nextNode( &pDomNode ) ) && NULL != pDomNode )
				return atoi( pDomNode->text );
		}
	}
	return CP_ACP;
}

const bool CHtmlFile::getDownloadImages(BSTR sXMLOptions)
{
	MSXML2::IXMLDOMDocumentPtr options( L"Msxml2.DOMDocument" );
	
    if ( options->loadXML( sXMLOptions ) == VARIANT_TRUE )
    {
		MSXML2::IXMLDOMNodeList *pNode = NULL;
		
		if ( SUCCEEDED( options->raw_getElementsByTagName( L"DownloadImages", &pNode ) ) && NULL != pNode )
		{
			MSXML2::IXMLDOMNode *pDomNode = NULL;
			if ( SUCCEEDED( pNode->raw_nextNode( &pDomNode ) ) && NULL != pDomNode )
				return (1  == atoi( pDomNode->text ));
		}
	}
	// we should download images by default
	return true;
}


const int CHtmlFile::getHtmFormatToLoad(BSTR sSrcFileName)
{
	std::wstring wstrFileName = sSrcFileName;

	if (wstrFileName.find(L"."))
	{
		std::wstring ext = strings::substring_after(wstrFileName, wstrFileName.find_last_of(L"."));
		if ( ext == L"htm" || ext == L"html")
			return AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
		else if (ext == L"mht")
			return AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
	}

	return 0;
}

const int CHtmlFile::getDestFormatFromOptions(BSTR sXMLOptions, BSTR& destFilePathInNoStandardLoad)
{
	MSXML2::IXMLDOMDocumentPtr options( L"Msxml2.DOMDocument" );
	
    if ( options->loadXML( sXMLOptions ) == VARIANT_TRUE )
    { 		
		MSXML2::IXMLDOMNodeListPtr elems_list;
		options->raw_getElementsByTagName(L"NoStandartOpen", &elems_list);	
		if (elems_list->length != 0)
		{
			MSXML2::IXMLDOMNodePtr elem = elems_list->nextNode();
			MSXML2::IXMLDOMNamedNodeMapPtr attrs;
			elem->get_attributes(&attrs);
			destFilePathInNoStandardLoad = attrs->getNamedItem( L"destinationpath" )->text.GetBSTR();
			return atoi(attrs->getNamedItem( L"destinationtype" )->text);
		}
	}
	return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
}


const bool CHtmlFile::getListOptions(BSTR sXMLOptions)
{	
	MSXML2::IXMLDOMDocumentPtr options( L"Msxml2.DOMDocument" );
	if ( options->loadXML( sXMLOptions ) == VARIANT_TRUE )
    { 		
		MSXML2::IXMLDOMNodeListPtr elems_list;
		options->raw_getElementsByTagName(L"ConvertListOptions", &elems_list);	
		if (elems_list->length != 0)
		{
			MSXML2::IXMLDOMNodePtr elem = elems_list->nextNode();
			MSXML2::IXMLDOMNamedNodeMapPtr attrs;
			elem->get_attributes(&attrs);
			
			return (std::wstring)attrs->getNamedItem( L"list2list" )->GetnodeValue().bstrVal == L"true";
		}
	}
	return false;
}


const bool CHtmlFile::getCKEditorOption(BSTR sXMLOptions)
{	
	MSXML2::IXMLDOMDocumentPtr options( L"Msxml2.DOMDocument" );
	if ( options->loadXML( sXMLOptions ) == VARIANT_TRUE )
    { 		
		MSXML2::IXMLDOMNodeListPtr elems_list;
		options->raw_getElementsByTagName(L"CKEditor", &elems_list);	
		if (elems_list->length != 0)
			return true;
	}
	return false;
}


HRESULT CHtmlFile::LoadHtmlFile( BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions )
{
	//timer
#ifdef _DEBUG
	CTimeMeasurer timer;
	timer.Reset();
	float time = 0;
#endif
	//timer
	wchar_t            tmp_path					[ 1024 ] = { 0 };
	wchar_t            xhtml_file				[ 1024 ] = { 0 };
	bool result = false;
	SHORT shCancel = 0;

	CreateDirectoryW( sDstPath, NULL );

	HtmlProgress( 0, 50000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	swprintf( xhtml_file, L"%s\\XHTML.xml", sDstPath );

	m_bDownloadImages = getDownloadImages (sXMLOptions);
	int nDefaultCodePage = getDefaultCodePageFromOptions (sXMLOptions);

	result = HtmlToXHtml_htmlayout( sSrcFileName, xhtml_file, NULL, m_bDownloadImages, nDefaultCodePage);
	//result = HtmlToXHtml_mshtml( sSrcFileName, xhtml_file, NULL, m_bDownloadImages );

	if (result == false)
		return S_FALSE;

	HtmlProgress( 0, 250000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;
	
    swprintf( tmp_path, L"%s\\docProps", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\_rels", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word\\_rels", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word\\theme", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word\\media", sDstPath );
    CreateDirectoryW( tmp_path, NULL );

	HtmlProgress( 0, 340000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;
	
	swprintf( tmp_path, L"%s\\[Content_Types].xml", sDstPath );	   		
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-content_types.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 380000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\_rels\\.rels", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-rels.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 420000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\docProps\\app.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-app.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 460000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\docProps\\core.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-core.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 500000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\document.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-document.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();


	HtmlProgress( 0, 540000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    OoxPostProcessorCutStubs( tmp_path );

	HtmlProgress( 0, 600000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\fontTable.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-fonttable.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 640000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\settings.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-settings.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 680000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\styles.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-styles.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 720000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\webSettings.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-websettings.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 760000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\theme\\theme1.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-theme.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 800000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	swprintf( tmp_path, L"%s\\word\\numbering.xml", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-numbering.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 850000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\_rels\\document.xml.rels", sDstPath );
	//timer.Reset();
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-relationships.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );
	//time += timer.GetTimeInterval();

	HtmlProgress( 0, 900000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;
   
    OoxPostProcessorCopyImages( tmp_path, sSrcFileName, sDstPath );

	HtmlProgress( 0, 950000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	DeleteFileW( xhtml_file );
	//timer
#ifdef _DEBUG
	FILE* file = fopen("C:\\timer.txt", "a");
	if (NULL != file)
	{
		fprintf(file, "--------\n");
		fprintf(file, "%f\n", time);
		fprintf(file, "--------\n");
		fclose(file);
	}
#endif
	//timer

	HtmlProgress( 0, 1000000, &shCancel );

	return S_OK;
}


HRESULT CHtmlFile::LoadMhtFile( BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions )
{	
	wchar_t            tmp_path       [ 1024 ] = { 0 };
	wchar_t            mht_file       [ 1024 ] = { 0 };
   	wchar_t            xhtml_file     [ 1024 ] = { 0 };
	SHORT shCancel = 0;

	HtmlProgress( 0, 50000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	swprintf( mht_file, L"%s", sSrcFileName );
    swprintf( xhtml_file, L"%s\\XHTML.xhtm", sDstPath );

	MhtFile mhtFile;
	mhtFile.ReadFile(mht_file);

	HtmlProgress( 0, 150000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;
	
	CreateDirectoryW( sDstPath, NULL );
	HtmlToXHtml_htmlayout( L"", xhtml_file, &mhtFile, m_bDownloadImages );

	HtmlProgress( 0, 250000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;
	
    swprintf( tmp_path, L"%s\\docProps", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\_rels", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word\\_rels", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word\\theme", sDstPath );
    CreateDirectoryW( tmp_path, NULL );
    swprintf( tmp_path, L"%s\\word\\media", sDstPath );
    CreateDirectoryW( tmp_path, NULL );

	HtmlProgress( 0, 340000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;
	
	swprintf( tmp_path, L"%s\\[Content_Types].xml", sDstPath );	   
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-content_types.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 380000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\_rels\\.rels", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-rels.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 420000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\docProps\\app.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-app.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 460000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\docProps\\core.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-core.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 500000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\document.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-document.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 540000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    OoxPostProcessorCutStubs( tmp_path );

	HtmlProgress( 0, 600000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\fontTable.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-fonttable.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 640000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\settings.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-settings.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 680000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\styles.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-styles.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 720000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\webSettings.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-websettings.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 760000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\theme\\theme1.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-theme.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 800000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	swprintf( tmp_path, L"%s\\word\\numbering.xml", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-numbering.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 850000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    swprintf( tmp_path, L"%s\\word\\_rels\\document.xml.rels", sDstPath );
	trans_xml( xhtml_file, tmp_path, m_sLibPath, L"2oox-relationships.crpt", IDR_CRPT1, IDS_CONV_CRPT_01 );

	HtmlProgress( 0, 900000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

    OoxPostProcessorCopyImagesFromMht( tmp_path, sSrcFileName, sDstPath, mhtFile );

	HtmlProgress( 0, 950000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	DeleteFileW(xhtml_file);


	HtmlProgress( 0, 1000000, &shCancel );

	return S_OK;
}


// CHtmlFile
STDMETHODIMP CHtmlFile::LoadFromFile( BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions )
{
	int srcFormat = getHtmFormatFromOptions(sXMLOptions);
	if (srcFormat == 0)
		srcFormat = getHtmFormatToLoad(sSrcFileName);

	BSTR destFilePathInNoStandardLoad;
	int dstFormat = getDestFormatFromOptions(sXMLOptions, destFilePathInNoStandardLoad);

	if ( srcFormat == AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML )
	{
		if ( dstFormat == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX )
		{
			return LoadHtmlFile(sSrcFileName, sDstPath, sXMLOptions);
		}
		else if ( dstFormat == AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT )
		{
			if ( html_to_mht(sSrcFileName, destFilePathInNoStandardLoad) )
				return S_OK;
			else
				return S_FALSE;
		}					
		else
			return S_FALSE;
		
	}
	else if( srcFormat == AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT )
	{
		if ( dstFormat == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)
		{
			return LoadMhtFile(sSrcFileName, sDstPath, sXMLOptions);
		}		
		else if (dstFormat == AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML )
		{
			;
		}
		else
			return S_FALSE;
	}
	else
		return S_FALSE;
	
	return S_OK;
}


STDMETHODIMP CHtmlFile::SaveToFile( BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions )
{
    wchar_t            tmp_file       [ 1024 ] = { 0 };
    wchar_t            files_dir      [ 1024 ] = { 0 };
    wchar_t            files_dir_full [ 1024 ] = { 0 };
    wchar_t          * lpPart                  = NULL;	

	SHORT shCancel = c_shProgressContinue;

    swprintf( tmp_file, L"%s\\temp.xml", sSrcPath );
	bool bListInList = getListOptions(sXMLOptions);

	unite_oox( sSrcPath, tmp_file, bListInList );

	bool bCKEditor = getCKEditorOption(sXMLOptions);

	HtmlProgress( 0, 200000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	trans_xml( tmp_file, sDstFileName, m_sLibPath, L"oox2htm.crpt", IDR_CRPT1, IDS_CONV_CRPT_01, ( bCKEditor ? _T("CKEditor") : NULL ) );

	HtmlProgress( 0, 400000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	HtmPostProcessorRemoveXmlns( sDstFileName );

	HtmlProgress( 0, 600000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;
	
	std::wstring docxMedia(sSrcPath);
	docxMedia += _bstr_t("\\word\\media\\");

	ImagePath(docxMedia, sDstFileName);	
    DeleteFileW( tmp_file );

	HtmlProgress( 0, 800000, &shCancel );
	if ( c_shProgressCancel == shCancel )
		return S_OK;

	if (getHtmFormatFromOptions(sXMLOptions) == AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT)
	{
		html_to_mht(sDstFileName, sDstFileName);
		wchar_t            tmp_path       [ 1024 ] = { 0 };
		swprintf( tmp_path, L"%s_files", sDstFileName );
		CStringW sSrc;
		sSrc = tmp_path;
		CString sSource;
		sSource = sSrc;
		RemoveDir(sSource);
	}	

	HtmlProgress( 0, 1000000, &shCancel );

    return S_OK;
}


int CHtmlFile::RemoveDir(const CString sPath )
{
	DWORD dwFileAttrib = ::GetFileAttributes( sPath );
	if(  dwFileAttrib != INVALID_FILE_ATTRIBUTES )
	{
		DWORD dwResult = 0;
		if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
		{
			HANDLE Handle;
			WIN32_FIND_DATA FindData;
			DWORDLONG Result = 0;

			Handle = FindFirstFile( ( sPath + _T("\\*.*") ), &FindData );
			if ( Handle == INVALID_HANDLE_VALUE )
				return 0;
			do
			{
				BOOL bRes = TRUE;
				if( ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
					if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
						Result += RemoveDir( sPath + _T("\\") + FindData.cFileName ); //
					else
						bRes = DeleteFile( sPath + _T("\\") + FindData.cFileName );
				if( FALSE == bRes )
					dwResult += 1;
				}
				while( FindNextFile( Handle, &FindData ) != 0 );
				FindClose( Handle );
				BOOL bRes = RemoveDirectory( sPath );
				if( FALSE == bRes )
					dwResult += 1;
			}
			else
			{
				if( FALSE == DeleteFile( sPath ) )
					dwResult = 1;
			}

			return dwResult;
		}// 0 - все закончилось хорошо
		return 0;
}


void CHtmlFile::HtmlProgress(LONG nID, LONG nPercent, SHORT *pshCancel)
{
	OnProgressEx( nID, nPercent, pshCancel );
	OnProgress  ( nID, nPercent );
}