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
#include <vector>
#include "../../../../Common/cfcpp/guid.h"

namespace XLS
{
	class CellRangeRef;
};

namespace AUX
{
	const int			normalizeColumn	(const int column);
	const int			normalizeRow	(const int row);

	const std::wstring	column2str		(const int column, const bool col_rel);
	const std::wstring	row2str			(const int row, const bool row_rel);
    const std::wstring	loc2str			(const int row, const bool row_rel, const int column, const bool col_rel);
   
	void				str2loc			(const std::wstring& str, int& row, bool& row_rel, int& column, bool& col_rel);
    void				str2loc			(std::wstring::const_iterator& str_begin, std::wstring::const_iterator& str_end, int& row, bool& row_rel, int& column, bool& col_rel);
	void				str2refs		(const std::wstring& str, std::vector<XLS::CellRangeRef>& vec);
};


namespace STR
{
	const std::string	int2str		(const int val, const int radix = 10);
	const std::wstring	int2wstr	(const int val, const int radix = 10);
	const std::wstring	double2str	(const double val);
	const std::string	bin2str		(const char* buf, const size_t nbuf);
	const std::wstring  guid2bstr	(_GUID_ & guid);
	const std::string	guid2str	(_GUID_ & guid);
    const std::wstring	guidFromStr	(const std::wstring & guid_str);
	const bool			bstr2guid	(const std::wstring & guid_str, _GUID_& guid);
	const std::wstring	int2hex_wstr(const int val, const size_t size_of = 4);

	const std::wstring	wchar_t2hex_str		(const wchar_t val);
	const std::wstring	escape_ST_Xstring	(const std::wstring& str);
	const std::wstring	unescape_ST_Xstring	(const std::wstring& str);

	const std::wstring	toARGB		(const _UINT32 rgba);
	const std::wstring	toARGB		(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha);
	const std::wstring	toRGB		(const _UINT32 rgba);
	const std::wstring	toRGB		(const unsigned char red, const unsigned char green, const unsigned char blue);
	const bool			fromARGB	(const std::wstring& argb, unsigned char& red, unsigned char& green, unsigned char& blue, unsigned char& alpha);
	
	const size_t		hex_str2int	(const std::wstring& hex);
	const size_t		hex_str2int	(const std::wstring::const_iterator& it_begin, const std::wstring::const_iterator& it_end);

	std::string		toStdString	(std::wstring wide_string, const _UINT32 code_page);
	std::wstring	toStdWString(std::string ansi_string, const _UINT32 code_page);
	std::wstring	toStdWString(char* ansi, int size, const _UINT32 code_page);
	//std::wstring	toStdWStringSystem(std::string ansi_string, const _UINT32 code_page);  .. to UnicodeConverter
};

namespace XMLSTUFF
{;

const std::wstring name2sheet_name(std::wstring name, const std::wstring prefix);
const std::wstring xti_indexes2sheet_name(const short tabFirst, const short tabLast, std::vector<std::wstring>& names, const std::wstring prefix = L"");

}

