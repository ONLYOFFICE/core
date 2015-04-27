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

	//Html File is XML-file with rootElement - html 
	 for (int i = 0; i < dwBytes - 2; i++)
	 {
		 if (0x3C == pBuffer[i] && (0x48 == pBuffer[i + 1] || 0x68 == pBuffer[i + 1]) && (0x54 == pBuffer[i + 2] || 0x74 == pBuffer[i + 2]) && (0x4d == pBuffer[i + 3] || 0x6d == pBuffer[i + 3]) && (0x4c == pBuffer[i + 4] || 0x6c == pBuffer[i + 4]))
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

	if ( (5 <= dwBytes) && (  '%' == pBuffer[0] && 'P' == pBuffer[1] && 'D' == pBuffer[2] && 'F' == pBuffer[3] && '-' == pBuffer[4] ))
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
	COfficeUtils OfficeUtils(NULL);

	POLE::Storage storage(fileName.c_str());

	if (OfficeUtils.IsArchive(fileName))
	{
		if ( isOOXFormatFile(fileName) )
		{
			//inside
		}
		else if ( isOpenOfficeFormatFile(fileName) )
		{
			//inside
		}
		else if ( isOnlyOfficeFormatFile(fileName) )
		{
			//inside
		}
	}
	else if (storage.open())
	{
		if ( isDocFormatFile(&storage) )
		{
		}
		else if ( isXlsFormatFile(&storage) )
		{
		}
		else if ( isPptFormatFile(&storage) )
		{
		}
	}
	else 
	{
		CFile file;
		if (file.OpenFile(std_string2string(fileName)) == false) return _T("");
		
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
		//else if (isHtmlFormatFile(buffer,sizeRead) )
		//{
		//	nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
		//}
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
	else if (0 == sExt.compare(_T(".txt")) || 0 == sExt.compare(_T(".xml")))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
	else if (0 == sExt.compare(_T(".csv")))
		nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
	else if (0 == sExt.compare(_T(".html")) || 0 == sExt.compare(_T(".htm")))
		nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
	else if (0 == sExt.compare(_T(".bin"))) //base64 string 
		nFileType = AVS_OFFICESTUDIO_FILE_CANVAS_PDF;

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
		
		if (strstr((char*)pBuffer, docxFormatLine)==0 || strstr((char*)pBuffer, dotxFormatLine) ==0 ||
			strstr((char*)pBuffer, docmFormatLine)==0 || strstr((char*)pBuffer, dotmFormatLine) ==0)
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		}

		else if (strstr((char*)pBuffer, xlsxFormatLine)==0 || strstr((char*)pBuffer, xltxFormatLine) ==0 ||
			strstr((char*)pBuffer, xlsmFormatLine)==0 || strstr((char*)pBuffer, xltmFormatLine) ==0)
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
		}

		else if (strstr((char*)pBuffer, pptxFormatLine)==0 || /*strstr((char*)pBuffer, ppsxFormatLine) ==0 ||*/
			strstr((char*)pBuffer, potxFormatLine)==0 || strstr((char*)pBuffer, pptmFormatLine) ==0 ||
			strstr((char*)pBuffer, ppsmFormatLine)==0 || strstr((char*)pBuffer, potmFormatLine) ==0 )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		}
		else if (strstr((char*)pBuffer, ppsxFormatLine) ==0 )
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
	COfficeUtils OfficeUtils(NULL);
	
	ULONG nBufferSize = 0;
	BYTE *pBuffer = NULL;

	HRESULT hresult = OfficeUtils.LoadFileFromArchive(fileName, L"mimetype", &pBuffer, nBufferSize);
	if (hresult == S_OK && pBuffer != NULL)
	{
		const char *odtFormatLine = "application/vnd.oasis.opendocument.text";	
		const char *odsFormatLine = "application/vnd.oasis.opendocument.spreadsheet";		
		const char *odpFormatLine = "application/vnd.oasis.opendocument.presentation";			 
		
		if ( strstr((char*)pBuffer, odtFormatLine)==0 )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
		}

		else if ( strstr((char*)pBuffer, odsFormatLine)==0 )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
		}

		else if ( strstr((char*)pBuffer, odpFormatLine)==0 )
		{
			nFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
		}

		delete []pBuffer;
		pBuffer = NULL;

		if (nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN) return true;
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

