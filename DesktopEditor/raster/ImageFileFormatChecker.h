/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
    _CXIMAGE_FORMAT_PIC = 25,
	_CXIMAGE_FORMAT_HEIF = 26,
	_CXIMAGE_FORMAT_WEBP = 27
};

class GRAPHICS_DECL CImageFileFormatChecker
{
public:
	__ENUM_CXIMAGE_FORMATS eFileType;
	
	CImageFileFormatChecker();
	CImageFileFormatChecker(const std::wstring& sFileName);
	CImageFileFormatChecker(BYTE* pBuffer, DWORD dwBytes);

	bool isImageFileInZip(const std::wstring& fileName);
	
	bool isImageFile(const std::wstring& fileName);
	bool isPngFile(const std::wstring& fileName);
	bool isSvmFile(const std::wstring& fileName);

	bool isRawFile(const std::wstring& fileName);
	bool isSvgFile(const std::wstring& fileName);
	bool isHeifFile(const std::wstring& fileName);

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
	bool isWebPFile(BYTE* pBuffer, DWORD dwBytes);

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
    bool isPicFile(BYTE* pBuffer,DWORD dwBytes);
	bool isHeifFile(BYTE* pBuffer, DWORD dwBytes);

	std::wstring DetectFormatByData(BYTE *Data, int DataSize);

};
