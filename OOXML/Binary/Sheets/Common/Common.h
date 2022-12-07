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

	void ReadFileType(const std::wstring& sXMLOptions, BYTE& result, UINT& nCodePage, std::wstring& wcDelimiter, BYTE& saveFileType);
}

