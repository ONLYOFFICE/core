#ifndef _BUILD_IMAGE_FILE_CHECKER_
#define _BUILD_IMAGE_FILE_CHECKER_

#include <string>
#include "../common/Types.h"

enum __ENUM_CXIMAGE_FORMATS {
_CXIMAGE_FORMAT_UNKNOWN = 0,
_CXIMAGE_FORMAT_BMP = 1,
_CXIMAGE_FORMAT_GIF = 2,
_CXIMAGE_FORMAT_JPG = 3,
_CXIMAGE_FORMAT_PNG = 4,
_CXIMAGE_FORMAT_ICO = 5,
_CXIMAGE_FORMAT_TIF = 6,
_CXIMAGE_FORMAT_TGA = 7,
_CXIMAGE_FORMAT_PCX = 8,
_CXIMAGE_FORMAT_WBMP = 9,
_CXIMAGE_FORMAT_WMF = 10,
_CXIMAGE_FORMAT_JP2 = 11,
_CXIMAGE_FORMAT_JPC = 12,
_CXIMAGE_FORMAT_PGX = 13,
_CXIMAGE_FORMAT_PNM = 14,
_CXIMAGE_FORMAT_RAS = 15,
_CXIMAGE_FORMAT_JBG = 16,
_CXIMAGE_FORMAT_MNG = 17,
_CXIMAGE_FORMAT_SKA = 18,
_CXIMAGE_FORMAT_RAW = 19,
_CXIMAGE_FORMAT_PSD = 20,
_CXIMAGE_FORMAT_EMF = 21,
_CXIMAGE_FORMAT_WB = 22,
};

class CImageFileFormatChecker
{
public:
	__ENUM_CXIMAGE_FORMATS eFileType;
	
	CImageFileFormatChecker()
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	CImageFileFormatChecker(std::wstring sFileName)
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
		isImageFile(sFileName);
	}
	
	bool isImageFile(std::wstring& fileName);
	bool isPngFile(std::wstring& fileName);

	bool isRawFile(std::wstring& fileName);
	bool isSvgFile(std::wstring& fileName);

	bool isBmpFile(BYTE* pBuffer,DWORD dwBytes);
	bool isGifFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPngFile(BYTE* pBuffer,DWORD dwBytes);
	bool isEmfFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWmfFile(BYTE* pBuffer,DWORD dwBytes);
	bool isTgaFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPcxFile(BYTE* pBuffer,DWORD dwBytes);
	bool isTiffFile(BYTE* pBuffer,DWORD dwBytes);
	bool isJpgFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWbFile(BYTE* pBuffer,DWORD dwBytes);

	bool isIcoFile(BYTE* pBuffer,DWORD dwBytes);
	bool isRasFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPsdFile(BYTE* pBuffer,DWORD dwBytes);
	bool isSvmFile(BYTE* pBuffer,DWORD dwBytes);
	bool isSwfFile(BYTE* pBuffer,DWORD dwBytes);
	bool isSfwFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWbcFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWbzFile(BYTE* pBuffer,DWORD dwBytes);
	bool isJ2kFile(BYTE* pBuffer,DWORD dwBytes);
	bool isJp2File(BYTE* pBuffer,DWORD dwBytes);
	bool isMj2File(BYTE* pBuffer,DWORD dwBytes);
	bool isIpodFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPgxFile(BYTE* pBuffer,DWORD dwBytes);

	std::wstring DetectFormatByData(BYTE *Data, int DataSize);

};

#endif //_BUILD_IMAGE_FILE_CHECKER_
