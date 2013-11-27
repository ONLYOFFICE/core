#include "stdafx.h"

#include <windows.h>

#include <string>
#include <vector>
#include <algorithm>
#include "utils.h"
#include "./../Common/RSA/XMLDecoder.h"
#include "./../Common/OfficeDrawing/file.h"
#include "..\AVSOfficeUtils\AVSOfficeUtils\AVSOfficeCriticalSection.h"
#include "utf8.h"

using namespace MSXML2;

#define CHECK_HR(HR) if FAILED(hr = (HR)) return hr;

AVSOfficeCriticalSection  criticalSection;

static int get_resource_id( _bstr_t lib_path,
                            _bstr_t xsl_filename,
                            int xsl_filelist_start,
                            int xsl_filenames_start )
{
    WCHAR lpBuff [ 128 ] = { 0 };
    int   i              = xsl_filenames_start;
    bool  found          = false;
    while ( LoadStringW( GetModuleHandleW( lib_path ), i, lpBuff, 128 ) )
    {
        if ( !wcscmp( lpBuff, xsl_filename ) )
        {
            found = true;
            break;
        }
        ++i;
    }
    return ( found ? ( i - xsl_filenames_start + xsl_filelist_start ) : -1 );
}

static void append_child( MSXML2::IXMLDOMElementPtr pElement, MSXML2::IXMLDOMNodePtr pChild )
{
	HRESULT hRes = S_OK;
	if ( MSXML2::NODE_ATTRIBUTE == pChild->GetnodeType() )
    {
		_variant_t vtAttr = pElement->getAttribute( pChild->GetnodeName() );
		//добавляем только новые атрибуты
		if( VT_NULL == vtAttr.vt )
			hRes = pElement->raw_setAttribute( pChild->GetnodeName(), pChild->GetnodeValue() );
    }
    else
    {
		struct MSXML2::IXMLDOMNode *pResultChild = 0;
		hRes = pElement->raw_appendChild( pChild->cloneNode( VARIANT_TRUE ), &pResultChild );
		RELEASEINTERFACE( pResultChild );
    }
}

//---------------DECRYPT----------------------------
IUnknown* setBlowfishKey(BYTE* pEncodedKey, LONG nEncodedKeySize)
{	
	MediaCore::IAVSMediaData *pData = NULL;
	CoCreateInstance(MediaCore::CLSID_CAVSMediaData, NULL, CLSCTX_ALL, MediaCore::IID_IAVSMediaData, (void**)&pData);
	if (NULL == pData)
	{
		delete []pEncodedKey;
		return NULL;
	}

	pData->AllocateBuffer(nEncodedKeySize);
	LPBYTE pBuffer = NULL;
	pData->get_Buffer(&pBuffer);
	memcpy(pBuffer, pEncodedKey, nEncodedKeySize);
	delete []pEncodedKey;

	IUnknown* punkRes = NULL;
	pData->QueryInterface(IID_IUnknown, (void**)&punkRes);

	RELEASEINTERFACE(pData);
	return punkRes;
}


BYTE* decrypt(const CString& filename, int& sizeOfFile)
{
	BYTE* text;
	CFile file;
	if (S_OK == file.OpenFile(filename))
	{
		ULONG64 lenFile = file.GetFileSize();			
		text = new BYTE[sizeOfFile = lenFile];
		file.ReadFile(text, sizeOfFile);
		
	}
	return text;
}


BYTE* extractFile(const int id, int& size_of_file)
{
	HRSRC hRes = 0;
	HGLOBAL hData = 0;
	void* pData;	
	HMODULE hModule = _AtlBaseModule.GetModuleInstance();
		
	hRes = FindResource(hModule, MAKEINTRESOURCE(id), L"CRPT");
	
	if(hRes == NULL)
		return NULL;
	
	DWORD dwSize = SizeofResource(hModule, hRes);
	
	if(dwSize == NULL)
		return NULL;
	
	hData = LoadResource(hModule, hRes);
	
	if(hData == NULL)
		return false;
	
	pData = LockResource(hData);
	
	if(pData == NULL)
		return false;

	BYTE* file = new BYTE[size_of_file = dwSize];
	memcpy(file, (BYTE*)pData, size_of_file);


	UnlockResource( hData );
	FreeResource( hData );

	return file;
}


BSTR readFileFromRresource(const int id)
{
	int sizeOfFile;
	BSTR textFile = NULL;
	BYTE* file = extractFile(id, sizeOfFile);
	//TODO TEST_CRYPT
	//BYTE* file = decrypt(L"1.crpt", sizeOfFile);
	if (file != NULL)
	{	
		LONG l = 0;
		for(int i = 0; i < 4; i++)
		{		
			LONG li = file[3 - i];
			l = l * 16 + li;
		}
		LONG l1 = l/2;
		LONG l2 = l - l1;
		LONG lenText = sizeOfFile - 4 - l;

		BYTE* keyCrypt = new BYTE[l];
		BYTE* textCrypt = new BYTE[lenText + 1];

		memcpy(keyCrypt, file + 4, l1);
		memcpy(textCrypt, file + l1 + 4, lenText);
		memcpy(keyCrypt + l1, file + sizeOfFile - l2, l2);
		textCrypt[lenText] = 0;

		IUnknown* key = setBlowfishKey(keyCrypt, l);		

		CXMLDecoder decoder;
		decoder.SetBlowfishKey(key);
		decoder.DecryptXML((BYTE**)&textCrypt, (DWORD*)&lenText);

		

		
		std::wstring text(lenText, 0);
		utf8_decode( textCrypt, textCrypt + lenText, text.begin() );
		CStringW textW;
		textW = text.c_str();
		textFile = textW.AllocSysString();
		

		key->Release();

		delete []file;
		delete []textCrypt;	
	}
	
	return textFile;
}
//---------------END_DECRYPT----------------------------

const _bstr_t changeFileName(const _bstr_t& filename)
{
	std::wstring name = filename;

	std::wstring::size_type pos = name.find_last_of(L".");
	if (pos != std::wstring::npos)	
		name = name.substr(0, pos);
	name += L".crpt";
	return name.c_str();
}


static _bstr_t XSLLoadFromRes( _bstr_t lib_path,
                               _bstr_t xsl_filename,
                               int     xsl_filelist_start,
                               int     xsl_filenames_start,
                               bool    reset_loading       = false )
{
    MSXML2::IXMLDOMDocumentPtr xsl_doc          ( L"Msxml2.DOMDocument" );
	xsl_doc->put_preserveWhiteSpace (VARIANT_TRUE);

    VARIANT_BOOL               sc               = VARIANT_FALSE;
    _bstr_t                    res              = L"";
    wchar_t                    buff     [ 128 ] = { 0 };

	_bstr_t newFileName = changeFileName(xsl_filename);	
    int                        id               = get_resource_id( lib_path,
                                                                   newFileName,
                                                                   xsl_filelist_start,
                                                                   xsl_filenames_start );
    static std::vector< int >  imported;	                          // Array of already inlined files. Declared static
                                                                    // since the function is recursive.

    if ( reset_loading )
    {
        imported.clear();
    }

    if ( std::find( imported.begin(), imported.end(), id ) == imported.end() )
    {
        imported.push_back( id );
        
		BSTR textXML = readFileFromRresource(id);		

        sc = xsl_doc->loadXML( textXML );
		//
        if ( sc == VARIANT_TRUE )
        {
            MSXML2::IXMLDOMElementPtr stylesheet_node = xsl_doc->createElement( L"xsl:stylesheet" );
            MSXML2::IXMLDOMNodeListPtr ins_elems  = xsl_doc->selectNodes( L"xsl:stylesheet/* | xsl:stylesheet/@*" );

	        while( MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode() )
	        {
                _bstr_t node_name = child->GetnodeName();
                if ( node_name == _bstr_t( L"xsl:import" ) )
                {
                    MSXML2::IXMLDOMNamedNodeMapPtr attrs = child->Getattributes();
                    _bstr_t href = attrs->getNamedItem( L"href" )->GetnodeValue().bstrVal;
                    MSXML2::IXMLDOMDocumentPtr xsl_doc_tmp( "Msxml2.DOMDocument" );
					xsl_doc_tmp->put_preserveWhiteSpace (VARIANT_TRUE);

					_bstr_t importedXml = XSLLoadFromRes( lib_path,
                                                               href,
                                                               xsl_filelist_start,
                                                               xsl_filenames_start );
					if ( importedXml.length() > 0 )
					{
						sc = xsl_doc_tmp->loadXML( importedXml );
						if ( sc == VARIANT_TRUE )
						{
							MSXML2::IXMLDOMNodeListPtr ins_elems_tmp  = xsl_doc_tmp->selectNodes( L"xsl:stylesheet/* | xsl:stylesheet/@*" );
							while ( MSXML2::IXMLDOMNodePtr child_tmp = ins_elems_tmp->nextNode() )
							{
								std::wstring child = child_tmp->baseName;
								append_child( stylesheet_node, child_tmp );
							}
						}
					}
                }
                else
                {
                    append_child( stylesheet_node, child );
                }
	        }

            xsl_doc->removeChild( xsl_doc->selectSingleNode( L"xsl:stylesheet" ) );
            xsl_doc->appendChild( stylesheet_node );
            res = xsl_doc->xml;
        }
        else
        {
            char buff[ 256 ] = { 0 };
            sprintf( buff, "Unable to open XSL resource: %S", xsl_filename.GetBSTR() );
            LOG_WARNING( buff );
        }
		SysFreeString(textXML);
		
    }
	

    return res;
}


HRESULT CreateMSXMLDocument(IXMLDOMDocument3 ** ixmlDomDocument, long version)
{
	HRESULT hr;
	CComPtr<IXMLDOMDocument3> xmlDomDocument;
	switch(version)
	{
	case 6:
		if FAILED(hr = xmlDomDocument.CoCreateInstance(__uuidof(DOMDocument60)))
		{
			// пытаемся создать MSXML более ранней версии
			return CreateMSXMLDocument(ixmlDomDocument, 4);
		}
		else
		{
			*ixmlDomDocument = xmlDomDocument.Detach();
			return hr;
		}
	case 4:
		CHECK_HR(xmlDomDocument.CoCreateInstance(__uuidof(DOMDocument40)));

		*ixmlDomDocument = xmlDomDocument.Detach();
		return hr;
	default:
		return E_INVALIDARG;
	}
}

static bool do_trans_xml( _bstr_t xml_path,
                          _bstr_t out_path,
                          _bstr_t lib_path,
                          _bstr_t xsl_filename,
                          int     xsl_filelist_start,
                          int     xsl_filenames_start )
{
    MSXML2::IXMLDOMDocumentPtr xml_doc          ( L"Msxml2.DOMDocument" );
    MSXML2::IXMLDOMDocumentPtr xsl_doc          ( L"Msxml2.DOMDocument" );
    MSXML2::IXMLDOMDocumentPtr transformed_doc  ( L"Msxml2.DOMDocument" );	
	
    _bstr_t                    xsl_text         = L"";
    _bstr_t                    transformed_text = L"";
    _variant_t                 tmp;
	
    LOG_MESSAGE( "do_trans_xml() function begin. Trying to perform XSL transformation" );
	
    if ( xml_doc->load( xml_path ) == VARIANT_FALSE )
    {
        LOG_ERROR( "Unable to load XML file" );
        return false;
    }			criticalSection.Enter();
	xsl_text = XSLLoadFromRes( lib_path,
							   xsl_filename,
							   xsl_filelist_start,
							   xsl_filenames_start,
							   true );		
	criticalSection.Leave();
    if ( !xsl_text.length() )
    {
        LOG_ERROR( "Empty XSL transformation string" );
        return false;
    }
    if ( xsl_doc->loadXML( xsl_text ) == VARIANT_FALSE )
    {
        LOG_ERROR( "Unable to load XSL transofrmation resource" );
        return false;
    }
    tmp.vt = VT_DISPATCH;
    tmp.pdispVal = transformed_doc;
    transformed_doc.Detach();

	if ( S_OK != xml_doc->transformNodeToObject( xsl_doc, tmp ) )
    {
        LOG_ERROR( "Unable to transform XML file" );
        return false;
    }
    transformed_doc.Attach( static_cast< MSXML2::IXMLDOMDocument * >( tmp.pdispVal ) );		
    tmp.vt = VT_EMPTY;
    tmp.pdispVal = NULL;
	
    if ( transformed_doc->save( out_path ) != S_OK )
    {
        LOG_ERROR( "Unable to save transformed file" );
        return false;
    }

	LOG_MESSAGE( "do_trans_xml() function exiting successfully." );

    return true;
}


void	Release(IXMLDOMDocument3Ptr &pDocXml)
{
	if ( pDocXml != NULL )
		pDocXml.Release();
}

static bool do_trans_xml2( _bstr_t xml_path,
						  _bstr_t out_path,
						  _bstr_t lib_path,
						  _bstr_t xsl_filename,
						  int     xsl_filelist_start,
						  int     xsl_filenames_start,
						  _bstr_t bsMode )
{
	IXMLDOMDocument3Ptr pDocXSLT( _T("Msxml2.FreeThreadedDOMDocument.6.0") );
	IXMLDOMDocument3Ptr pDocXml ( _T("Msxml2.FreeThreadedDOMDocument.6.0") );
	IXMLDOMDocument3Ptr pDocRes ( _T("Msxml2.FreeThreadedDOMDocument.6.0") );

	VARIANT_BOOL vbSuccess = VARIANT_TRUE;	
	_bstr_t      bsXslText = L"";

	std::wstring outFileName = std::wstring(out_path); 
	std::wstring filepath	 = std::wstring(xml_path);

	LOG_MESSAGE("do_trans_xml() function begin. Trying to perform XSL transformation");

	pDocXml->put_preserveWhiteSpace (VARIANT_TRUE);
	pDocRes->put_preserveWhiteSpace (VARIANT_TRUE);
	pDocXSLT->put_preserveWhiteSpace (VARIANT_TRUE);


	//
	pDocXml->put_async (VARIANT_FALSE);
	pDocXml->put_validateOnParse(VARIANT_TRUE);

	//
	if ( S_OK != pDocXml->raw_load( CComVariant( filepath.c_str() ), &vbSuccess ) || VARIANT_TRUE != vbSuccess )
	{
		 IErrorInfo* pErrInfo = NULL;
        ::GetErrorInfo(0, &pErrInfo);

        CComBSTR bstrDescription; 
        CComBSTR bstrSource; 
        CComBSTR bstrHelpFile;

        pErrInfo->GetDescription(&bstrDescription);
        pErrInfo->GetSource(&bstrSource);
        pErrInfo->GetHelpFile(&bstrHelpFile);

		LOG_ERROR ("Unable to load XML file");
		LOG_ERROR (std::string (CW2A(bstrDescription)));
		Release( pDocXSLT );
		Release( pDocXml  );
		Release( pDocRes  );
		return false;
	}	
	else
	{
		#ifdef _DEBUG
			// check xml passed to xslt. to view empty elements or elements with singlt space char, 
			// pls view it in the text viewer, not in built-in xml-viewer
			BSTR bstrXml;
			pDocXml->put_preserveWhiteSpace (VARIANT_TRUE);
			pDocXml->get_xml(&bstrXml);
			SysFreeString (bstrXml);
		#endif
	}

	criticalSection.Enter();

	bsXslText = XSLLoadFromRes( lib_path, xsl_filename, xsl_filelist_start, xsl_filenames_start, true );		

	criticalSection.Leave();

	if ( !bsXslText.length() )
	{
		LOG_ERROR("Empty XSL transformation string");
		Release( pDocXSLT );
		Release( pDocXml  );
		Release( pDocRes  );
		return false;
	}

	if ( S_OK != pDocXSLT->raw_loadXML( bsXslText, &vbSuccess ) || VARIANT_TRUE != vbSuccess )
	{
		LOG_ERROR("Unable to load XSL transofrmation resource");
		Release( pDocXSLT );
		Release( pDocXml  );
		Release( pDocRes  );

		return false;
	}

	MSXML2::IXSLTemplatePtr pXSLT(_T("Msxml2.XSLTemplate.6.0"));
	pXSLT->PutRefstylesheet( pDocXSLT );

	MSXML2::IXSLProcessorPtr pProcessor = pXSLT->createProcessor();
	if ( NULL == pProcessor )
	{
		LOG_ERROR("Unable to load XSL transofrmation resource");
		Release( pDocXSLT );
		Release( pDocXml  );
		Release( pDocRes  );

		return false;
	}

	pProcessor->Putinput( _variant_t( pDocXml.GetInterfacePtr() ) );
	pProcessor->addParameter( "HtmlMode", bsMode, _bstr_t(L"") );

	if ( VARIANT_TRUE != pProcessor->transform() )
	{
		LOG_ERROR( "Unable to transform XML file" );
		Release( pDocXSLT );
		Release( pDocXml  );
		Release( pDocRes  );

		return false;
	}

	_bstr_t bsResultXml = pProcessor->Getoutput();
	if ( S_OK != pDocRes->raw_loadXML( bsResultXml.GetBSTR(), &vbSuccess ) || VARIANT_TRUE != vbSuccess )
	{
		CStringW sString( bsResultXml.GetBSTR() );

		// Возможно на выходе мы получили невалидный Xml(возможно, из-за каких-либо символов)
		FILE *pFile = _wfopen( outFileName.c_str(), _T("wb+") );
		if ( pFile )
		{
			LPSTR pBuffer = NULL;
			int   nBufLen = WideCharToMultiByte( CP_UTF8, 0, sString.GetBuffer(), sString.GetLength(), pBuffer, 0, NULL, NULL );
			if ( nBufLen > 0 )
			{
				pBuffer = new CHAR[nBufLen];
				if ( pBuffer )
				{
					::memset( pBuffer, 0x00, nBufLen );

					WideCharToMultiByte( CP_UTF8, 0, sString.GetBuffer(), sString.GetLength(), pBuffer, nBufLen, NULL, NULL );
					::fwrite( pBuffer, 1, nBufLen, pFile );

					delete []pBuffer;
				}
			}

			::fclose( pFile );
		}
	}
	else
		pDocRes->save( CComVariant(outFileName.c_str()) );

	LOG_MESSAGE( "do_trans_xml() function exiting successfully." );

	Release( pDocXSLT );
	Release( pDocXml  );
	Release( pDocRes  );

	return true;
}

extern "C" bool trans_xml( const wchar_t * xml_path,
                           const wchar_t * out_path,
                           const wchar_t * lib_path,
                           const wchar_t * xsl_filename,
                           int             xsl_filelist_start,
                           int             xsl_filenames_start,
						   const wchar_t * wsMode )
{
    bool res = false;
    
    try
    {
        CoInitialize( NULL );
		res = do_trans_xml2( xml_path, out_path, lib_path, xsl_filename, xsl_filelist_start, xsl_filenames_start, wsMode );
        CoUninitialize();
    }
    catch ( const _com_error & e )
    {
		std::string error;
		str2wstr((wchar_t*)e.ErrorMessage(), error.c_str());
		LOG_ERROR( error.c_str() );
        res = false;
    }
    catch ( ... )
    {
        LOG_ERROR( "Unknown exception." );
        res = false;
    }

    return res;    
}