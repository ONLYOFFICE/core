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
	unsigned int add_or_find(const std::wstring & format_code, char type);
	void serialize(std::wostream & _Wostream) const;
};
unsigned int xlsx_num_fmts::Impl::add(const std::wstring & format_code, unsigned int id)
{
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
		//занято id для format_code -> следующий если есть берем
		id = add(format_code, ++id);
	}
	return id;
}
unsigned int xlsx_num_fmts::Impl::add_or_find(const std::wstring & format_code, char format_code_type)
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
			else if (std::wstring::npos != format_code.find(L"h:mm"))			id = 20; 
			else if (std::wstring::npos != format_code.find(L"h:mm:ss"))		id = 21; 
			else if (std::wstring::npos != format_code.find(L"m/d/yy h:mm"))	id = 22; 
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Scientific)
		{
			if (std::wstring::npos != format_code.find(L"0.00E+00"))			id = 11;
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Fraction)
		{
			if (std::wstring::npos != format_code.find(L"??/??"))				id = 13;
			else if (std::wstring::npos != format_code.find(L"?/?"))			id = 12; 
			else
				id = last_custom_id++;
		}
		else if (format_code_type == odf_types::office_value_type::Percentage)
		{
			if (std::wstring::npos != format_code.find(L"0.00%"))				id = 10;
			else if (std::wstring::npos != format_code.find(L"0%"))				id = 9;
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
				CP_XML_NODE(L"numFmt")
				{	
					CP_XML_ATTR (L"numFmtId", arrFormats[i].id);
					CP_XML_ATTR (L"formatCode", arrFormats[i].code);
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
