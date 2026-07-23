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
#include <vector>
#include <map>
#include <xml/utils.h>
#include <xml/simple_xml_writer.h>

#include "xlsx_numFmts.h"
#include "../../DataTypes/officevaluetype.h"

namespace cpdoccore {
namespace oox {

class xlsx_num_fmts::Impl
{
private:
	struct _desc
	{
		std::wstring code;
		unsigned int id;
	};
    std::vector<_desc> arrFormats;
	std::map<unsigned int, size_t> mapIdsFormat;
	std::map<std::wstring, size_t> mapFormatsCode;

	unsigned int last_custom_id = 164;

	unsigned int add(const std::wstring & format_code, unsigned int id);
public:   
	unsigned int add_or_find(std::wstring format_code, char type);
	void serialize(std::wostream & _Wostream) const;
};
unsigned int xlsx_num_fmts::Impl::add(const std::wstring & format_code, unsigned int id)
{
	if (format_code.empty()) return id;

	std::map<unsigned int, size_t>::iterator pFind = mapIdsFormat.find(id);

	if (pFind == mapIdsFormat.end())
	{
		mapIdsFormat.insert(std::make_pair(id, arrFormats.size()));
		mapFormatsCode.insert(std::make_pair(format_code, arrFormats.size()));

		_desc desc = {format_code, id};
		arrFormats.push_back(desc);

	}
	else
	{
		//id is taken for format_code -> take next if available
		id = add(format_code, ++id);
	}
	return id;
}
unsigned int xlsx_num_fmts::Impl::add_or_find(std::wstring format_code, char format_code_type)
{
	if (format_code.empty()) return 0;

	std::map<std::wstring, size_t>::iterator pFind = mapFormatsCode.find(format_code);

	if (pFind != mapFormatsCode.end())
	{
		return arrFormats[pFind->second].id;
	}
	else
	{ 
		unsigned int id = 0;
		if (format_code_type == odf_types::office_value_type::Currency)
		{
			if (std::wstring::npos != format_code.find(L"#,##0.00"))
			{
				if (std::wstring::npos != format_code.find(L"[Red]"))			id = 8; // "€"#,##0.00;[Red]\-"€"#,##0.00
				else															id = 7; // "€"#,##0.00;\-"€"#,##0.00
			}
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Date)
		{
			if (std::wstring::npos != format_code.find(L"mm-dd-yy"))			id = 14; 
			else if (std::wstring::npos != format_code.find(L"d-mmm-yy"))		id = 15; 
			else if (std::wstring::npos != format_code.find(L"d-mmm"))			id = 16; 
			else if (std::wstring::npos != format_code.find(L"mmm-yy"))			id = 17; 
			else if (std::wstring::npos != format_code.find(L"m/d/yy h:mm"))	id = 22; 
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Time)
		{
			if (std::wstring::npos != format_code.find(L"h:mm AM/PM"))			id = 18; 
			else if (std::wstring::npos != format_code.find(L"h:mm:ss AM/PM"))	id = 19; 
			else if (std::wstring::npos != format_code.find(L"m/d/yy h:mm"))	id = 22; 
			else if (std::wstring::npos != format_code.find(L"h:mm:ss"))		id = 21; 
			else if (std::wstring::npos != format_code.find(L"h:mm"))			id = 20; 
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Scientific)
		{
			if (std::wstring::npos != format_code.find(L"0.00E+00"))
			{
				format_code.clear();
				id = 11;
			}
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Fraction)
		{
			if (std::wstring::npos != format_code.find(L"??/??"))
			{
				format_code.clear();
				id = 13;
			}
			else if (std::wstring::npos != format_code.find(L"?/?"))
			{
				format_code.clear();
				id = 12;
			}
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Percentage)
		{
			if (std::wstring::npos != format_code.find(L"0.00%"))
			{
				format_code.clear();
				id = 10;
			}
			else if (std::wstring::npos != format_code.find(L"0%"))
			{
				format_code.clear();
				id = 9;
			}
			else
				id = last_custom_id++;
		}
		else
		{
			id = last_custom_id++;
		}
		return add(format_code, id);
	}
}
void xlsx_num_fmts::Impl::serialize(std::wostream & strm) const
{
    CP_XML_WRITER(strm)
    {
        CP_XML_NODE (L"numFmts")
        {
            CP_XML_ATTR (L"formatCode", arrFormats.size());
			
			for (size_t i = 0; i < arrFormats.size(); ++i)
			{
				if (false == arrFormats[i].code.empty())
				{
					CP_XML_NODE(L"numFmt")
					{
						CP_XML_ATTR(L"numFmtId", arrFormats[i].id);
						CP_XML_ATTR(L"formatCode", arrFormats[i].code);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------
xlsx_num_fmts::xlsx_num_fmts() : impl_(new xlsx_num_fmts::Impl())
{
}
xlsx_num_fmts::~xlsx_num_fmts()
{
}
unsigned int xlsx_num_fmts::add_or_find(const std::wstring & format_code, char format_code_type)
{
    return impl_->add_or_find(format_code, format_code_type);
}
void xlsx_num_fmts::serialize(std::wostream & strm) const
{
	impl_->serialize(strm);
}
   
}
}
