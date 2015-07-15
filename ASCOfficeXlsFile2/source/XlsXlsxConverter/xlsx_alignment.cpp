
#include "xlsx_alignment.h"

#include <ostream>

#include <boost/functional.hpp>
#include <boost/functional/hash/hash.hpp>

#include "simple_xml_writer.h"



namespace oox {

bool xlsx_alignment::operator == (const xlsx_alignment & rVal) const
{
    const bool res = 
        horizontal == rVal.horizontal &&
        indent == rVal.indent &&
        justifyLastLine.get_value_or(false) == rVal.justifyLastLine.get_value_or(false) &&
        readingOrder == rVal.readingOrder &&
        relativeIndent == rVal.relativeIndent &&
        shrinkToFit.get_value_or(false) == rVal.shrinkToFit.get_value_or(false) &&
        textRotation.get_value_or(0) == rVal.textRotation.get_value_or(0) &&
        vertical == rVal.vertical &&
        wrapText.get_value_or(false) == rVal.wrapText.get_value_or(false);   

    return res;
}

bool xlsx_alignment::operator != (const xlsx_alignment & rVal) const
{
    return !(this->operator ==(rVal));
}
std::size_t hash_value(xlsx_alignment const & val)
{
    std::size_t seed = 0;
	boost::hash_combine(seed, val.horizontal.get_value_or(L""));
    boost::hash_combine(seed, val.indent.get_value_or(0));
    boost::hash_combine(seed, val.justifyLastLine.get_value_or(false));
    boost::hash_combine(seed, val.readingOrder.get_value_or(0));
    boost::hash_combine(seed, val.relativeIndent.get_value_or(0));
    boost::hash_combine(seed, val.shrinkToFit.get_value_or(false));
    boost::hash_combine(seed, val.textRotation.get_value_or(0));
    boost::hash_combine(seed, val.vertical.get_value_or(L""));
    boost::hash_combine(seed, val.wrapText.get_value_or(false));
    return seed;

}

bool is_default(const xlsx_alignment & rVal)
{
    const xlsx_alignment defaultAlignment;
    return rVal == defaultAlignment;
}

xlsx_alignment OdfProperties2XlsxAlignment()
{
    xlsx_alignment alignment;

    //if (parProp && parProp->fo_text_align_)
    //{
    //    switch(parProp->fo_text_align_->get_type())
    //    {
    //    default:
    //    case odf_types::text_align::Start:
    //    case odf_types::text_align::Left:
    //        alignment.horizontal = L"left";
    //        break;
    //    case odf_types::text_align::Right:
    //    case odf_types::text_align::End:
    //        alignment.horizontal = L"right";
    //        break;
    //    case odf_types::text_align::Center:
    //        alignment.horizontal = L"center";
    //        break;
    //    case odf_types::text_align::Justify:
    //        alignment.horizontal = L"justify";
    //        break;        
    //    }
    //}

    //// TODO : indent

    //if (parProp && parProp->fo_text_align_last_ &&
    //    parProp->fo_text_align_last_->get_type() == odf_types::text_align::Justify)
    //{
    //    alignment.justifyLastLine = true;            
    //}

    //if (textProp && textProp->style_text_rotation_angle_)
    //{
    //    alignment.textRotation = textProp->style_text_rotation_angle_.get();
    //}

    //if (cellProp && cellProp->common_rotation_angle_attlist_.style_rotation_angle_)
    //{
    //    alignment.textRotation = cellProp->common_rotation_angle_attlist_.style_rotation_angle_.get();        
    //}


    //_CP_OPT(odf_types::vertical_align) v_align;
    //
    //if (parProp && parProp->style_vertical_align_)
    //    v_align = parProp->style_vertical_align_;
    //else if (cellProp && cellProp->style_vertical_align_)
    //    v_align = cellProp->style_vertical_align_;
    //
    //if (v_align)
    //{
    //    switch(v_align->get_type())
    //    {        
    //    case odf_types::vertical_align::Top:
    //        alignment.vertical = L"top";
    //        break;
    //    default:
    //    case odf_types::vertical_align::Auto:
    //    case odf_types::vertical_align::Middle:
    //        alignment.vertical = L"center";
    //        break;
    //    case odf_types::vertical_align::Baseline:
    //    case odf_types::vertical_align::Bottom:
    //        alignment.vertical = L"bottom";
    //        break;
    //    case odf_types::vertical_align::Justify:
    //        alignment.vertical = L"justify";
    //        break;
    //    }
    //}
    //

    //if (cellProp &&
    //    cellProp->fo_wrap_option_ &&
    //    cellProp->fo_wrap_option_->get_type() == odf_types::wrap_option::Wrap
    //    )
    //{
    //    alignment.wrapText = true;
    //}

    return alignment;
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_alignment & alignment)
{
    if (is_default(alignment))
        return;
    
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"alignment")
        {
            if (alignment.horizontal)
                CP_XML_ATTR(L"horizontal", alignment.horizontal.get());
            
            if (alignment.indent)
                CP_XML_ATTR(L"indent", alignment.indent.get());

            if (alignment.justifyLastLine)
                CP_XML_ATTR(L"justifyLastLine", alignment.justifyLastLine.get());

            if (alignment.readingOrder)
                CP_XML_ATTR(L"readingOrder", alignment.readingOrder.get());

            if (alignment.relativeIndent)
                CP_XML_ATTR(L"relativeIndent", alignment.relativeIndent.get());

            if (alignment.shrinkToFit)
                CP_XML_ATTR(L"shrinkToFit", alignment.shrinkToFit.get());

            if (alignment.textRotation)
                CP_XML_ATTR(L"textRotation", alignment.textRotation.get());

            if (alignment.vertical)
                CP_XML_ATTR(L"vertical", alignment.vertical.get());

            if (alignment.wrapText)
                CP_XML_ATTR(L"wrapText", alignment.wrapText.get());
        }
    }

}

}

