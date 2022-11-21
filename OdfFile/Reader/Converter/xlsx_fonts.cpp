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

#include "xlsx_fonts.h"
#include "xlsx_font.h"

#include <xml/simple_xml_writer.h>

#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>

namespace cpdoccore {
namespace oox {

class xlsx_fonts::Impl
{
public:
	Impl(){}


    size_t size() const;
    size_t fontId(	const odf_reader::text_format_properties_content_ptr	textProp,
					const odf_reader::paragraph_format_properties			* parProp,
					const odf_reader::style_table_cell_properties_attlist	* cellProp);

	void serialize(std::wostream & _Wostream) const;

    
private:
    typedef boost::unordered_set<xlsx_font, boost::hash<xlsx_font> > fonts_array_t;
    fonts_array_t fonts_;

};

namespace 
{

struct compare_xlsx_fonts
{
    bool operator() (xlsx_font const & x1, xlsx_font const & x2)
    {
        return x1.index < x2.index;            
    }
};

}

void xlsx_fonts::Impl::serialize(std::wostream & _Wostream) const
{
    std::vector<xlsx_font> fonts;
            
	for (boost::unordered_set<xlsx_font, boost::hash<xlsx_font>>::iterator it = fonts_.begin(); it != fonts_.end(); ++it)
    {
        fonts.push_back(*it);
    }

    std::sort(fonts.begin(), fonts.end(), compare_xlsx_fonts());

    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"fonts")
        {
            CP_XML_ATTR(L"count", fonts.size());
			
			for (size_t i = 0; i < fonts.size(); ++i)
			{
				xlsx_serialize(CP_XML_STREAM(), fonts[i]);
			}
		}
	}
}

size_t xlsx_fonts::Impl::fontId(const odf_reader::text_format_properties_content_ptr	textProp,
								const odf_reader::paragraph_format_properties			* parProp,
								const odf_reader::style_table_cell_properties_attlist	* cellProp)
{
    xlsx_font fnt = xlsx_font(textProp, parProp, cellProp);
   
	fonts_array_t::const_iterator i = fonts_.find(fnt);
    if (i != fonts_.end())
        return i->index;
    else
    {
        fnt.index = fonts_.size();
        fonts_.insert(fnt);
        //fonts_.push_back(OdfFont2XlsxFont(textProp, parProp, cellProp));
        return fnt.index;
    }
}

//const xlsx_font & xlsx_fonts::Impl::getFont(size_t id) const
//{
//    return fonts_.at(id);
//}

/*
int xlsx_fonts::Impl::getIndex(const odf_reader::text_format_properties_content_ptr textProp,
                         const odf_reader::paragraph_format_properties * parProp,
                         const odf_reader::style_table_cell_properties_attlist * cellProp) const
{
    const xlsx_font newFont = OdfFont2XlsxFont(textProp, parProp, cellProp);
    for (size_t i = 0; i < fonts_.size(); ++i)
    {
        if (fonts_[i] == newFont)
            return static_cast<int>(i);
    }
    return -1;
}
*/

size_t xlsx_fonts::Impl::size() const { return fonts_.size(); }


////////////////////////////////////////////////////////////////////////////////////////////////

xlsx_fonts::xlsx_fonts(): impl_(new xlsx_fonts::Impl())
{
}

xlsx_fonts::~xlsx_fonts()
{
}

size_t xlsx_fonts::size() const
{
    return impl_->size();
}

void xlsx_fonts::serialize(std::wostream & _Wostream) const
{
    return impl_->serialize(_Wostream);
}

size_t xlsx_fonts::fontId(const odf_reader::text_format_properties_content_ptr textProp,
    const odf_reader::paragraph_format_properties * parProp,
    const odf_reader::style_table_cell_properties_attlist * cellProp)
{
    return impl_->fontId(textProp, parProp, cellProp);
}

//const xlsx_font & xlsx_fonts::getFont(size_t id) const
//{
//    return impl_->getFont(id);
//}    

}
}
