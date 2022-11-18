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
        content_.push_back(name_and_ref());
		
		content_.back().name	= name;
		content_.back().ref		= oox_ref;
		content_.back().tableId	= tableId;
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

void xlsx_defined_names::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->xlsx_serialize(_Wostream);
}

}
}
