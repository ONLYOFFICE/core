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

#include "xlsx_defined_names.h"

#include <vector>

#include "../../Formulas/formulasconvert.h"
#include <xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

class xlsx_defined_names::Impl
{
public:
    void add(std::wstring const & name, std::wstring const & ref, bool formula, int tableId)
    {
        formulasconvert::odf2oox_converter converter;
        std::wstring oox_ref;
		
		if (formula)
		{
			oox_ref = converter.convert_named_expr(ref);
		}
		else
		{
			oox_ref = converter.convert_named_ref(ref);
		}
		add(name, oox_ref, tableId);
    }
	void add(std::wstring const& name, std::wstring const& oox_ref, int tableId)
	{
		content_.emplace_back();

		content_.back().name = name;
		content_.back().ref = oox_ref;
		content_.back().tableId = tableId;
	}
    void xlsx_serialize(std::wostream & _Wostream)
    {
        if (content_.empty()) return;

        CP_XML_WRITER(_Wostream)
        {
            CP_XML_NODE(L"definedNames")
            {
                for (size_t i = 0 ; i < content_.size(); i++)
                {
                    CP_XML_NODE(L"definedName")
                    {
                        CP_XML_ATTR(L"name", XmlUtils::EncodeXmlString(content_[i].name));
						
						if (content_[i].tableId >= 0)
						{
							CP_XML_ATTR(L"localSheetId", content_[i].tableId);
						}	
						
						if ( content_[i].ref.find(L"#REF!") != std::wstring::npos )
						{
							CP_XML_ATTR(L"comment", XmlUtils::EncodeXmlString(content_[i].ref));
							CP_XML_CONTENT(L"#REF!");
						}
						else
							CP_XML_CONTENT(XmlUtils::EncodeXmlString(content_[i].ref));

                    }
                }
            }
        }
    }

private:
	struct name_and_ref
	{
		name_and_ref() : tableId(-1) {}

		std::wstring	name;
		std::wstring	ref;
		int				tableId;
	};
	std::vector<name_and_ref> content_;
};

xlsx_defined_names::xlsx_defined_names() : impl_(new xlsx_defined_names::Impl())
{
}

xlsx_defined_names::~xlsx_defined_names()
{
}

void xlsx_defined_names::add(std::wstring const & name, std::wstring const & ref, bool formula, int tableId)
{
    return impl_->add(name, ref, formula, tableId);        
}
void xlsx_defined_names::add(std::wstring const& name, std::wstring const& oox_ref, int tableId)
{
	return impl_->add(name, oox_ref, tableId);
}
void xlsx_defined_names::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->xlsx_serialize(_Wostream);
}

}
}
