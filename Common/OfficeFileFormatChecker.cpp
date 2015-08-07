#include <algorithm>
#include "OfficeFileFormatChecker.h"
#include "DocxFormat/Source/SystemUtility/File.h"
#include "../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

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
		CFile file;
        if (file.OpenFile(std_string2string(fileName)) != S_OK ) return _T("");
		
		unsigned char* buffer = new unsigned char[4096]; //enaf !!
		if (!buffer){file.CloseFile();return _T("");}

		file.ReadFile(buffer,MIN_SIZE_BUFFER);
		int sizeRead = file.GetPosition();
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
            nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
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

	int nExtPos = fileName.rfind(_T('.'));
	std::wstring sExt = fileName.substr(nExtPos);

	std::transform(sExt.begin(), sExt.end(), sExt.begin(), tolower);

	if (0 == sExt.compare(_T(".mht")))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
	else if (0 == sExt.compare(_T(".csv")))
		nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
	else if (0 == sExt.compare(_T(".html")) || 0 == sExt.compare(_T(".htm")))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
	else if (0 == sExt.compare(_T(".bin"))) //base64 string 
		nFileType = AVS_OFFICESTUDIO_FILE_CANVAS_PDF;
    else //if (0 == sExt.compare(_T(".txt")) || 0 == sExt.compare(_T(".xml"))) //volsciv.rtf -или любой другой
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

        int res = 0;
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
            XmlUtils::CXmlNode oRoot;
            if(TRUE == oRoot.FromXmlStringA(xml_string))
            {
                XmlUtils::CXmlNodes oXmlNodes;
                if(TRUE == oRoot.GetChilds(oXmlNodes))
                {
                    for(int i = 0; i < oXmlNodes.GetCount(); ++i)
                    {
                        XmlUtils::CXmlNode oXmlNode;
                        if(oXmlNodes.GetAt(i, oXmlNode))
                        {
                            std::wstring sName = oXmlNode.GetName();
                            if(_T("manifest:file-entry") == sName)
                            {
                                CStringA value = oXmlNode.GetAttributeA(CStringA("manifest:media-type"));

                                if (value.GetLength() < 1) continue;
                                if ( value == odtFormatLine)
                                {
                                    nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
                                }

                                else if ( value == odsFormatLine)
                                {
                                    nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
                                }

                                else if ( value == odpFormatLine)
                                {
                                    nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
                                }
                            }
                        }
                    }
                }
            }
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
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:		return _T(".docx");
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:		return _T(".doc");
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF:		return _T(".rtf");
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT:		return _T(".txt");
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML:		return _T(".html");
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:		return _T(".odt");
	case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:	return _T(".xlsx");
	case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:		return _T(".ods");
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:	return _T(".pptx");
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:	return _T(".odp");
	case AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY:		return _T(".doct");
	case AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY:		return _T(".xlst");
	case AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY:		return _T(".pptt");

	}
	
	return _T("");
}

std::wstring COfficeFileFormatChecker::GetFormatExtension(const std::wstring & fileName)
{
	if (isOfficeFile(fileName))
		return GetExtensionByType(nFileType);

	return _T("");
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
