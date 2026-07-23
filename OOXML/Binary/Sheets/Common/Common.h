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

#include "../../../Base/Unit.h"
#include "../../../SystemUtility/SystemUtility.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/common/StringBuilder.h"

#include <string>
#include <vector>

#define BUFFER_GROW_SIZE 1 * 1024 * 1024 //1mb

namespace SerializeCommon
{
    std::wstring DownloadImage(const std::wstring& strFile);
    VOID convertBase64ToImage (NSFile::CFileBinary& oFile, std::wstring &pBase64);
	long Round(double val);
    std::wstring changeExtention(const std::wstring& sSourcePath, const std::wstring& sTargetExt);

	class CommentData
	{
	public :
		std::wstring sText;
		std::wstring sTime;
		std::wstring sOOTime;
		std::wstring sUserId;
		std::wstring sUserName;
		std::wstring sUserData;
		std::wstring sQuoteText;
		std::wstring sGuid;
		bool Solved;
		bool Document;

		bool bSolved;
		bool bDocument;
		std::vector<CommentData*> aReplies;

		CommentData();
		~CommentData();
	};

    void ReadFileType(const std::wstring& sXMLOptions, BYTE& result, UINT& nCodePage, std::wstring& wcDelimiter, BYTE& saveFileType, _INT32& Lcid);
}

