#pragma once

#include <string>

#include "OfficeFileFormats.h"

namespace POLE
{
	class Storage;
}
class COfficeFileFormatChecker
{
public:
	int nFileType;
	
	COfficeFileFormatChecker()
	{
		nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
	}
	COfficeFileFormatChecker(std::wstring sFileName)
	{
		nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;

		isOfficeFile(sFileName);
	}
	
	bool isOfficeFile(const std::wstring & fileName);	
	
	std::wstring GetFormatExtension(const std::wstring & fileName);
	std::wstring GetExtensionByType(int type);

private:

	bool isOOXFormatFile		(const std::wstring & fileName);
	bool isOpenOfficeFormatFile	(const std::wstring & fileName);
	bool isOnlyOfficeFormatFile	(const std::wstring & fileName);

	bool isDocFormatFile	(POLE::Storage *storage);
	bool isXlsFormatFile	(POLE::Storage *storage);
	bool isPptFormatFile	(POLE::Storage *storage);
    bool isMS_OFFCRYPTOFormatFile	(POLE::Storage * storage);

	bool isRtfFormatFile	(unsigned char* pBuffer,int dwBytes);
	bool isHtmlFormatFile	(unsigned char* pBuffer,int dwBytes);
	bool isPdfFormatFile	(unsigned char* pBuffer,int dwBytes);

	bool isBinaryDoctFormatFile	(unsigned char* pBuffer,int dwBytes);
	bool isBinaryXlstFormatFile	(unsigned char* pBuffer,int dwBytes);
	bool isBinaryPpttFormatFile	(unsigned char* pBuffer,int dwBytes);

    bool isDjvuFormatFile(unsigned char* pBuffer,int dwBytes);
    bool isMobiFormatFile(unsigned char* pBuffer,int dwBytes);
    bool isFB2FormatFile(unsigned char* pBuffer,int dwBytes);
    bool isXpsFile(const std::wstring & fileName);

};
