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
#include "xlsx_fill.h"
#include "xlsx_fills.h"

#include "../Format/style_text_properties.h"
#include "../Format/style_table_properties.h"
#include "../Format/style_paragraph_properties.h"

#include <xml/simple_xml_writer.h>

#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>

namespace cpdoccore {
namespace oox {

class xlsx_fills::Impl
{
public:

    typedef boost::unordered_set<xlsx_fill, boost::hash<xlsx_fill> > xlsx_fill_array;

    xlsx_fill_array fills_;
};

xlsx_fills::xlsx_fills(): impl_( new xlsx_fills::Impl() )
{
//defaults fill
     {
        xlsx_patternFill patternFill;
        patternFill.patternType = L"none";

        xlsx_fill fill;
        fill.patternFill = patternFill;
        fill.index = 0;
		fill.bDefault = true;

        impl_->fills_.insert(fill);
    }

    {
        xlsx_patternFill patternFill;
        patternFill.patternType = L"gray125";

        xlsx_fill fill;
        fill.patternFill = patternFill;
        fill.index = 1;
		fill.bDefault = false;

        impl_->fills_.insert(fill);
    }

}

xlsx_fills::~xlsx_fills()
{
}

size_t xlsx_fills::size() const
{
    return impl_->fills_.size();
}

size_t xlsx_fills::fillId(	const odf_reader::text_format_properties_content_ptr	textProp,
							const odf_reader::paragraph_format_properties			* parProp,
							const odf_reader::style_table_cell_properties_attlist	* cellProp, bool default_set)
{
    bool is_default;
    return fillId(textProp, parProp, cellProp,default_set, is_default);
}

size_t xlsx_fills::fillId(	const odf_reader::text_format_properties_content_ptr	textProp,
							const odf_reader::paragraph_format_properties			* parProp,
							const odf_reader::style_table_cell_properties_attlist	* cellProp, 
							bool default_set, bool & is_default)
{
	is_default = true;
	xlsx_fill fill(NULL, cellProp);
	
	if (fill.bEnabled)
	{
		fill.bDefault = default_set;

        Impl::xlsx_fill_array::const_iterator i = impl_->fills_.find(fill);
       
		if (i != impl_->fills_.end())
        {
            const std::size_t dbgId = i->index;
			if (default_set && i->bDefault != default_set)
			{
				fill.index = i->index;
				impl_->fills_.insert(i,fill);
			
				is_default = default_set;
				return fill.index;
			}

			is_default = i->bDefault;
            return dbgId;
			
        }
        else
        {
            fill.index = impl_->fills_.size();
            impl_->fills_.insert(fill);
		
			is_default = default_set;
            return fill.index;
       }
	}

    return 0;
}

namespace 
{

struct compare_xlsx_fills
{
    bool operator() (xlsx_fill const & x1, xlsx_fill const & x2)
    {
        return x1.index < x2.index;            
    }
};

}

void xlsx_fills::serialize(std::wostream & _Wostream) const
{
    std::vector<xlsx_fill> inst_array;
            	
	for (boost::unordered_set<xlsx_fill, boost::hash<xlsx_fill>>::iterator it = impl_->fills_.begin(); it != impl_->fills_.end(); ++it)
	{
        inst_array.push_back(*it);
    }

    std::sort(inst_array.begin(), inst_array.end(), compare_xlsx_fills());

    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"fills")
        {
            CP_XML_ATTR(L"count", inst_array.size());
			
			for (size_t i = 0; i < inst_array.size(); i++)
            {
                xlsx_serialize(CP_XML_STREAM(), inst_array[i]);
            }
        }
    }
}

}
}
