#pragma once
#include "stdafx.h"
#include "../Common/OfficeFileTemplate.h"

class  StorageConverters
{
//----------------------------- Public Members --------------------------------
//----------------------------- Public Methods --------------------------------

public:	BOOL GetConverter( int nFormat  , IAVSOfficeFileTemplate** ppiOutConverter)
		{
			if( AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormat )
				return GetInterface(	__uuidof(AVSOfficeDocFile::COfficeDocFile),		ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormat )
				return GetInterface(	__uuidof(AVSOfficeDocxFile::COfficeDocxFile),	ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormat )
				return GetInterface(	__uuidof(AVSOfficeHtmlFile::CHtmlFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormat )
				return GetInterface(	__uuidof(AVSOfficeOdtFile::COdtFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF == nFormat )
				return GetInterface(	__uuidof(AVSOfficeRtfFile::CRtfFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormat )
				return GetInterface(	__uuidof(AVSOfficeTxtFile::CTxtFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_IMAGE & nFormat )
				return GetInterface(	__uuidof(AVSOfficeImageFile::COfficeImageFile),	ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormat )
				return GetInterface(	__uuidof(AVSOfficePDFFile::CPDFFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS == nFormat )
				return GetInterface(	__uuidof(AVSOfficeXlsFile::CXlsFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormat )
				return GetInterface(	__uuidof(AVSOfficeDocxFile::COfficeDocxFile),	ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nFormat )
				return GetInterface(	__uuidof(AVSOfficePPTFile::CAVSOfficePPTFile),	ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormat )
				return GetInterface(	__uuidof(AVSOfficePPTXFile::CAVSOfficePPTXFile),ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP == nFormat )
				return GetInterface(	__uuidof(AVSOfficeOdpFile::CAVSOfficeODPFile ),	ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormat )
				return GetInterface(	__uuidof(OfficeEpub::COfficeEpubFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF == nFormat )
				return GetInterface(	__uuidof(FlashStudio::CSWFOfficeFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nFormat )
				return GetInterface(	__uuidof(OfficeFB2::CFB2File),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU == nFormat )
				return GetInterface(	__uuidof(OfficeDjVu::CDjVuFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == nFormat )
				return GetInterface(	__uuidof(OfficeXPS::CXPSFile),			ppiOutConverter );
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI == nFormat )
				return GetInterface(	__uuidof(EBOOKWriter::COfficeMobiFile),			ppiOutConverter );
			return FALSE;
		}
public:	BOOL GetConverter( CString sFilename  , IAVSOfficeFileTemplate** ppiOutConverter)
		{
			AVSOfficeFormatChecker avsOfficeFormatChecker;
			int nType = avsOfficeFormatChecker.GetFileFormat( sFilename );
			return GetConverter( nType, ppiOutConverter );
		}
//----------------------------- Private Methods --------------------------------
private:	BOOL GetInterface(REFCLSID rclsid,  IAVSOfficeFileTemplate** ppiOutConverter)
		{
			if( NULL == *ppiOutConverter )
			{
				bool bRews = SUCCEEDED (CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER, __uuidof(IAVSOfficeFileTemplate), (void **)(ppiOutConverter) ) );
				if( false == bRews )
					return FALSE;
			}
			return TRUE;
		}
};