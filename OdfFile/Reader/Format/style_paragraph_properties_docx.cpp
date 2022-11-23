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

#include "odf_document.h"
#include "odfcontext.h"

#include "style_paragraph_properties.h"

#include <xml/simple_xml_writer.h>

#include "../../Reader/Converter/docx_conversion_context.h"

#include "../../DataTypes/borderstyle.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



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

        w_sz	= std::to_wstring( szInt );
        w_color = borderStyle.get_color().get_hex_value();

        if (borderPadding)
            w_space = std::to_wstring((int)(borderPadding->get_value_unit(length::pt) + 0.5) );

        switch(borderStyle.get_style())
        {
        case border_style::none:        w_val = L"none";    break;
        case border_style::solid:
        case border_style::single:
                                        w_val = L"single";  break;
        case border_style::double_:     w_val = L"double";  break;
        case border_style::dotted:      w_val = L"dotted";  break;
        case border_style::dashed:      w_val = L"dashed";  break;
        case border_style::groove:      w_val = L"thinThickMediumGap";  break;
        case border_style::ridge:       w_val = L"thickThinMediumGap";  break;
        case border_style::inset:       w_val = L"inset";  break;
        case border_style::outset:      w_val = L"outset";  break;
        case border_style::hidden:      w_val = L"nil";     break;
        }

    }
    std::wstring res;
    if (!w_val.empty())
        res += L" w:val=\"" + w_val + L"\" ";
    if (!w_sz.empty())
        res += L"w:sz=\"" + w_sz + L"\" ";
    if (!w_color.empty())
        res += L"w:color=\"" + w_color + L"\" ";
    if (!w_space.empty())
        res += L"w:space=\"" + w_space + L"\" ";
    if (!Shadow.empty())
        res += L"w:shadow=\"" + Shadow + L"\" ";

    return res;
}

std::wstring docx_process_margin(const _CP_OPT(length_or_percent) & margin, double Mul)
{
    if (margin)
    {
        if (margin->get_type() == length_or_percent::Length)
        {
            int val = (int)(0.5 + Mul * margin->get_length().get_value_unit(length::pt));
            return std::to_wstring( val );
        }
        else
        {
            _CP_LOG << L"[docx_convert] convert margin warning: invalid type (percent)\n";
        }       
    }
    return L"";
}

}

void paragraph_format_properties::docx_convert(oox::docx_conversion_context & Context)
{
	bool drawing	= false;

 	if (Context.get_drawing_context().get_current_shape() || Context.get_drawing_context().get_current_frame())
	{
		drawing = true;
	}

	const odf_reader::style_instance *style_inst = Context.get_styles_context().get_current_processed_style();

	std::wstringstream & _pPr = Context.get_styles_context().paragraph_nodes();
 
	CP_XML_WRITER(_pPr)
	{
 		if (style_writing_mode_)
		{
			writing_mode::type type = style_writing_mode_->get_type();
			switch(type)
			{
			case writing_mode::RlTb:
			case writing_mode::TbRl:
			case writing_mode::Rl:
				Context.set_rtl(true);
				break;
			default:
				Context.set_rtl(false);
			}
		}
		if (Context.get_rtl()) //может быть он установился от стиля родителя !!
		{
			_pPr << L"<w:bidi/>";
		}
		
		odf_reader::list_style_container & list_styles = Context.root()->odf_context().listStyleContainer();
		
		int level = Context.get_outline_level();
		bool bOutlineList = (list_styles.outline_style() != NULL);

		if (style_inst)
		{
			if (level < 0 && style_inst->outline_level())
			{ 
				level = style_inst->outline_level().get();
			}

			if ((style_inst->list_style_name()) && (style_inst->list_style_name()->empty()))
			{
				bOutlineList = false;
			}
		}
	
		if (level >= 0 && level < 10)
		{
			text_outline_style* outline_style = list_styles.outline_style();
			if (outline_style)
			{
				bool bListEnabled = true;
				if (level < outline_style->content_.size())
				{
					office_element_ptr elm = outline_style->content_[level];
					text_outline_level_style* outline_level = dynamic_cast<text_outline_level_style*>(elm.get());
					if (outline_level)
					{
						if (!outline_level->common_num_format_attlist_.style_num_format_)
							bListEnabled = false;
						else if (outline_level->common_num_format_attlist_.style_num_format_->get_type() == odf_types::style_numformat::none)
							bListEnabled = false;
					}
				}
				else
					bListEnabled = false;

				_pPr << L"<w:numPr>";
					if (bListEnabled && level < 9 && bOutlineList)
					{
						_pPr << L"<w:ilvl w:val=\"" << level - 1 << L"\"/>";
						_pPr << L"<w:numId w:val=\"" << list_styles.id_outline() << L"\"/>";
					}
					else
					{
						_pPr << L"<w:ilvl w:val=\"" << 0 << L"\"/>";
						_pPr << L"<w:numId w:val=\"" << 0 << L"\"/>";
					}
				_pPr << L"</w:numPr>";
			}
			_pPr << L"<w:outlineLvl w:val=\"" << level << L"\"/>";
		}
	    
		if (fo_text_align_)
		{
			std::wstring jc;
			switch(fo_text_align_->get_type())
			{
				case text_align::Left:			jc = L"left";	break;
				case text_align::Right:			jc = L"right";	break;
				case text_align::Center:		jc = L"center";	break;
				case text_align::Justify:		jc = L"both";	break;
				case text_align::Start:			jc = Context.get_rtl() ? L"end": L"start"; break;
				case text_align::End:			jc = Context.get_rtl() ? L"start": L"end"; break;
			}

			if (!jc.empty()) CP_XML_NODE(L"w:jc"){CP_XML_ATTR(L"w:val", jc );}
		}
		else if (fo_keep_with_next_) //&& Context.get_drop_cap_context().state() != 2
		{
			std::wstring val; 
			if (fo_keep_with_next_->get_type() == keep_together::Always)
				val = L"true";
			else if (fo_keep_with_next_->get_type() == keep_together::Auto)
				val = L"false";

			if (!val.empty())CP_XML_NODE(L"w:keepNext"){CP_XML_ATTR(L"w:val", val );}
		}
		if (fo_keep_together_)
		{
			std::wstring val;
			if (fo_keep_together_->get_type() == keep_together::Always)
				val = L"true";
			else if (fo_keep_together_->get_type() == keep_together::Auto)
				val = L"false";

			if (!val.empty())CP_XML_NODE(L"w:keepLines"){CP_XML_ATTR(L"w:val", val );}
		}
		if (Context.get_drop_cap_context().state() == 2)
		{
			CP_XML_NODE(L"w:keepNext");
			CP_XML_NODE(L"w:framePr")
			{
				CP_XML_ATTR(L"w:dropCap", L"drop");
				if (Context.get_drop_cap_context().Scale > 0)
				{
					CP_XML_ATTR(L"w:lines",Context.get_drop_cap_context().Scale);
				}
				else
				{
					CP_XML_ATTR(L"w:hSpace", Context.get_drop_cap_context().Space);	
				}
				CP_XML_ATTR(L"w:wrap", L"around"); 
				CP_XML_ATTR(L"w:hAnchor", L"text");
				CP_XML_ATTR(L"w:vAnchor", L"text");
			}
			
			CP_XML_NODE(L"w:spacing")
			{
				CP_XML_ATTR(L"w:after", 0); 
				if (Context.get_drop_cap_context().FontSize > 0)
					CP_XML_ATTR(L"w:line", Context.get_drop_cap_context().FontSize);
				else
					CP_XML_ATTR(L"w:line", 240);
				CP_XML_ATTR(L"w:lineRule", L"exact");
			}
			CP_XML_NODE(L"w:textAlignment"){CP_XML_ATTR(L"w:val", L"baseline");}
		}

		if (Context.get_page_break_before() == 2)
		{
			Context.set_page_break_before(0);
			CP_XML_NODE(L"w:pageBreakBefore"){CP_XML_ATTR(L"w:val", L"true");  }  
		}
		else if (fo_break_before_)
		{
			std::wstring val;
			if (fo_break_before_->get_type() == fo_break::Auto)
				val = L"false";
			else if (fo_break_before_->get_type() == fo_break::Page)
				val = L"true";
			else 
				Context.set_page_break_before(fo_break_before_->get_type());

			if (!val.empty())
				CP_XML_NODE(L"w:pageBreakBefore"){CP_XML_ATTR(L"w:val", val);}
		}
		if (fo_widows_)
		{
 			CP_XML_NODE(L"w:widowControl"){CP_XML_ATTR(L"w:val", bool (*fo_widows_ > 0));}
		}

		if ((fo_border_ || fo_border_top_ || fo_border_bottom_ || fo_border_left_ || fo_border_right_) && drawing == false)
		{
			std::wstring w_shadow;
	        
			if (style_shadow_)
				w_shadow = style_shadow_->get_type() != shadow_type::None ? L"1" : L"0";

			std::wstring w_top, w_left, w_right, w_bottom, w_between;
	        
			if (fo_border_)
			{
				w_top = w_left = w_right = w_bottom = process_border(*fo_border_, style_border_line_width_, fo_padding_, w_shadow);
				if (style_join_border_ && *style_join_border_)
				{
					w_between = w_top;
				}
			}
			else
			{
				if (fo_border_top_)
					w_top = process_border(*fo_border_top_, style_border_line_width_top_, fo_padding_top_, w_shadow);
				if (fo_border_bottom_)
					w_bottom = process_border(*fo_border_bottom_, style_border_line_width_bottom_, fo_padding_bottom_, w_shadow);
				if (fo_border_left_)
					w_left = process_border(*fo_border_left_, style_border_line_width_left_, fo_padding_left_, w_shadow);
				if (fo_border_right_)
					w_right = process_border(*fo_border_right_, style_border_line_width_right_, fo_padding_right_, w_shadow);
			}
			CP_XML_NODE(L"w:pBdr")
			{
				if (!w_top.empty())
					CP_XML_STREAM() << L"<w:top " << w_top << L" />";
				if (!w_left.empty())
					CP_XML_STREAM() << L"<w:left " << w_left << L" />";
				if (!w_right.empty())
					CP_XML_STREAM() << L"<w:right " << w_right << L" />";
				if (!w_bottom.empty())
					CP_XML_STREAM() << L"<w:bottom " << w_bottom << L" />";
			}
		}
		if ((style_line_height_at_least_ || fo_line_height_ || style_line_spacing_ ||
			fo_margin_top_ || fo_margin_bottom_ || fo_margin_) && Context.get_drop_cap_context().state() != 2)
		{
			std::wstring w_after, w_before;
			std::wstring w_line, w_lineRule;

            w_after = docx_process_margin(fo_margin_bottom_, 20.0);
            w_before = docx_process_margin(fo_margin_top_, 20.0);

			// TODO :   здесь 240 берется из корневого стиля? надо не константу использовать а брать оттуда
			//          в xsl преобразованиях так же написано 
			if (fo_line_height_)
			{
				if (fo_line_height_->get_type() == line_width::Percent)
				{
					w_line = std::to_wstring( (int)( 0.5 + fo_line_height_->get_percent().get_value() * 240.0 / 100.0 ) );
					w_lineRule = L"auto";
				}
				else if(fo_line_height_->get_type() == line_width::Normal) 
				{
					w_line = L"240";
					w_lineRule = L"exact";
				}
				else if (fo_line_height_->get_type() == line_width::PositiveLength)
				{
					w_line = std::to_wstring((int)(0.5 + 20.0 * fo_line_height_->get_positive_length().get_value_unit(length::pt)));
					w_lineRule = L"exact";
				}
			}
			else if (style_line_height_at_least_)
			{
				w_lineRule = L"atLeast";
				w_line = std::to_wstring((int)(0.5 + 20.0 * style_line_height_at_least_->get_value_unit(length::pt)));
			} 
			else if (style_line_spacing_)
			{
				w_lineRule = L"auto";
				w_line = std::to_wstring( (int)(0.5 + 240.0 + 20.0 * style_line_spacing_->get_value_unit(length::pt)) );
			}
			CP_XML_NODE(L"w:spacing")
			{
				if (!w_line.empty())
					CP_XML_ATTR(L"w:line", w_line);   				
				if (!w_after.empty())
					CP_XML_ATTR(L"w:after", w_after);
				if (!w_before.empty())
					CP_XML_ATTR(L"w:before",w_before);
				if (!w_lineRule.empty())
					CP_XML_ATTR(L"w:lineRule", w_lineRule);

			}
		}
		if (fo_margin_left_ || //? + буквица
			fo_margin_right_ || 
			(fo_text_indent_ && Context.get_drop_cap_context().state() != 1))
		{
			// TODO auto indent
			std::wstring w_left, w_right, w_hanging, w_firstLine;

            w_left = docx_process_margin(fo_margin_left_, 20.0);
            w_right = docx_process_margin(fo_margin_right_, 20.0);
            w_firstLine = docx_process_margin(fo_text_indent_, 20.0);

			if (w_left.empty())			w_left = L"0";
			if (w_right.empty())		w_right = L"0";
			if (w_firstLine.empty())	w_hanging = L"0";
	                
		   CP_XML_NODE(L"w:ind")
		   {
				CP_XML_ATTR(L"w:left", w_left);
				CP_XML_ATTR(L"w:right", w_right);
		        
				if (Context.get_drop_cap_context().state() != 1 )//состояние сразу после добавления буквицы - не нужны ни отступы, ни висячие
				{
					if (!w_firstLine.empty())
						CP_XML_ATTR(L"w:firstLine", w_firstLine);

					if (!w_hanging.empty())
						CP_XML_ATTR(L"w:hanging", w_hanging);
				}
			}
		}

		if (style_vertical_align_ && Context.get_drop_cap_context().state() != 2)
		{
			std::wstring w_val;
			switch(style_vertical_align_->get_type())
			{
				case vertical_align::Baseline:		w_val = L"baseline";break;
				case vertical_align::Top:			w_val = L"top";		break;
				case vertical_align::Middle:		w_val = L"center";	break;
				case vertical_align::Bottom:        w_val = L"bottom";	break;
				case vertical_align::Auto:          w_val = L"auto";	break;
			}

			if (!w_val.empty())
				CP_XML_NODE(L"w:textAlignment"){CP_XML_ATTR(L"w:val", w_val);}
		}
		if (style_punctuation_wrap_)
		{
			std::wstring w_val;
			if (style_punctuation_wrap_->get_type() == punctuation_wrap::Simple)
				w_val = L"false";
			else if (style_punctuation_wrap_->get_type() == punctuation_wrap::Hanging)
				w_val = L"true";
	        
			if (!w_val.empty())
				CP_XML_NODE(L"w:kinsoku"){CP_XML_ATTR(L"w:val", w_val);}
		}
		if (fo_background_color_)
		{
			const std::wstring w_fill = (fo_background_color_->get_type() == background_color::Enabled 
				? fo_background_color_->get_color().get_hex_value() : L"auto");
			CP_XML_NODE(L"w:shd")
			{
				CP_XML_ATTR(L"w:val", L"clear"); 
				CP_XML_ATTR(L"w:color", L"auto");
				CP_XML_ATTR(L"w:fill", w_fill );
			}
		}
	}
    if (fo_break_after_ && Context.in_automatic_style())
    {
		Context.set_page_break_after(fo_break_after_->get_type());
    }

	Context.get_tabs_context().docx_convert(Context);

	//if (style_tab_stops_)	
	//{
	//	style_tab_stops_->docx_convert(Context);
	//}
}
void style_tab_stops::docx_convert(oox::docx_conversion_context & Context)
{
	if (content_.empty()) return;
	std::wstringstream & _pPr = Context.get_styles_context().paragraph_nodes();
 
    _pPr << L"<w:tabs>";
		for (size_t i = 0; i < content_.size(); i++)
		{
			content_[i]->docx_convert(Context);
		}
    _pPr << L"</w:tabs>";
}
	
void style_tab_stop::docx_convert(oox::docx_conversion_context & Context, bool clear)
{
    std::wstringstream & _pPr = Context.get_styles_context().paragraph_nodes();

    _pPr << L"<w:tab";

	length def_tab =  length(1.0, length::cm);// в ms значение 0.8 не корректно оО
		
	double tab_pos = 20.0 * style_position_.get_value_unit(length::pt) + margin_left ;
	double min_tab_pos = 20.0 * def_tab.get_value_unit(length::pt) ;

	if (tab_pos < min_tab_pos)
		tab_pos = min_tab_pos;

	std::wstring val = clear ? L"clear" : L"left"; //????
	if (style_type_ && !clear)
	{
		switch(style_type_->get_type())
		{
			case style_type::Left:	val = L"left";		break;
			case style_type::Center:val = L"center";	break;
			case style_type::Char:	val = L"decimal";	break;
			case style_type::Right:	val = L"right";		break;
		}
	}

	_pPr << L" w:val=\"" << val << "\"";
    _pPr << L" w:pos=\"" << (int)tab_pos << "\"";
	
	std::wstring leader;

	if ((style_leader_type_ && style_leader_type_->get_type() == line_type::None) ||
		(style_leader_style_ && style_leader_style_->get_type() == line_style::None))
	{
		leader = L"none";
	}
	else if (!style_leader_type_ || style_leader_type_ && style_leader_type_->get_type() != line_type::None)
	{
		if (style_leader_style_)
		{
			switch(style_leader_style_->get_type())
			{
			case line_style::None:			leader.clear();			break;
			case line_style::Solid:
			{
				if (style_leader_text_)
				{
					if (*style_leader_text_ == L"-")
						leader = L"hyphen";
					else if (*style_leader_text_ == L"_")
						leader = L"underscore";
				}
			}break;
			case line_style::Dotted:		leader = L"dot";		break;
			case line_style::Dash:			leader = L"hyphen";		break;
			case line_style::LongDash:		leader = L"middleDot";	break;
			case line_style::DotDash:		leader = L"middleDot";	break;
			case line_style::DotDotDash:	leader = L"middleDot";	break;
			case line_style::Wave:			leader = L"middleDot";	break;
			}
		}
	}
	if (!clear && !leader.empty())
	{
		_pPr << L" w:leader=\"" << leader << "\"";
	}
	_pPr << L"/>";
}
void style_tab_stop::docx_convert(oox::docx_conversion_context & Context)
{
	docx_convert(Context, false);
}

void style_paragraph_properties::docx_convert(oox::docx_conversion_context & Context)
{
    content_.docx_convert(Context);
}


}
}
