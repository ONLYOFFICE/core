
#include "xlsx_borders.h"

#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "xlsx_border.h"
#include "../odf/datatypes/length.h"
#include "../odf/datatypes/borderstyle.h"
#include "../odf/style_table_properties.h"


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

    void xlsx_serialize(std::wostream & _Wostream)
    {
        std::vector<xlsx_border> inst_array;

        BOOST_FOREACH(const xlsx_border & inst, borders_)
        {
            inst_array.push_back(inst);
        }

        std::sort(inst_array.begin(), inst_array.end(), compare_());

        CP_XML_WRITER(_Wostream)
        {
            CP_XML_NODE(L"borders")
            {
                CP_XML_ATTR(L"count", inst_array.size());
                BOOST_FOREACH( xlsx_border & border, inst_array)
                {
                    cpdoccore::oox::xlsx_serialize(CP_XML_STREAM(), border);
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

void xlsx_borders::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->xlsx_serialize(_Wostream);
}

xlsx_borders::xlsx_borders(): impl_(new Impl())
{
}

xlsx_borders::~xlsx_borders()
{
}

void xlsx_serialize(std::wostream & _Wostream, xlsx_borders & borders)
{    
    return borders.xlsx_serialize(_Wostream);
}

}
}


