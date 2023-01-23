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
#pragma once

#include <string>
#include "../common/Types.h"
#include "./../graphics/config.h"

enum __ENUM_CXIMAGE_FORMATS
{
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
	_CXIMAGE_FORMAT_SVM = 23,
	_CXIMAGE_FORMAT_SVG = 24,
};

class GRAPHICS_DECL CImageFileFormatChecker
{
public:
	__ENUM_CXIMAGE_FORMATS eFileType;
	
	CImageFileFormatChecker();
	CImageFileFormatChecker(std::wstring sFileName);
	CImageFileFormatChecker(BYTE* pBuffer, DWORD dwBytes);

	bool isImageFileInZip(std::wstring& fileName);
	
	bool isImageFile(std::wstring& fileName);
	bool isPngFile(std::wstring& fileName);
	bool isSvmFile(std::wstring& fileName);

	bool isRawFile(std::wstring& fileName);
	bool isSvgFile(std::wstring& fileName);

	bool isImageFile(BYTE* pBuffer,DWORD dwBytes);
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
	bool isSvgFile(BYTE* pBuffer,DWORD dwBytes);
	bool isRawFile(BYTE* pBuffer,DWORD dwBytes);

	std::wstring DetectFormatByData(BYTE *Data, int DataSize);

};
