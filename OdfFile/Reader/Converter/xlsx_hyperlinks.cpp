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

#include <xml/simple_xml_writer.h>

#include "xlsx_hyperlinks.h"
#include "oox_rels.h"
#include "../../Formulas/formulasconvert.h"
#include "../../../OOXML/Base/Unit.h"
#include "xlsx_utils.h"

namespace cpdoccore {
namespace oox {

class xlsx_hyperlinks::Impl
{
public:
    Impl() {}
    ~Impl() {}

    struct record
    {
        std::wstring ref;
        std::wstring display;
        std::wstring location;
        std::wstring id;
        std::wstring href;
        std::wstring type;
	};
	
	void dump_rels(rels & Rels) const
	{
		for (size_t i = 0; i < records_.size(); i++)
		{
			if (records_[i].type == L"External")
			{
				Rels.add( relationship(records_[i].id, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink", 
					records_[i].location, records_[i].type) );
			}
		}
	}

    std::wstring add(std::wstring const & ref, std::wstring const & target1, std::wstring const & display)
    {
		std::wstring target =target1;

		int res = target.rfind(L"/");
		if (res >=0 && res+1==target.length())
		{
			target.resize(target.length()-1);
		}
		
		record r;
        r.ref = ref;      
		r.display = display;
		r.id = std::wstring(L"hId") + std::to_wstring(records_.size()+1);

        if (0 == target.find(L"#"))//ссыль на страницу или метку в текущем документе
        {			
			r.location = converter_.convert_chart_distance(target.substr(1));
			r.type = L"Internal";
        }
		else
		{
			r.location = XmlUtils::EncodeXmlString(converter_.convert_spacechar(target));
			r.type = L"External" ;
		}
        records_.push_back(r);

		return r.id;
    }

    void xlsx_serialize(std::wostream & _Wostream) const
    {
		for (size_t i = 0; i < records_.size(); i++)
        {
            CP_XML_WRITER(_Wostream)
            {
                CP_XML_NODE(L"hyperlink")
                {
                    CP_XML_ATTR(L"ref", records_[i].ref);
                    CP_XML_ATTR(L"display", XmlUtils::EncodeXmlString(records_[i].display));
                  
					if (!records_[i].location.empty() && records_[i].type == L"Internal")
                    {
                        CP_XML_ATTR(L"location", records_[i].location);
                    }

                    if (!records_[i].id.empty() && records_[i].type == L"External")
                    {
                        CP_XML_ATTR(L"r:id", records_[i].id);                    
                    }
                }
            }
        }
    }


private:
    std::vector<record> records_;
    formulasconvert::odf2oox_converter converter_;

};

xlsx_hyperlinks::xlsx_hyperlinks() : impl_( new xlsx_hyperlinks::Impl() )
{
}

xlsx_hyperlinks::~xlsx_hyperlinks()
{
}

std::wstring xlsx_hyperlinks::add(std::wstring const & ref, std::wstring const & target, std::wstring const & display)
{   
    return impl_->add(ref, target, display);
}

void xlsx_hyperlinks::xlsx_serialize(std::wostream & _Wostream) const
{
    return impl_->xlsx_serialize(_Wostream);
}
void xlsx_hyperlinks::dump_rels(rels & Rels) const
{
	return impl_->dump_rels(Rels);
}

}
}
