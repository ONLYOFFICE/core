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

#include "xlsx_borders.h"

#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>
#include <xml/simple_xml_writer.h>

#include "xlsx_border.h"
#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"
#include "../Format/style_table_properties.h"


namespace cpdoccore {

	using namespace odf_types;

namespace oox {

namespace  {

// TODO надо сделать конвертацию в зависимости от стиля
std::wstring convert_border_style(const odf_types::border_style& borderStyle)
{
	std::wstring retVal = L"none";
   
	if (borderStyle.initialized())
    {
        if (borderStyle.get_style() == odf_types::border_style::none || borderStyle.is_none())
           retVal = L"none";
        else if (borderStyle.get_style() == odf_types::border_style::double_)
            retVal = L"double";
        else if (borderStyle.get_style() == odf_types::border_style::dotted)
            retVal = L"dotted";
        else if (borderStyle.get_style() == odf_types::border_style::dashed)
            retVal = L"dashed";
        else

            retVal = L"thin";            
    }
    return retVal;
}

void process_border(xlsx_border_edge & borderEdge, const _CP_OPT(border_style) & borderStyle)
{
    if (borderStyle)
    {
        xlsx_color color;    
        color.rgb = borderStyle->get_color().get_hex_value();

        borderEdge.color = color;
        borderEdge.style = convert_border_style(*borderStyle);
		borderEdge.width = boost::lexical_cast<int>(borderStyle->get_length().get_value_unit(odf_types::length::emu));
    }
}

bool check_border(const _CP_OPT(std::wstring) & odfBorderStyle)
{
    if (odfBorderStyle)
    {
        odf_types::border_style borderStyle(*odfBorderStyle);
       
		if (convert_border_style(borderStyle) != L"none")
            return true;
    }
    return false;
}

}

class xlsx_borders::Impl
{
public:
    Impl()
    {
        xlsx_border border;        
        border.left = xlsx_border_edge();
        border.right = xlsx_border_edge();
        border.top = xlsx_border_edge();
        border.bottom = xlsx_border_edge();
        border.index = 0;
        borders_.insert(border);
        //borders_.push_back(border);
    }

    size_t size() const
    {
        return borders_.size();
    }

    size_t borderId(odf_reader::style_table_cell_properties_attlist * cellProp)
    {
        bool is_default;
        return borderId(cellProp, is_default);
    }

    size_t borderId(const odf_reader::style_table_cell_properties_attlist * cellProp, bool & is_default_val)
    {
        xlsx_border border;

        border.left		= xlsx_border_edge();
        border.right	= xlsx_border_edge();
        border.top		= xlsx_border_edge();
        border.bottom	= xlsx_border_edge();
        
        if (cellProp)
        {
            const common_border_attlist & odfBordersAttr = cellProp->common_border_attlist_;
            
			process_border(*border.left,	odfBordersAttr.fo_border_);
            process_border(*border.right,	odfBordersAttr.fo_border_);
            process_border(*border.top,		odfBordersAttr.fo_border_);
            process_border(*border.bottom,	odfBordersAttr.fo_border_);

            process_border(*border.left,	odfBordersAttr.fo_border_left_);
            process_border(*border.right,	odfBordersAttr.fo_border_right_);
            process_border(*border.top,		odfBordersAttr.fo_border_top_);
            process_border(*border.bottom,	odfBordersAttr.fo_border_bottom_);

            if (check_border(cellProp->style_diagonal_bl_tr_))
            {
                border.diagonal = xlsx_border_edge();
                _CP_OPT(border_style) borderStyle(*cellProp->style_diagonal_bl_tr_);
                process_border(*border.diagonal, borderStyle);
                border.diagonalUp = true;
            }

            if (check_border(cellProp->style_diagonal_tl_br_))
            {
                if (!border.diagonal)
                    border.diagonal = xlsx_border_edge();
                _CP_OPT(border_style) borderStyle (*cellProp->style_diagonal_tl_br_);
               process_border(*border.diagonal, borderStyle);
                border.diagonalDown = true;
            }
        }

        if (is_default(border))
        {
            is_default_val = true;
            return 0;
        }
        else
        {
            is_default_val = false;
            xlsx_borders_array::const_iterator i = borders_.find(border);
            if (i != borders_.end())
            {
                return i->index;
            }
            else
            {
                border.index = borders_.size();
                borders_.insert(border);
                //borders_.push_back(border);
                return border.index;
            }
        }
    }

    struct compare_
    {
        template <class T>
        bool operator() (T const & x1, T const & x2)
        {
            return x1.index < x2.index;            
        }
    };

    void serialize(std::wostream & _Wostream)
    {
        std::vector<xlsx_border> inst_array;

		for (boost::unordered_set<xlsx_border, boost::hash<xlsx_border>>::iterator it = borders_.begin(); it != borders_.end(); ++it)
        {
            inst_array.push_back(*it);
        }

        std::sort(inst_array.begin(), inst_array.end(), compare_());

        CP_XML_WRITER(_Wostream)
        {
            CP_XML_NODE(L"borders")
            {
                CP_XML_ATTR(L"count", inst_array.size());
				
				for (size_t i = 0; i < inst_array.size(); i++)
                {
                    cpdoccore::oox::xlsx_serialize(CP_XML_STREAM(), inst_array[i]);
                }
            }
        }
    }

private:
    typedef boost::unordered_set<xlsx_border, boost::hash<xlsx_border> > xlsx_borders_array;
    xlsx_borders_array borders_;
};

size_t xlsx_borders::size() const
{
    return impl_->size();
}

size_t xlsx_borders::borderId(odf_reader::style_table_cell_properties_attlist * cellProp)
{
    return impl_->borderId(cellProp);
}

size_t xlsx_borders::borderId(const odf_reader::style_table_cell_properties_attlist * cellProp, bool & is_default)
{
    return impl_->borderId(cellProp, is_default);
}

void xlsx_borders::serialize(std::wostream & _Wostream)
{
    return impl_->serialize(_Wostream);
}

xlsx_borders::xlsx_borders(): impl_(new Impl())
{
}

xlsx_borders::~xlsx_borders()
{
}

}
}


