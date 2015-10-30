#include <algorithm>
#include "OfficeFileFormatChecker.h"
#include "../DesktopEditor/common/File.h"
#include "../OfficeUtils/src/OfficeUtils.h"
#include "3dParty/pole/pole.h"

#define MIN_SIZE_BUFFER 4096
#define MAX_SIZE_BUFFER 102400

bool COfficeFileFormatChecker::isRtfFormatFile(unsigned char* pBuffer,int dwBytes)
{
	if (pBuffer == NULL) return false;

	if ( (5 <= dwBytes) && (  '{' == pBuffer[0] && '\\' == pBuffer[1] && 'r' == pBuffer[2] && 't' == pBuffer[3] && 'f' == pBuffer[4] ))
		return true;

	return false;
}
bool COfficeFileFormatChecker::isHtmlFormatFile(unsigned char* pBuffer,int dwBytes)
{
	if (pBuffer == NULL) return false;

    bool tagOpen = false;
    //Html File is XML-file with rootElement - html
    for (int i = 0; i < dwBytes - 4 && i < 100; i++)
    {
        if (0x3C == pBuffer[i])
            tagOpen = true;
        else if (0x3E == pBuffer[i])
            tagOpen = false;
        else if (tagOpen && (0x48 == pBuffer[i] || 0x68 == pBuffer[i]) && (0x54 == pBuffer[i + 1] || 0x74 == pBuffer[i + 1])
                         && (0x4d == pBuffer[i + 2] || 0x6d == pBuffer[i + 2]) && (0x4c == pBuffer[i + 3] || 0x6c == pBuffer[i + 3]))
            return true;
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
bool COfficeFileFormatChecker::isPdfFormatFile	(unsigned char* pBuffer,int dwBytes)
{
	if (pBuffer == NULL) return false;

    int nTempBufferSize = dwBytes < 20 ? dwBytes : 20;
    char* pTempBuffer = new char[nTempBufferSize];

    memcpy ( pTempBuffer, pBuffer, nTempBufferSize );
    pTempBuffer[nTempBufferSize - 1] = '\0';

    char* pFirst = strstr( (char*)pTempBuffer, "%PDF-" );
    if( NULL != pFirst )
        return true;

	return false;
}
bool COfficeFileFormatChecker::isDocFormatFile	(POLE::Storage * storage)
{
	if (storage == false) return false;

	POLE::Stream stream(storage, "WordDocument");	
	
	unsigned char buffer[10];
	if (stream.read(buffer,10) >0)
		return true;

	return false;
}
bool COfficeFileFormatChecker::isXlsFormatFile	(POLE::Storage * storage)
{
	if (storage == false) return false;

	POLE::Stream stream(storage, "Workbook");
	
	unsigned char buffer[10];
	if (stream.read(buffer,10) >0)
		return true;

	return false;
}
bool COfficeFileFormatChecker::isPptFormatFile	(POLE::Storage * storage)
{
	if (storage == false) return false;

	POLE::Stream stream(storage, "PowerPoint Document");	

	unsigned char buffer[10];
	if (stream.read(buffer,10) >0)
		return true;

	return false;
}
bool COfficeFileFormatChecker::isMS_OFFCRYPTOFormatFile	(POLE::Storage * storage)
{
    if (storage == false) return false;

    std::list<std::string> entries = storage->entries("DataSpaces");
    if (entries.size() > 0)
        return true;

    return false;
}
bool COfficeFileFormatChecker::isOfficeFile(const std::wstring & fileName)
{
    //приоритет как оказывается важен
    //Metamorphic Manual for windows 28415.doc
	POLE::Storage storage(fileName.c_str());
    if (storage.open())
    {
        if ( isDocFormatFile(&storage) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
            return true;
        }
        else if ( isXlsFormatFile(&storage) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
            return true;
        }
        else if ( isPptFormatFile(&storage) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
            return true;
        }
        else if ( isMS_OFFCRYPTOFormatFile(&storage) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO;
            return true;
        }
    }

    COfficeUtils OfficeUtils(NULL);
    if (OfficeUtils.IsArchive(fileName) == S_OK)
	{
             if ( isOOXFormatFile(fileName) )           return true;
        else if ( isOpenOfficeFormatFile(fileName) )    return true;
        else if ( isOnlyOfficeFormatFile(fileName) )    return true;
        else if ( isXpsFile(fileName) )                 return true;
	}


    // others
    {
        NSFile::CFileBinary file;
        if (!file.OpenFile(fileName))
            return L"";
		
		unsigned char* buffer = new unsigned char[4096]; //enaf !!
        if (!buffer){file.CloseFile();return L"";}

        DWORD dwReadBytes = 0;
        file.ReadFile(buffer,MIN_SIZE_BUFFER,dwReadBytes);
        int sizeRead = (int)file.GetFilePosition();
		file.CloseFile();

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
        else if (isHtmlFormatFile(buffer,sizeRead) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
        }
        else if (isFB2FormatFile(buffer,sizeRead) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2;
        }
        else if (isPdfFormatFile(buffer,sizeRead) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
        }
        else if (isDjvuFormatFile(buffer,sizeRead) )
        {
            nFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU;
        }
	/////////////////////////////////////////////////////////////////////////
		if (buffer)delete []buffer;
		buffer = NULL;
	}
	if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN)return true;

//// by Extension

    std::wstring::size_type nExtPos = fileName.rfind(L'.');
	std::wstring sExt = fileName.substr(nExtPos);

	std::transform(sExt.begin(), sExt.end(), sExt.begin(), tolower);

    if (0 == sExt.compare(L".mht"))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
    else if (0 == sExt.compare(L".csv"))
		nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
    else if (0 == sExt.compare(L".html") || 0 == sExt.compare(L".htm"))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
    else if (0 == sExt.compare(L".bin")) //base64 string
		nFileType = AVS_OFFICESTUDIO_FILE_CANVAS_PDF;
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

        std::string::size_type res = 0;
        if ( (res = strContentTypes.find(docxFormatLine))>0 || (res = strContentTypes.find(dotxFormatLine))>0 ||
            (res = strContentTypes.find(docmFormatLine))>0 || (res = strContentTypes.find(dotmFormatLine))>0)
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		}

        else if ((res = strContentTypes.find(xlsxFormatLine))>0 || (res = strContentTypes.find(xltxFormatLine))>0 ||
            (res = strContentTypes.find(xlsmFormatLine))>0 || (res = strContentTypes.find(xltmFormatLine))>0)
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
		}

        else if ((res = strContentTypes.find(pptxFormatLine))>0 || /*(res = strContentTypes.find(ppsxFormatLine))>0 ||*/
            (res = strContentTypes.find(potxFormatLine))>0 || (res = strContentTypes.find(pptmFormatLine))>0 ||
            (res = strContentTypes.find(ppsmFormatLine))>0 || (res = strContentTypes.find(potmFormatLine))>0 )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		}
        else if ((res = strContentTypes.find(ppsxFormatLine)) >0)
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX;
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
bool COfficeFileFormatChecker::isOpenOfficeFormatFile(const std::wstring & fileName)
{
    const char *odtFormatLine = "application/vnd.oasis.opendocument.text";
    const char *odsFormatLine = "application/vnd.oasis.opendocument.spreadsheet";
    const char *odpFormatLine = "application/vnd.oasis.opendocument.presentation";
    const char *epubFormatLine = "application/epub+zip";

    COfficeUtils OfficeUtils(NULL);
	
	ULONG nBufferSize = 0;
	BYTE *pBuffer = NULL;

	HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"mimetype", &pBuffer, nBufferSize);
	if (hresult == S_OK && pBuffer != NULL)
	{
		
        if ( NULL != strstr((char*)pBuffer, odtFormatLine) )
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
    }else
    {
       //если не записан тип смотрим манифест
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

std::wstring COfficeFileFormatChecker::GetExtensionByType(int type)
{
    switch (type)
    {
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:		return L".docx";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:		return L".doc";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:		return L".odt";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF:		return L".rtf";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT:		return L".txt";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML:		return L".html";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT:		return L".mht";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB:		return L".epub";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2:		return L".fb2";
    case AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI:		return L".mobi";

    case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:	return L".pptx";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT:	return L".ppt";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:	return L".odp";
    case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:	return L".ppsx";

    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:	return L".xlsx";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS:		return L".xls";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:		return L".ods";
    case AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV:		return L".csv";

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
