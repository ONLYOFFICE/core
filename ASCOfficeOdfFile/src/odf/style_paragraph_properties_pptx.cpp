#include "precompiled_cpodf.h"
#include <iostream>
#include "style_paragraph_properties.h"
#include <boost/foreach.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "../docx/pptx_conversion_context.h"
#include "datatypes/borderstyle.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

namespace {

std::wstring process_border(const border_style & borderStyle,
    const _CP_OPT(border_widths) & borderLineWidths,
    const _CP_OPT(length) & borderPadding, const std::wstring & Shadow = L"")
{
    std::wstring w_sz;
    std::wstring w_color;
    std::wstring w_val = L"single";
    std::wstring w_space;

    if (!borderStyle.initialized() || borderStyle.is_none())
    {
        w_val = L"none";
    }
    else if (borderStyle.initialized())
    {
        double width = borderStyle.get_length().get_value_unit(length::pt);
        //borderLineWidths ? borderLineWidths->get_summ_unit(length::pt) : borderStyle.get_length().get_value_unit(length::pt);
        int szInt = (int)(0.5 + 8.0 * width);
        if (szInt <= 0)
            szInt = 1;
        w_sz = boost::lexical_cast<std::wstring>( szInt );
        w_color = boost::lexical_cast<std::wstring>( borderStyle.get_color().get_hex_value() );

        if (borderPadding)
            w_space = boost::lexical_cast<std::wstring>((int)(borderPadding->get_value_unit(length::pt) + 0.5) );

        const std::wstring borderStyleStr = borderStyle.get_style();
        if (szInt == 0)
            w_val = L"none";
        else if (borderStyleStr == L"solid" 
            || borderStyleStr == L"single")
            w_val = L"single";
        else if (borderStyleStr == L"double")
            w_val = L"double";
        else if (borderStyleStr == L"dotted")
            w_val = borderStyleStr;
        else if (borderStyleStr == L"dashed")
            w_val = borderStyleStr;
        else if (borderStyleStr == L"groove")
            w_val = L"thinThickMediumGap";
        else if (borderStyleStr == L"ridge")
            w_val = L"thickThinMediumGap";
        else if (borderStyleStr == L"inset")
            w_val = L"inset";
        else if (borderStyleStr == L"outset")
            w_val = L"outset";
        else if (borderStyleStr == L"hidden")
            w_val = L"nil";

    }
    std::wstring res;
    if (!w_val.empty())
        res += L" a:val=\"" + w_val + L"\" ";
    if (!w_sz.empty())
        res += L"a:sz=\"" + w_sz + L"\" ";
    if (!w_color.empty())
        res += L"a:color=\"" + w_color + L"\" ";
    if (!w_space.empty())
        res += L"a:space=\"" + w_space + L"\" ";
    if (!Shadow.empty())
        res += L"a:shadow=\"" + Shadow + L"\" ";

    return res;
}

std::wstring process_margin(const _CP_OPT(length_or_percent) & margin, length::unit unit, double Mul)
{
    if (margin)
    {
        if (margin->get_type() == length_or_percent::Length)
        {
            int val = (int)(0.5 + Mul * margin->get_length().get_value_unit(unit));
            return boost::lexical_cast<std::wstring>( val );
        }
        else
        {
        }       
    }
    return L"";
}

}

void paragraph_format_properties::pptx_convert(oox::pptx_conversion_context & Context)
{
	std::wstringstream & _pPr = Context.get_text_context().get_styles_context().paragraph_attr();
 //сначала аттрибуты к самому pPr или defPPr

	if (fo_text_align_)
	{
		std::wstring jc;
		switch(fo_text_align_->get_type())
		{
		case text_align::Start:
		case text_align::Left:
			jc = L"l";		break;
		case text_align::End:
		case text_align::Right:
			jc = L"r";		break;
		case text_align::Center:	
			jc = L"ctr";	break;
		case text_align::Justify:
			jc = L"just";	break;
		}
		if (jc.length()>0) _pPr << L"algn=\"" << jc << "\" ";
	}
	if (fo_margin_left_ || fo_margin_right_ || fo_text_indent_ )
	{
		// TODO auto indent
		std::wstring w_left, w_right, w_firstLine;

		w_left = process_margin(fo_margin_left_, length::emu, 1.);
		w_right = process_margin(fo_margin_right_, length::emu, 1.);
		w_firstLine = process_margin(fo_text_indent_,length::emu, 1.);

		//if (w_left.empty())			w_left = L"0";
		//if (w_right.empty())		w_right = L"0";
		//if (w_firstLine.empty())	w_hanging = L"0";
                
		if (!w_left.empty())
			_pPr << L"marL=\"" << w_left << "\" ";
		if (!w_right.empty())
			_pPr << L"marR=\"" << w_right << "\" ";        
		if (!w_firstLine.empty())
			_pPr << L"indent=\"" << w_firstLine << "\" ";
	}
	if (style_vertical_align_)
	{
		std::wstring w_val;
		switch(style_vertical_align_->get_type())
		{
			case vertical_align::Baseline:		w_val = L"base";break;
			case vertical_align::Top:			w_val = L"t";		break;
			case vertical_align::Middle:		w_val = L"ctr";	break;
			case vertical_align::Bottom:        w_val = L"b";	break;
			case vertical_align::Auto:          w_val = L"auto";	break;
		}

		if (!w_val.empty())
			_pPr << L"fontAlgn=\""  << w_val << "\" ";
	}
	//if (style_writing_mode_)
	//{
	//	writing_mode::type type = style_writing_mode_->get_type();
	//	switch(type)
	//	{
	//	case writing_mode::RlTb:
	//	case writing_mode::TbRl:
	//	case writing_mode::Rl:
	//		Context.set_rtl(true);
	//		break;
	//	default:
	//		Context.set_rtl(false);
	//	}
	//}
////////////////////////////////////////////////////////////////////////
 //теперь nodes  к pPr или defPPr
	CP_XML_WRITER(Context.get_text_context().get_styles_context().paragraph_nodes())
	{
		//if (fo_keep_with_next_) //&& Context.get_drop_cap_context().state() != 2
		//{
		//	std::wstring val; 
		//	if (fo_keep_with_next_->get_type() == keep_together::Always)
		//		val = L"true";
		//	else if (fo_keep_with_next_->get_type() == keep_together::Auto)
		//		val = L"false";

		//	if (!val.empty())CP_XML_NODE(L"a:keepNext"){CP_XML_ATTR(L"a:val", val );}
		//}
		//if (fo_keep_together_)
		//{
		//	std::wstring val;
		//	if (fo_keep_together_->get_type() == keep_together::Always)
		//		val = L"true";
		//	else if (fo_keep_together_->get_type() == keep_together::Auto)
		//		val = L"false";

		//	if (!val.empty())CP_XML_NODE(L"a:keepLines"){CP_XML_ATTR(L"a:val", val );}
		//}

		//if (fo_widows_)
		//{
 	//		CP_XML_NODE(L"a:widowControl"){CP_XML_ATTR(L"a:val", bool (*fo_widows_ > 0));}
		//}
		//if (fo_border_ || fo_border_top_ || fo_border_bottom_ || fo_border_left_ || fo_border_right_)
		//{
		//	std::wstring w_shadow;
	 //       
		//	if (style_shadow_)
		//		w_shadow = style_shadow_->get_type() != shadow_type::None ? L"1" : L"0";

		//	std::wstring w_top, w_left, w_right, w_bottom, w_between;
	 //       
		//	if (fo_border_)
		//	{
		//		w_top = w_left = w_right = w_bottom = process_border(*fo_border_, style_border_line_width_, fo_padding_, w_shadow);
		//		if (style_join_border_ && *style_join_border_)
		//		{
		//			w_between = w_top;
		//		}
		//	}
		//	else
		//	{
		//		if (fo_border_top_)
		//			w_top = process_border(*fo_border_top_, style_border_line_width_top_, fo_padding_top_, w_shadow);
		//		if (fo_border_bottom_)
		//			w_bottom = process_border(*fo_border_bottom_, style_border_line_width_bottom_, fo_padding_bottom_, w_shadow);
		//		if (fo_border_left_)
		//			w_left = process_border(*fo_border_left_, style_border_line_width_left_, fo_padding_left_, w_shadow);
		//		if (fo_border_right_)
		//			w_right = process_border(*fo_border_right_, style_border_line_width_right_, fo_padding_right_, w_shadow);
		//	}
		//	CP_XML_NODE(L"a:pBdr")
		//	{
		//		if (!w_top.empty())
		//			CP_XML_STREAM() << L"<a:top " << w_top << L" />";
		//		if (!w_left.empty())
		//			CP_XML_STREAM() << L"<a:left " << w_left << L" />";
		//		if (!w_right.empty())
		//			CP_XML_STREAM() << L"<a:right " << w_right << L" />";
		//		if (!w_bottom.empty())
		//			CP_XML_STREAM() << L"<a:bottom " << w_bottom << L" />";
		//	}
		//}

		if (style_line_height_at_least_ || fo_line_height_ || style_line_spacing_)
		{
			std::wstring w_line, w_lineRule;
			if (fo_line_height_)
			{
				if (fo_line_height_->get_type() == line_width::Percent)
				{
					double percent = fo_line_height_->get_percent().get_value();
					w_line = boost::lexical_cast<std::wstring>( (int)( 0.5 + percent *1000. ) );
					w_lineRule = L"a:spcPct";
				}
				//else if(fo_line_height_->get_type() == line_width::Normal) 
				//{
				//}
				else if (fo_line_height_->get_type() == line_width::PositiveLength)
				{
					w_line = boost::lexical_cast<std::wstring>((int)(0.5 + 100.0 * fo_line_height_->get_positive_length().get_value_unit(length::pt)));
					w_lineRule = L"a:spcPts";
				}
			}
			else if (style_line_height_at_least_)
			{
				w_lineRule = L"a:spcPts";
				w_line = boost::lexical_cast<std::wstring>((int)(0.5 + 100.0 * style_line_height_at_least_->get_value_unit(length::pt)));
			} 
			else if (style_line_spacing_)
			{
				w_lineRule = L"a:spcPts";
				w_line = boost::lexical_cast<std::wstring>( (int)(0.5 + 240.0 + 100.0 * style_line_spacing_->get_value_unit(length::pt)) );
		
			}
			CP_XML_NODE(L"a:lnSpc")
			{
				CP_XML_NODE(w_lineRule)
				{
					CP_XML_ATTR(L"val",w_line);
				}
			}	
		}
		if (fo_margin_top_ || fo_margin_)
		{
			std::wstring w_before = process_margin(fo_margin_top_, length::pt, 1000.0);
			CP_XML_NODE(L"a:spcBef")
			{
				CP_XML_NODE(L"a:spcPct")
				{
					CP_XML_ATTR(L"val",w_before);
				}
			}
		}
		if (fo_margin_bottom_ || fo_margin_)
		{
			std::wstring w_after = process_margin(fo_margin_bottom_, length::pt, 1000.0);
			CP_XML_NODE(L"a:spcAft")
			{
				CP_XML_NODE(L"a:spcPct")
				{
					CP_XML_ATTR(L"val",w_after);
				}
			}
		}
		//if (style_punctuation_wrap_)
		//{
		//	std::wstring w_val;
		//	if (style_punctuation_wrap_->get_type() == punctuation_wrap::Simple)
		//		w_val = L"false";
		//	else if (style_punctuation_wrap_->get_type() == punctuation_wrap::Hanging)
		//		w_val = L"true";
	 //       
		//	if (!w_val.empty())
		//		CP_XML_NODE(L"a:kinsoku"){CP_XML_ATTR(L"a:val", w_val);}
		//}
		//if (fo_background_color_)
		//{
		//	const std::wstring w_fill = (fo_background_color_->get_type() == background_color::Enabled 
		//		? fo_background_color_->get_color().get_hex_value() : L"auto");
		//	CP_XML_NODE(L"a:shd")
		//	{
		//		CP_XML_ATTR(L"a:val", L"clear"); 
		//		CP_XML_ATTR(L"a:color", L"auto");
		//		CP_XML_ATTR(L"a:fill", w_fill );
		//	}
		//}
	}

	//if (style_tab_stops_)	
	//	style_tab_stops_->pptx_convert(Context);
}
void style_tab_stops::pptx_convert(oox::pptx_conversion_context & Context)
{
	//if (style_tab_stops_.size()<1)return;
	//std::wstringstream & _pPr = Context.get_text_context().get_styles_context().paragraph_nodes();
 //
 //   _pPr << L"<a:tabLst>";

	//if (style_tab_stops_.size())
	//{
	//	BOOST_FOREACH(const office_element_ptr & elm, style_tab_stops_)
	//	{
	//		elm->pptx_convert(Context);
	//	}
	//}
 //   _pPr << L"</a:tabLst>";
}

void style_tab_stop::pptx_convert(oox::pptx_conversion_context & Context)
{
    //std::wstringstream & _pPr = Context.get_text_context().get_styles_context().paragraph_nodes();

    //_pPr << L"<a:tab ";
    //
    //_pPr << L"a:pos=\"" << (int)( 20.0 * style_position_.get_value_unit(length::pt) ) << "\" ";
    //
    //{
    //    std::wstring val = L"left";
    //    if (style_type_)
    //    {
    //        switch(style_type_->get_type())
    //        {
    //        case style_type::Left:
    //            val = L"left";
    //            break;
    //        case style_type::Center:
    //            val = L"center";
    //            break;
    //        case style_type::Right:
    //            val = L"right";
    //            break;
    //        case style_type::Char:
    //            val = L"decimal";            
    //            break;
    //        }
    //    }
    //    if (!val.empty())
    //        _pPr << L"a:val=\"" << val << "\" ";
    //}

    //{
    //    std::wstring leader;

    //    if (style_leader_type_ && style_leader_type_->get_type() == line_type::None ||
    //        style_leader_style_ && style_leader_style_->get_type() == line_style::None)
    //    {
    //        leader = L"none";
    //    }
    //    else if (
    //        !style_leader_type_ ||
    //        style_leader_type_ && style_leader_type_->get_type() != line_type::None
    //        )
    //    {
    //        if (style_leader_style_)
    //        {
    //            _CP_LOG(info) << L"[pptx_convert] convert warning (a:tab/@a:leader)\n";
    //            switch(style_leader_style_->get_type())
    //            {
    //            case line_style::None:
    //                leader = L"";
    //                break;
    //            case line_style::Solid:
    //                if (style_leader_text_)
    //                {
    //                    if (*style_leader_text_ == L"-")
    //                        leader = L"hyphen";
    //                    else if (*style_leader_text_ == L"_")
    //                        leader = L"underscore";
    //                }
    //                break;
    //            case line_style::Dotted:
    //                leader = L"dot";
    //                break;
    //            case line_style::Dash:
    //                leader = L"hyphen";
    //                break;
    //            case line_style::LongDash:
    //                leader = L"middleDot";
    //                break;
    //            case line_style::DotDash:
    //                leader = L"middleDot";
    //                break;
    //            case line_style::DotDotDash:
    //                leader = L"middleDot";
    //                break;
    //            case line_style::Wave:
    //                leader = L"middleDot";
    //                break;
    //            }
    //        }
    //    }

    //    if (!leader.empty())
    //        _pPr << L"a:leader=\"" << leader << "\" ";
    //}
    //
    //_pPr << L" />";
}

void style_paragraph_properties::pptx_convert(oox::pptx_conversion_context & Context)
{
    style_paragraph_properties_content_.pptx_convert(Context);
}


}
}
