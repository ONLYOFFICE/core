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

#include "OfficeFileFormats.h"

namespace POLE
{
	class Storage;
}
class COfficeFileFormatChecker
{
public:
	int				nFileType;
	bool			bMacroEnabled;
	std::wstring	sDocumentID;
	
	COfficeFileFormatChecker()
	{
		nFileType		= AVS_OFFICESTUDIO_FILE_UNKNOWN;
		bMacroEnabled	= false;
	}
	COfficeFileFormatChecker(std::wstring sFileName)
	{
		nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;

		isOfficeFile(sFileName);
	}
	
	bool isOfficeFile(const std::wstring & fileName);	
	bool isVbaProjectFile(const std::wstring & fileName);

	std::wstring GetFormatExtension(const std::wstring & fileName);
	std::wstring GetExtensionByType(int type);
	static int GetFormatByExtension(const std::wstring& ext);

	bool isOOXFormatFile		(const std::wstring & fileName, bool unpacked = false);
	bool isOpenOfficeFormatFile	(const std::wstring & fileName, std::wstring & documentID);
	bool isOnlyOfficeFormatFile	(const std::wstring & fileName);

	bool isDocFormatFile	(const std::wstring & fileName);
	bool isXlsFormatFile	(const std::wstring & fileName);

	bool isDocFormatFile	(POLE::Storage *storage);
	bool isXlsFormatFile	(POLE::Storage *storage);
	bool isPptFormatFile	(POLE::Storage *storage);
    bool isMS_OFFICECRYPTOFormatFile(POLE::Storage * storage, std::wstring & documentID);
	bool isMS_MITCRYPTOFormatFile(POLE::Storage * storage, std::wstring & documentID);
	bool isVbaProjectFile(POLE::Storage * storage);
	bool isMS_OFFCRYPTOFormatFile	(const std::wstring & fileName, std::wstring & documentID);

    std::wstring getDocumentID		(const std::wstring & fileName);
	
	bool isOOXFlatFormatFile(unsigned char* pBuffer,int dwBytes);
	
	bool isDocFlatFormatFile(unsigned char* pBuffer,int dwBytes);
	
	bool isXlsFlatFormatFile(unsigned char* pBuffer, int dwBytes);

	bool isRtfFormatFile	(unsigned char* pBuffer,int dwBytes);
	bool isHtmlFormatFile	(unsigned char* pBuffer,int dwBytes, bool testCloseTag);
	bool isMultiPartsHtmlFormatFile(unsigned char* pBuffer, int dwBytes);
	bool isPdfFormatFile	(unsigned char* pBuffer,int dwBytes, std::wstring & documentID);
	bool isOpenOfficeFlatFormatFile(unsigned char* pBuffer,int dwBytes);

	bool isBinaryDoctFormatFile	(unsigned char* pBuffer,int dwBytes);
	bool isBinaryXlstFormatFile	(unsigned char* pBuffer,int dwBytes);
	bool isBinaryPpttFormatFile	(unsigned char* pBuffer,int dwBytes);

    bool isDjvuFormatFile(unsigned char* pBuffer,int dwBytes);
    bool isMobiFormatFile(unsigned char* pBuffer,int dwBytes);
    bool isFB2FormatFile(unsigned char* pBuffer,int dwBytes);
    bool isXpsFile(const std::wstring & fileName);

};
