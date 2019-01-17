/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "OfficeFileFormatChecker.h"

#include "../DesktopEditor/common/File.h"
#include "../OfficeUtils/src/OfficeUtils.h"

//#if defined FILE_FORMAT_CHECKER_WITH_MACRO
//	#include "../ASCOfficePPTFile/PPTFormatLib/PPTFormatLib.h"
//#endif

#include "3dParty/pole/pole.h"
#include <algorithm>

#define MIN_SIZE_BUFFER 4096
#define MAX_SIZE_BUFFER 102400

bool COfficeFileFormatChecker::isRtfFormatFile(unsigned char* pBuffer,int dwBytes)
{
	if (pBuffer == NULL) return false;

	if ( (5 <= dwBytes) && (  '{' == pBuffer[0] && '\\' == pBuffer[1] && 'r' == pBuffer[2] && 't' == pBuffer[3] && 'f' == pBuffer[4] ))
		return true;

	return false;
}
bool COfficeFileFormatChecker::isHtmlFormatFile(unsigned char* pBuffer, int dwBytes, bool testCloseTag)
{
	if (pBuffer == NULL) return false;

    bool tagOpen	= false;
	
	if (testCloseTag)
	{
		for (int i = 0; i < dwBytes - 6 ; i++)
		{
			if ((0x3C == pBuffer[i]) && (0x2F == pBuffer[i +1])	&& (0x48 == pBuffer[i + 2] || 0x68 == pBuffer[i + 2]) 
																&& (0x54 == pBuffer[i + 3] || 0x74 == pBuffer[i + 3])
																&& (0x4d == pBuffer[i + 4] || 0x6d == pBuffer[i + 4]) 
																&& (0x4c == pBuffer[i + 5] || 0x6c == pBuffer[i + 5]))
			{
				return true;
			}
		}
	}
	else
	{
		for (int i = 0; i < dwBytes - 4 && i < 100; i++)
		{
			if (0x3C == pBuffer[i])
				tagOpen = true;
			else if (0x3E == pBuffer[i])
				tagOpen = false;
			else if (tagOpen && (0x48 == pBuffer[i] || 0x68 == pBuffer[i]) && (0x54 == pBuffer[i + 1] || 0x74 == pBuffer[i + 1])
							 && (0x4d == pBuffer[i + 2] || 0x6d == pBuffer[i + 2]) && (0x4c == pBuffer[i + 3] || 0x6c == pBuffer[i + 3]))
			{
				return true;
			}
		}
	}
    return false;
}

bool COfficeFileFormatChecker::isBinaryDoctFormatFile	(unsigned char* pBuffer,int dwBytes)
{
	if (pBuffer == NULL) return false;

	if ( (4 <= dwBytes) && (  'D' == pBuffer[0] && 'O' == pBuffer[1] && 'C' == pBuffer[2] && 'Y' == pBuffer[3] ))
		return true;

	 return false;
}
bool COfficeFileFormatChecker::isBinaryXlstFormatFile	(unsigned char* pBuffer,int dwBytes)
{
	if (pBuffer == NULL) return false;

	if ( (4 <= dwBytes) && (  'X' == pBuffer[0] && 'L' == pBuffer[1] && 'S' == pBuffer[2] && 'Y' == pBuffer[3] ))
		return true;

	 return false;
}
bool COfficeFileFormatChecker::isBinaryPpttFormatFile	(unsigned char* pBuffer,int dwBytes)
{
	if (pBuffer == NULL) return false;

	if ( (4 <= dwBytes) && (  'P' == pBuffer[0] && 'P' == pBuffer[1] && 'T' == pBuffer[2] && 'Y' == pBuffer[3] ))
		return true;

	return false;
}
bool COfficeFileFormatChecker::isPdfFormatFile	(unsigned char* pBuffer,int dwBytes, std::wstring & documentID)
{
	if (pBuffer == NULL) return false;

	documentID.clear();

    if (dwBytes < 1)
        return false;

    pBuffer[dwBytes - 1] = '\0';

    char* pFirst = strstr( (char*)pBuffer, "%PDF-" );
	
    if( NULL != pFirst )
	{
		pFirst = strstr( (char*)pBuffer, "%DocumentID " );
		if( NULL != pFirst )
		{
			pFirst += 12;
			char* pLast = strstr( pFirst, " ");
			if( NULL != pLast )
			{
				std::string s(pFirst, pLast - pFirst);
				documentID = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)s.c_str(), s.length());
			}
		}
        return true;
	}

	return false;
}
bool COfficeFileFormatChecker::isDocFormatFile	(POLE::Storage * storage)
{
	if (storage == NULL) return false;

	POLE::Stream stream(storage, L"WordDocument");	
	
	unsigned char buffer[64];
	if (stream.read(buffer, 64) > 0)
	{
		//ms office 2007 encrypted contains stream WordDocument !!
		std::list<std::wstring> entries = storage->entries(L"DataSpaces");
		if (entries.size() > 0)
			return false;
		
		if ((buffer[0] == 0xEC && buffer[1] == 0xA5) ||		// word 1997-2003
			(buffer[0] == 0xDC && buffer[1] == 0xA5) ||		// word 1995
			(buffer[0] == 0xDB && buffer[1] == 0xA5))		// word 2.0
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
		}
		else if ( isHtmlFormatFile(buffer, 64, false) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER;
            return true;
        }
		if (storage->isDirectory(L"Macros"))
		{
			bMacroEnabled = true;
		}
		return true;
	}

	return false;
}
bool COfficeFileFormatChecker::isXlsFormatFile	(POLE::Storage * storage)
{
	if (storage == NULL) return false;

    unsigned char buffer[10];

    POLE::Stream stream(storage, L"Workbook");
	
	if (stream.read(buffer,10) < 1)
	{
		POLE::Stream stream2(storage, L"Book");

		if (stream2.read(buffer,10) < 1)
		{
			POLE::Stream stream3(storage, L"WORKBOOK");

			if (stream3.read(buffer,10) < 1)
			{
				POLE::Stream stream4(storage, L"BOOK");

				if (stream4.read(buffer,10) < 1)
				{
					POLE::Stream stream5(storage, L"book");

					if (stream5.read(buffer,10) < 1)
						return false;
				}
			}
		}
	}
	if (storage->isDirectory(L"_VBA_PROJECT_CUR"))
	{
		bMacroEnabled = true;
	}
	return true;
}
bool COfficeFileFormatChecker::isDocFlatFormatFile	(unsigned char* pBuffer, int dwBytes)
{
	if (pBuffer == NULL) return false;

	if ((pBuffer[0] == 0xEC && pBuffer[1] == 0xA5) ||	
		(pBuffer[0] == 0xDC && pBuffer[1] == 0xA5) || 
		(pBuffer[0] == 0xDB && pBuffer[1] == 0xA5))
		return true;

	return false;
}

bool COfficeFileFormatChecker::isPptFormatFile	(POLE::Storage * storage)
{
	if (storage == NULL) return false;

	POLE::Stream stream(storage, L"PowerPoint Document");	

	unsigned char buffer[10];
	if (stream.read(buffer,10) < 1)
		return false;

	return true;
}

std::wstring COfficeFileFormatChecker::getDocumentID (const std::wstring & _fileName )
{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
    std::wstring fileName = CorrectPathW(_fileName);
#else
    std::wstring fileName = _fileName;
#endif
	std::wstring documentID;

	POLE::Storage storage(fileName.c_str());
    if (storage.open())
    {
		if ( isMS_OFFCRYPTOFormatFile(&storage, documentID) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO;
        }
	}
	else
	{
		if ( false == isOpenOfficeFormatFile(fileName, documentID))
		{
			NSFile::CFileBinary file;
			if (!file.OpenFile(fileName))
				return documentID;
			
			unsigned char* buffer = new unsigned char[4096]; //enaf !!
			if (!buffer){file.CloseFile();return documentID;}

			DWORD dwReadBytes = 0;
			file.ReadFile(buffer, MIN_SIZE_BUFFER, dwReadBytes);
			file.CloseFile();

			if (isPdfFormatFile(buffer, (int)dwReadBytes, documentID) )
			{
				nFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
			}
		}
	}
	sDocumentID = documentID;

	return documentID;
}

bool COfficeFileFormatChecker::isMS_OFFCRYPTOFormatFile	(const std::wstring & _fileName, std::wstring & documentID)
{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
    std::wstring fileName = CorrectPathW(_fileName);
#else
    std::wstring fileName = _fileName;
#endif

	POLE::Storage storage(fileName.c_str());
    if (storage.open())
    {
		if ( isMS_OFFCRYPTOFormatFile(&storage, documentID) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO;
            return true;
        }
	}
	return false;
}

bool COfficeFileFormatChecker::isMS_OFFCRYPTOFormatFile	(POLE::Storage * storage, std::wstring & documentID)
{
    if (storage == NULL) return false;

	documentID.clear();

	bool result = false;
    std::list<std::wstring> entries = storage->entries(L"DataSpaces");
    if (entries.size() > 0)
	{
        result = true;
	}

    if ( storage->exists(L"EncryptionInfo") && 
				storage->exists(L"EncryptedPackage"))
	{
        result = true;
	}
	if (result)
	{
		POLE::Stream stream(storage, L"DocumentID");	
		
		std::string sData;
		sData.resize(stream.size());
		if (stream.read((BYTE*)sData.c_str(), stream.size()) > 0)
		{
			documentID = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sData.c_str(), sData.length());
		}

	}
	return result;
}
bool COfficeFileFormatChecker::isOfficeFile(const std::wstring & _fileName)
{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
    std::wstring fileName = CorrectPathW(_fileName);
#else
    std::wstring fileName = _fileName;
#endif

    //приоритет как оказывается важен
    //Metamorphic Manual for windows 28415.doc
	POLE::Storage storage(fileName.c_str());
    if (storage.open())
    {
        if ( isDocFormatFile(&storage) )
        {
			//nFileType внутри
			return true;
        }
        else if ( isXlsFormatFile(&storage) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
            return true;
        }
        else if ( isPptFormatFile(&storage) )
        {
//#if defined FILE_FORMAT_CHECKER_WITH_MACRO
//			COfficePPTFile pptFile;
//			
//			bMacroEnabled = true;
//			long nResult = pptFile.OpenFile(fileName, L"", bMacroEnabled);
//			if (nResult != S_OK)
//			{
//				return false;
//			}         
//			pptFile.CloseFile();
//#endif
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
            return true;
        }
        else if ( isMS_OFFCRYPTOFormatFile(&storage, sDocumentID) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO;
            return true;
        }
	}

    COfficeUtils OfficeUtils(NULL);
    if (OfficeUtils.IsArchive(fileName) == S_OK)
	{
             if ( isOOXFormatFile(fileName) )						return true;
        else if ( isOpenOfficeFormatFile(fileName, sDocumentID) )	return true;
        else if ( isOnlyOfficeFormatFile(fileName) )				return true;
        else if ( isXpsFile(fileName) )								return true;
	}
//-----------------------------------------------------------------------------------------------
    // others
    {
        NSFile::CFileBinary file;
        if (!file.OpenFile(fileName))
            return false;
		
		unsigned char* buffer = new unsigned char[4096]; //enaf !!
        if (!buffer){file.CloseFile();return false;}

        DWORD dwReadBytes = 0;
        file.ReadFile(buffer, MIN_SIZE_BUFFER, dwReadBytes);
        int sizeRead = (int)dwReadBytes;

		if ( isRtfFormatFile(buffer,sizeRead) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
		}
		else if ( isBinaryDoctFormatFile(buffer,sizeRead) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_CANVAS_WORD;
		}
		else if ( isBinaryXlstFormatFile(buffer,sizeRead) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET;
		}
		else if ( isBinaryPpttFormatFile(buffer,sizeRead) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION;
		}        
        else if (isPdfFormatFile(buffer,sizeRead, sDocumentID) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
        }
        else if (isDjvuFormatFile(buffer,sizeRead) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU;
        }
		else if (isHtmlFormatFile(buffer,sizeRead, false))
        {
			long fileSize = file.GetFileSize();
			if (fileSize > MIN_SIZE_BUFFER)		
			{
				file.SeekFile(fileSize - MIN_SIZE_BUFFER);
				file.ReadFile(buffer, MIN_SIZE_BUFFER, dwReadBytes);
				int sizeRead = (int)dwReadBytes;
			}
			if (isHtmlFormatFile(buffer,sizeRead, true))
			{
				nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
			}
        }
        else if (isFB2FormatFile(buffer,sizeRead) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2;
        }
		else if (isOpenOfficeFlatFormatFile(buffer,sizeRead) )
		{
			//nFileType
		}
		else if (isDocFlatFormatFile(buffer,sizeRead) )
		{
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC_FLAT; // without compaund container
		}

//------------------------------------------------------------------------------------------------
		file.CloseFile();

		if (buffer)delete []buffer;
		buffer = NULL;
	}
	if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN)return true;
//------------------------------------------------------------------------------------------------
//// by Extension

    std::wstring::size_type nExtPos = fileName.rfind(L'.');
	std::wstring sExt = L"unknown";
    
    if (nExtPos != std::wstring::npos)
        sExt = fileName.substr(nExtPos);

	std::transform(sExt.begin(), sExt.end(), sExt.begin(), tolower);

    if (0 == sExt.compare(L".mht"))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
    else if (0 == sExt.compare(L".csv"))
		nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
    else if (0 == sExt.compare(L".html") || 0 == sExt.compare(L".htm"))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
    else if (0 == sExt.compare(L".bin")) //base64 string
		nFileType = AVS_OFFICESTUDIO_FILE_CANVAS_PDF;
    else if (0 == sExt.compare(L".doct"))//случай архива с html viewer
		nFileType = AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY;
    else //if (0 == sExt.compare(L".txt") || 0 == sExt.compare(L".xml")) //volsciv.rtf -или любой другой
        nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;

	if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;

	return false;
}
bool COfficeFileFormatChecker::isOOXFormatFile(const std::wstring & fileName)
{
	COfficeUtils OfficeUtils(NULL);
	
	ULONG nBufferSize = 0;
	BYTE *pBuffer = NULL;

	HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"[Content_Types].xml", &pBuffer, nBufferSize);
	if (hresult == S_OK && pBuffer != NULL)
	{

		const char *docxFormatLine = "application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml";
		const char *dotxFormatLine = "application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml";
		const char *docmFormatLine = "application/vnd.ms-word.document.macroEnabled.main+xml";
		const char *dotmFormatLine = "application/vnd.ms-word.template.macroEnabledTemplate.main+xml";

		const char *xlsxFormatLine = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
		const char *xltxFormatLine = "application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml";
		const char *xlsmFormatLine = "application/vnd.ms-excel.sheet.macroEnabled.main+xml";
		const char *xltmFormatLine = "application/vnd.ms-excel.template.macroEnabled.main+xml";

		const char *pptxFormatLine = "application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml";
		const char *ppsxFormatLine = "application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml";
		const char *potxFormatLine = "application/vnd.openxmlformats-officedocument.presentationml.template.main+xml";
		const char *pptmFormatLine = "application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml";
		const char *ppsmFormatLine = "application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml";
		const char *potmFormatLine = "application/vnd.ms-powerpoint.template.macroEnabled.main+xml";

        std::string strContentTypes((char*)pBuffer, nBufferSize);

        std::string::size_type res1 = std::string::npos;
        std::string::size_type res = 0;
        
		if (std::string::npos != strContentTypes.find(docxFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		}
		else if (std::string::npos != strContentTypes.find(docmFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM;
			bMacroEnabled = true;
		}
        else if (std::string::npos != strContentTypes.find(dotxFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX;
		}
		else if (std::string::npos != strContentTypes.find(dotmFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM;
			bMacroEnabled = true;
		}
		else if (std::string::npos != strContentTypes.find(xlsxFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
		}
		else if (std::string::npos != strContentTypes.find(xlsmFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM;
			bMacroEnabled = true;
		}
        else if (std::string::npos != strContentTypes.find(xltxFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX;
		}
		else if (std::string::npos != strContentTypes.find(xltmFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM;
			bMacroEnabled = true;
		}
		else if (std::string::npos != strContentTypes.find(pptxFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		}
		else if (std::string::npos != strContentTypes.find(pptmFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM;
			bMacroEnabled = true;
		}
		else if (std::string::npos != strContentTypes.find(ppsmFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM;
			bMacroEnabled = true;
		}
		else if (std::string::npos != strContentTypes.find(ppsxFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX;
		}
        else if (std::string::npos != strContentTypes.find(potxFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX;
		}
        else if (std::string::npos != strContentTypes.find(potmFormatLine))
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM;
			bMacroEnabled = true;
		}
		delete []pBuffer;
		pBuffer = NULL;

		if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;
	}
	return false;
}
bool COfficeFileFormatChecker::isOnlyOfficeFormatFile(const std::wstring & fileName)
{
	COfficeUtils OfficeUtils(NULL);
	
	ULONG nBufferSize = 128; // ограничим считывание из бинарника
	BYTE *pBuffer = NULL;

	HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"Editor.bin", &pBuffer, nBufferSize);
	if (hresult == S_OK && pBuffer != NULL)
	{
		if (isBinaryDoctFormatFile(pBuffer, nBufferSize) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY;
		}
		else if (isBinaryXlstFormatFile(pBuffer, nBufferSize) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY;
		}
		else if (isBinaryPpttFormatFile(pBuffer, nBufferSize) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY;
		}

		delete []pBuffer;
		pBuffer = NULL;

		if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;
	}
	return false;
}
bool COfficeFileFormatChecker::isOpenOfficeFormatFile(const std::wstring & fileName, std::wstring & documentID)
{
	documentID.clear();

    const char *odtFormatLine = "application/vnd.oasis.opendocument.text";
    const char *odsFormatLine = "application/vnd.oasis.opendocument.spreadsheet";
    const char *odpFormatLine = "application/vnd.oasis.opendocument.presentation";
	const char *ottFormatLine = "application/vnd.oasis.opendocument.text-template";
	const char *otsFormatLine = "application/vnd.oasis.opendocument.spreadsheet-template";
 	const char *otpFormatLine = "application/vnd.oasis.opendocument.presentation-template";
	const char *epubFormatLine = "application/epub+zip";

    COfficeUtils OfficeUtils(NULL);
	
	ULONG nBufferSize = 0;
	BYTE *pBuffer = NULL;

	HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"documentID", &pBuffer, nBufferSize);
	if (hresult == S_OK && pBuffer != NULL)
	{
		documentID = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pBuffer, nBufferSize);
		
		delete []pBuffer;
		pBuffer = NULL;
	}

	nBufferSize = 0;	
	hresult = OfficeUtils.LoadFileFromArchive(fileName, L"mimetype", &pBuffer, nBufferSize);
	if (hresult == S_OK && pBuffer != NULL)
	{
		if ( NULL != strstr((char*)pBuffer, ottFormatLine) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT;
		}
        else if ( NULL != strstr((char*)pBuffer, otsFormatLine) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS;
		}
        else if ( NULL != strstr((char*)pBuffer, otpFormatLine) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP;
		}		
        else if ( NULL != strstr((char*)pBuffer, odtFormatLine) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
		}
        else if ( NULL != strstr((char*)pBuffer, odsFormatLine) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
		}
        else if ( NULL != strstr((char*)pBuffer, odpFormatLine) )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
		}
		else if ( NULL != strstr((char*)pBuffer, epubFormatLine) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB;
        }

		delete []pBuffer;
		pBuffer = NULL;

		if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;
    }
	else
    {
       //если не записан тип смотрим манифест
		nBufferSize = 0;
		HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"META-INF/manifest.xml", &pBuffer, nBufferSize);
        if (hresult == S_OK && pBuffer != NULL)
        {
            std::string xml_string((char*)pBuffer, nBufferSize);

            std::string::size_type nFindMem1 = xml_string.find(odtFormatLine);
            std::string::size_type nFindMem2 = xml_string.find(odsFormatLine);
            std::string::size_type nFindMem3 = xml_string.find(odpFormatLine);

            if (nFindMem1 != std::string::npos && nFindMem2 != std::string::npos)
            {
                if (nFindMem1 < nFindMem2)
                    nFindMem2 = std::string::npos;
                else
                    nFindMem1 = std::string::npos;
            }
            if (nFindMem1 != std::string::npos && nFindMem3 != std::string::npos)
            {
                if (nFindMem1 < nFindMem3)
                    nFindMem3 = std::string::npos;
                else
                    nFindMem1 = std::string::npos;
            }
            if (nFindMem2 != std::string::npos && nFindMem3 != std::string::npos)
            {
                if (nFindMem2 < nFindMem3)
                    nFindMem3 = std::string::npos;
                else
                    nFindMem2 = std::string::npos;
            }

            if (nFindMem1 != std::string::npos)
                nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
            else if (nFindMem2 != std::string::npos)
                nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
            else if (nFindMem3 != std::string::npos)
                nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;

            delete []pBuffer;
            pBuffer = NULL;

            if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;
        }

    }
	return false;
}

bool COfficeFileFormatChecker::isOpenOfficeFlatFormatFile(unsigned char* pBuffer,int dwBytes)
{
	const char *odfFormatLine = "office:document xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\"";
	
	std::string xml_string((char*)pBuffer, dwBytes);

	if (std::string::npos == xml_string.find(odfFormatLine))
	{
		return false;
	}

    const char *odtFormatLine = "application/vnd.oasis.opendocument.text";
    const char *odsFormatLine = "application/vnd.oasis.opendocument.spreadsheet";
    const char *odpFormatLine = "application/vnd.oasis.opendocument.presentation";

	if (std::string::npos != xml_string.find(odtFormatLine))
	{
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT;
	}
	else if (std::string::npos != xml_string.find(odsFormatLine))
	{
		nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT;
	}
	else if (std::string::npos != xml_string.find(odpFormatLine))
	{
		nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT;
	}

	if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;

	return false;
}

std::wstring COfficeFileFormatChecker::GetExtensionByType(int type)
{
    switch (type)
    {
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:		return L".docx";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:		return L".docm";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:		return L".dotx";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:		return L".dotm";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:		return L".doc";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:		return L".odt";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF:		return L".rtf";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT:		return L".txt";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML:		return L".html";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT:		return L".mht";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB:		return L".epub";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2:		return L".fb2";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI:		return L".mobi";
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT:	return L".fodt";
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT:		return L".ott";

    case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:	return L".pptx";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:	return L".pptm";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT:	return L".ppt";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:	return L".odp";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:	return L".ppsx";
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:	return L".ppsm";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:	return L".potx";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:	return L".potm";
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT:return L".fodp";
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP:	return L".otp";

    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:	return L".xlsx";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:	return L".xlsm";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:	return L".xltx";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:	return L".xltm";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS:		return L".xls";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:		return L".ods";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV:		return L".csv";
	case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT:return L".fods";
	case AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS:		return L".ots";

    case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:	return L".pdf";
    case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF:	return L".swf";
    case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU:	return L".djvu";
    case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:	return L".xps";
    case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG:	return L".svg";
    case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR:	return L".htmlr";

    case AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP:       return L".zip";
    case AVS_OFFICESTUDIO_FILE_OTHER_JSON:          return L".json";

    case AVS_OFFICESTUDIO_FILE_IMAGE:
    case AVS_OFFICESTUDIO_FILE_IMAGE_JPG:           return L".jpg";
    case AVS_OFFICESTUDIO_FILE_IMAGE_TIFF:          return L".tiff";
    case AVS_OFFICESTUDIO_FILE_IMAGE_TGA:           return L".tga";
    case AVS_OFFICESTUDIO_FILE_IMAGE_GIF:           return L".gif";
    case AVS_OFFICESTUDIO_FILE_IMAGE_PNG:           return L".png";
    case AVS_OFFICESTUDIO_FILE_IMAGE_EMF:           return L".emf";
    case AVS_OFFICESTUDIO_FILE_IMAGE_WMF:           return L".wmf";
    case AVS_OFFICESTUDIO_FILE_IMAGE_BMP:           return L".bmp";
    case AVS_OFFICESTUDIO_FILE_IMAGE_CR2:           return L".cr2";
    case AVS_OFFICESTUDIO_FILE_IMAGE_PCX:           return L".pcx";
    case AVS_OFFICESTUDIO_FILE_IMAGE_RAS:           return L".ras";
    case AVS_OFFICESTUDIO_FILE_IMAGE_PSD:           return L".psd";
    case AVS_OFFICESTUDIO_FILE_IMAGE_ICO:           return L".ico";

    case AVS_OFFICESTUDIO_FILE_CANVAS_WORD:
    case AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET:
    case AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION:	return L".bin";
    case AVS_OFFICESTUDIO_FILE_OTHER_OLD_DOCUMENT:
    case AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY:        return L".doct";
    case AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY:        return L".xlst";
    case AVS_OFFICESTUDIO_FILE_OTHER_OLD_PRESENTATION:
    case AVS_OFFICESTUDIO_FILE_OTHER_OLD_DRAWING:
    case AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY:        return L".pptt";
    }
    return L"";
}

int COfficeFileFormatChecker::GetFormatByExtension(const std::wstring& ext)
{
    if (L".docx" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
    if (L".docm" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM;
    if (L".dotx" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX;
    if (L".dotm" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM;
    if (L".doc" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
    if (L".odt" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
    if (L".rtf" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
    if (L".txt" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
    if (L".html" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
    if (L".mht" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
    if (L".epub" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB;
    if (L".fb2" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2;
    if (L".mobi" == ext)
        return AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI;
	if (L".docm" == ext)
		return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM;
	if (L".dotx" == ext)
		return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX;
	if (L".dotm" == ext)
		return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM;
	if (L".fodt" == ext)
		return AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT;
	if (L".ott" == ext)
		return AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT;


    if (L".pptx" == ext)
        return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
    if (L".pptm" == ext)
        return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM;
	if (L".ppsm" == ext)
		return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM;
    if (L".ppt" == ext)
        return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
    if (L".odp" == ext)
        return AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
    if (L".ppsx" == ext)
        return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX;
    if (L".potx" == ext)
        return AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX;
    if (L".potm" == ext)
        return AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM;
	if (L".fodp" == ext)
		return AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT;
	if (L".otp" == ext)
		return AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP;

    if (L".xlsx" == ext)
        return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
    if (L".xlsm" == ext)
        return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM;
    if (L".xltx" == ext)
        return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX;
    if (L".xltm" == ext)
        return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM;
    if (L".xls" == ext)
        return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
    if (L".ods" == ext)
        return AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
    if (L".csv" == ext)
        return AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
	if (L".xlsm" == ext)
		return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM;
	if (L".xltx" == ext)
		return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX;
	if (L".xltm" == ext)
		return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM;
	if (L".fods" == ext)
		return AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT;
	if (L".ots" == ext)
		return AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS;


    if (L".pdf" == ext)
        return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
    if (L".djvu" == ext)
        return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU;
    if (L".xps" == ext)
        return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS;

    return 0;
}

std::wstring COfficeFileFormatChecker::GetFormatExtension(const std::wstring & fileName)
{
	if (isOfficeFile(fileName))
		return GetExtensionByType(nFileType);

    return L"";
}

bool COfficeFileFormatChecker::isDjvuFormatFile(unsigned char* pBuffer,int dwBytes)
{
    if (pBuffer == NULL) return false;

    if ( (8 <= dwBytes) && (0x41 == pBuffer[0] && 0x54 == pBuffer[1] && 0x26 == pBuffer[2] && 0x54 == pBuffer[3] &&
                            0x46 == pBuffer[4] && 0x4f == pBuffer[5] && 0x52 == pBuffer[6] && 0x4d == pBuffer[7]))
        return true;

    return false;
}
bool COfficeFileFormatChecker::isMobiFormatFile(unsigned char* pBuffer,int dwBytes)
{
    if (pBuffer == NULL) return false;

    if ( (68 <= dwBytes) && (('B' == pBuffer[60] && 'O' == pBuffer[61] && 'O' == pBuffer[62] && 'K' == pBuffer[63] && 'M' == pBuffer[64] && 'O' == pBuffer[65] && 'B' == pBuffer[66] && 'I' == pBuffer[67]) ||
                             ('T' == pBuffer[60] && 'E' == pBuffer[61] && 'X' == pBuffer[62] && 't' == pBuffer[63] && 'R' == pBuffer[64] && 'E' == pBuffer[65] && 'A' == pBuffer[66] && 'd' == pBuffer[67])))
        return true;

    return false;
}
bool COfficeFileFormatChecker::isFB2FormatFile(unsigned char* pBuffer,int dwBytes)
{
    if (pBuffer == NULL) return false;

    bool tagOpen = false;
    //FB2 File is XML-file with rootElement - FictionBook
    for (int i = 0; i < dwBytes - 11 && i < 100; i++)
    {
        if (0x3C == pBuffer[i])
            tagOpen = true;
        else if (0x3E == pBuffer[i])
            tagOpen = false;
        else if (tagOpen && 0x46 == pBuffer[i]     && 0x69 == pBuffer[i + 1] && 0x63 == pBuffer[i + 2]
                         && 0x74 == pBuffer[i + 3] && 0x69 == pBuffer[i + 4] && 0x6F == pBuffer[i + 5]
                         && 0x6E == pBuffer[i + 6] && 0x42 == pBuffer[i + 7] && 0x6F == pBuffer[i + 8]
                         && 0x6F == pBuffer[i + 9] && 0x6B == pBuffer[i + 10])
            return true;
    }
    return false;
}
bool COfficeFileFormatChecker::isXpsFile(const std::wstring & fileName)
{
    const char *xpsFormatLine = "http://schemas.microsoft.com/xps/2005/06/fixedrepresentation";

    COfficeUtils OfficeUtils(NULL);

    ULONG nBufferSize = 0;
    BYTE *pBuffer = NULL;

    HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"_rels/.rels", &pBuffer, nBufferSize);
    if (hresult == S_OK && pBuffer != NULL)
    {
        if ( NULL != strstr((char*)pBuffer, xpsFormatLine) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS;
        }

        delete []pBuffer;
        pBuffer = NULL;

        if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;
    }else
    {
        HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"_rels/.rels/[0].piece", &pBuffer, nBufferSize);
        if (hresult == S_OK && pBuffer != NULL)
        {
            return true;
        }
    }
    return false;
}
