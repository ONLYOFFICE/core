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

#include "OfficeFileFormats.h"

namespace POLE
{
	class Storage;
}
class COfficeFileFormatChecker
{
public:
	int nFileType;
	bool bMacroEnabled;
	std::wstring sDocumentID;

	COfficeFileFormatChecker()
	{
		nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		bMacroEnabled = false;
	}
	COfficeFileFormatChecker(std::wstring sFileName)
	{
		nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;

		isOfficeFile(sFileName);
	}

	bool isOfficeFile(const std::wstring& fileName);
	bool isVbaProjectFile(const std::wstring& fileName);

	std::wstring GetFormatExtension(const std::wstring& fileName);
	std::wstring GetExtensionByType(int type);
	static int GetFormatByExtension(const std::wstring& ext);

	bool isOOXFormatFile(const std::wstring& fileName, bool unpacked = false);
	bool isOpenOfficeFormatFile(const std::wstring& fileName, std::wstring& documentID);
	bool isOnlyOfficeFormatFile(const std::wstring& fileName);
	bool isMacFormatFile(const std::wstring& fileName);
	bool isHwpxFile(const std::wstring& fileName);

	bool isDocFormatFile(const std::wstring& fileName);
	bool isXlsFormatFile(const std::wstring& fileName);

	bool isCompoundFile     (POLE::Storage* storage);
	bool isOleObjectFile(POLE::Storage* storage);
	bool isDocFormatFile(POLE::Storage* storage);
	bool isXlsFormatFile(POLE::Storage* storage);
	bool isPptFormatFile(POLE::Storage* storage);
	bool isMS_OFFICECRYPTOFormatFile(POLE::Storage* storage, std::wstring& documentID);
	bool isMS_MITCRYPTOFormatFile(POLE::Storage* storage, std::wstring& documentID);
	bool isVbaProjectFile(POLE::Storage* storage);
	bool isMS_OFFCRYPTOFormatFile(const std::wstring& fileName, std::wstring& documentID);
	bool isHwpFile(POLE::Storage* storage);

	bool iXmlFile(const std::wstring& fileName);

	std::wstring getDocumentID(const std::wstring& fileName);

	bool isOOXFlatFormatFile(unsigned char* pBuffer, int dwBytes);

	bool isDocFlatFormatFile(unsigned char* pBuffer, int dwBytes);

	bool isXlsFlatFormatFile(unsigned char* pBuffer, int dwBytes);

	bool isRtfFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isHtmlFormatFile(unsigned char* pBuffer, int dwBytes, bool testCloseTag);
	bool isMultiPartsHtmlFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isPdfFormatFile(unsigned char* pBuffer, int dwBytes, std::wstring& documentID);
	bool isPdfOformFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isOpenOfficeFlatFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isHwpmlFile(unsigned char* pBuffer, int dwBytes);

	bool isBinaryDoctFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isBinaryXlstFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isBinaryPpttFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isBinaryVsdtFormatFile(unsigned char* pBuffer, int dwBytes);

	bool isDjvuFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isMobiFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isFB2FormatFile(unsigned char* pBuffer, int dwBytes);
	bool isXpsFile(const std::wstring& fileName);
	bool isOFDFile(const std::wstring& fileName);
};
