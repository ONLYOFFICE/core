/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef UTILITY_TXT_FILE_INCLUDE_H_
#define UTILITY_TXT_FILE_INCLUDE_H_

#include <string>
#include <list>

#include "../../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

class TxtFile
{
public:
	TxtFile(const std::wstring & path);

	const std::list<std::string>	readAnsiOrCodePage();
    const std::list<std::wstring>	readUnicodeFromBytes(char *file_data, long file_size);
	const std::list<std::wstring>	readUnicode();
	//const std::list<std::wstring>	readUnicodeWithOutBOM(); /// не используем
	const std::list<std::wstring>	readBigEndian();
	const std::list<std::string>	readUtf8();

	void writeAnsiOrCodePage	(const std::list<std::string>& content);
	void writeUnicode			(const std::list<std::wstring>& content);
	void writeBigEndian			(const std::list<std::wstring>& content);
	void writeUtf8				(const std::list<std::string>& content);

	const bool	isUnicode();
	const bool	isBigEndian();
	const bool	isUtf8();

	const int	getLinesCount();

private:
	std::wstring	m_path;
	int				m_linesCount;
};

#endif // UTILITY_TXT_FILE_INCLUDE_H_
