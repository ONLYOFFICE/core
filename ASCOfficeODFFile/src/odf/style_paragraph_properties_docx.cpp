#include "precompiled_cpodf.h"
#include <iostream>
#include "style_paragraph_properties.h"
#include <boost/foreach.hpp>

#include "borderstyle.h"

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

std::wstring process_margin(const _CP_OPT(length_or_percent) & margin, double Mul)
{
    if (margin)
    {
        if (margin->get_type() == length_or_percent::Length)
        {
            int val = (int)(0.5 + Mul * margin->get_length().get_value_unit(length::pt));
            return boost::lexical_cast<std::wstring>( val );
        }
        else
        {
#ifdef _DEBUG
			_CP_LOG(info) << L"[docx_convert] convert margin warning: invalid type (percent)\n";
#endif
        }       
    }
    return L"";
}

}

/*
(from http://www.schemacentral.com/sc/ooxml/e-w_pPr-2.html)
[ ] w:pStyle                Referenced Paragraph Style
[+] w:keepNext              Keep Paragraph With Next Paragraph
[+] w:keepLines             Keep All Lines On One Page
[+] w:pageBreakBefore       Start Paragraph on Next Page
[ ] w:framePr               Text Frame Properties
[+] w:widowControl          Allow First/Last Line to Display on a Separate Page
[ ] w:numPr                 Numbering Definition Instance Reference
[ ] w:suppressLineNumbers   Suppress Line Numbers for Paragraph
[+] w:pBdr                  Paragraph Borders
[ ] w:shd                   Paragraph Shading
[+] w:tabs                  Set of Custom Tab Stops
[+] w:suppressAutoHyphens   Suppress Hyphenation for Paragraph
[ ] w:kinsoku               Use East Asian Typography Rules for First and Last Character per Line
[ ] w:wordWrap              Allow Line Breaking At Character Level
[ ] w:overflowPunct         Allow Punctuation to Extent Past Text Extents
[ ] w:topLinePunct          Compress Punctuation at Start of a Line
[ ] w:autoSpaceDE           Automatically Adjust Spacing of Latin and East Asian Text
[ ] w:autoSpaceDN           Automatically Adjust Spacing of East Asian Text and Numbers
[ ] w:bidi                  Right to Left Paragraph Layout
[ ] w:adjustRightInd        Automatically Adjust Right Indent When Using Document Grid
[ ] w:snapToGrid            Use Document Grid Settings for Inter-Line Paragraph Spacing
[+] w:spacing               Spacing Between Lines and Above/Below Paragraph
[+] w:ind                   Paragraph Indentation
[ ] w:contextualSpacing     Ignore Spacing Above and Below When Using Identical Styles
[ ] w:mirrorIndents         Use Left/Right Indents as Inside/Outside Indents
[ ] w:suppressOverlap       Prevent Text Frames From Overlapping
[+] w:jc                    Paragraph Alignment
[ ] w:textDirection         Paragraph Text Flow Direction
[+] w:ment         Vertical Character Alignment on Line
[ ] w:textboxTightWrap      Allow Surrounding Paragraphs to Tight Wrap to Text Box Contents
[ ] w:outlineLvl            Associated Outline Level
[ ] w:divId                 Associated HTML div ID
[ ] w:cnfStyle              Paragraph Conditional Formatting
[ ] w:rPr                   Run Properties for the Paragraph Mark
[ ] w:sectPr                Section Properties
[ ] w:pPrChange             Revision Information for Paragraph Properties
*/

void paragraph_format_properties::docx_convert(oox::docx_conversion_context & Context)
{
   std::wstringstream & _pPr = Context.get_styles_context().paragraph_style();

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
        
    if (fo_text_align_)
    {
        std::wstring jc;
        switch(fo_text_align_->get_type())
        {
        case text_align::Start:
        case text_align::Left:
            jc = L"left";
            break;
        case text_align::End:
        case text_align::Right:
            jc = L"right";
            break;
        case text_align::Center:
            jc = L"center";
            break;
        case text_align::Justify:
            jc = L"both";
            break;
        }

        if (!jc.empty())
            _pPr << L"<w:jc w:val=\"" << jc << L"\" />";
    }
    else if (fo_keep_with_next_) //&& Context.get_drop_cap_context().state() != 2
    {
        std::wstring val; 
        if (fo_keep_with_next_->get_type() == keep_together::Always)
            val = L"true";
        else if (fo_keep_with_next_->get_type() == keep_together::Auto)
            val = L"false";

        if (!val.empty())
            _pPr << L"<w:keepNext w:val=\"" << val << L"\" />";
    }

    if (fo_keep_together_)
    {
        std::wstring val;
        if (fo_keep_together_->get_type() == keep_together::Always)
            val = L"true";
        else if (fo_keep_together_->get_type() == keep_together::Auto)
            val = L"false";

        if (!val.empty())
            _pPr << L"<w:keepLines w:val=\"" << val << L"\" />";
    }
	if (Context.get_drop_cap_context().state() == 2)
	{
		_pPr << L"<w:keepNext/> ";
		_pPr << L"<w:framePr w:hAnchor=\"text\" w:vAnchor=\"text\" w:wrap=\"around\" w:dropCap=\"drop\" ";
				_pPr << L"w:hSpace=\"" << Context.get_drop_cap_context().Space << "\" ";	
				_pPr << L"w:lines=\"" << Context.get_drop_cap_context().Scale << "\" ";
		_pPr << L"/> ";
		
		_pPr << L"<w:spacing w:after=\"0\" w:lineRule=\"exact\" ";
		if (Context.get_drop_cap_context().FontSize>0)
				_pPr << L"w:line=\"" << Context.get_drop_cap_context().FontSize << L"\" ";
		else
				_pPr << L"w:line=\"240\" ";
		_pPr << L"/> ";
		_pPr << L"<w:textAlignment w:val=\"baseline\" />";
	}

    if (Context.check_page_break_before())
    {
        _pPr << L"<w:pageBreakBefore w:val=\"true\" />";    
    }
    else if (fo_break_before_)
    {
        std::wstring val;
        if (fo_break_before_->get_type() == fo_break::Auto)
            val = L"false";
        else if (fo_break_before_->get_type() == fo_break::Page)
            val = L"true";

        if (!val.empty())
            _pPr << L"<w:pageBreakBefore w:val=\"" << val << L"\" />";
    }

    if (fo_widows_)
    {
        std::wstring val;
        if (*fo_widows_ > 0)
            val = L"true";
        else        
            val = L"false";
        _pPr << L"<w:widowControl w:val=\"" << val << L"\" />";
    }

    if (fo_border_ || fo_border_top_ || fo_border_bottom_ || fo_border_left_ || fo_border_right_)
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


        _pPr << L"<w:pBdr>";
        if (!w_top.empty())
            _pPr << L"<w:top " << w_top << L" />";
        if (!w_left.empty())
            _pPr << L"<w:left " << w_left << L" />";
        if (!w_right.empty())
            _pPr << L"<w:right " << w_right << L" />";
        if (!w_bottom.empty())
            _pPr << L"<w:bottom " << w_bottom << L" />";
        _pPr << L"</w:pBdr>";

    }
    if ((style_line_height_at_least_ || fo_line_height_ || style_line_spacing_ ||
        fo_margin_top_ || fo_margin_bottom_ || fo_margin_) && Context.get_drop_cap_context().state() != 2)
    {
        std::wstring w_after, w_before;
        std::wstring w_line, w_lineRule;

        w_after = process_margin(fo_margin_bottom_, 20.0);
        w_before = process_margin(fo_margin_top_, 20.0);

        // TODO :   здесь 240 берется из корневого стиля? надо не константу использовать а брать оттуда
        //          в xsl преобразованиях так же написано 
        if (fo_line_height_)
        {
            if (fo_line_height_->get_type() == line_width::Percent)
            {
                w_line = boost::lexical_cast<std::wstring>( (int)( 0.5 + fo_line_height_->get_percent().get_value() * 240.0 / 100.0 ) );
                w_lineRule = L"auto";
            }
            else if(fo_line_height_->get_type() == line_width::Normal) 
            {
                w_line = L"240";
                w_lineRule = L"exact";
            }
            else if (fo_line_height_->get_type() == line_width::PositiveLength)
            {
                w_line = boost::lexical_cast<std::wstring>((int)(0.5 + 20.0 * fo_line_height_->get_positive_length().get_value_unit(length::pt)));
                w_lineRule = L"exact";
            }
        }
        else if (style_line_height_at_least_)
        {
            w_lineRule = L"atLeast";
            w_line = boost::lexical_cast<std::wstring>((int)(0.5 + 20.0 * style_line_height_at_least_->get_value_unit(length::pt)));
        } 
        else if (style_line_spacing_)
        {
            w_lineRule = L"auto";
            w_line = boost::lexical_cast<std::wstring>( (int)(0.5 + 240.0 + 20.0 * style_line_spacing_->get_value_unit(length::pt)) );
        }

        _pPr << L"<w:spacing ";
        if (!w_after.empty())
            _pPr << "w:after=\"" << w_after << "\" ";
        if (!w_before.empty())
            _pPr << "w:before=\"" << w_before << "\" ";
        if (!w_lineRule.empty())
            _pPr << "w:lineRule=\"" << w_lineRule << "\" ";
        if (!w_line.empty())
            _pPr << "w:line=\"" << w_line << "\" ";        
        
        _pPr << L" />";
    }

    if (fo_margin_left_ || //? + буквица
        fo_margin_right_ || 
        (fo_text_indent_ && Context.get_drop_cap_context().state() != 1))
    {
        // TODO auto indent
        std::wstring w_left, w_right, w_hanging, w_firstLine;

        w_left = process_margin(fo_margin_left_, 20.0);
        w_right = process_margin(fo_margin_right_, 20.0);
        w_firstLine = process_margin(fo_text_indent_, 20.0);

        if (w_left.empty())			w_left = L"0";
        if (w_right.empty())		w_right = L"0";
        if (w_firstLine.empty())	w_hanging = L"0";
                
        _pPr << L"<w:ind ";

        _pPr << L"w:left=\"" << w_left << L"\" ";
        _pPr << L"w:right=\"" << w_right << L"\" ";
        
		if (Context.get_drop_cap_context().state() != 1 )//состояние сразу после добавления буквицы - не нужны ни отступы, ни висячие
		{
			if (!w_firstLine.empty())
				_pPr << L"w:firstLine=\"" << w_firstLine << L"\" ";

			if (!w_hanging.empty())
				_pPr << L"w:hanging=\"" << w_hanging << L"\" ";
		}

        _pPr << L" />";
    }

    if (style_vertical_align_ && Context.get_drop_cap_context().state() != 2)
    {
        std::wstring w_val;
        switch(style_vertical_align_->get_type())
        {
        case vertical_align::Baseline:
            w_val = L"baseline";
            break;
        case vertical_align::Top:
            w_val = L"top";
            break;
        case vertical_align::Middle:
            w_val = L"center";
            break;
        case vertical_align::Bottom:
            w_val = L"bottom";
            break;
        case vertical_align::Auto:
            w_val = L"auto";
            break;
        }

        if (!w_val.empty())
            _pPr << L"<w:textAlignment w:val=\"" << w_val << "\" />";
    }

    if (style_punctuation_wrap_)
    {
        std::wstring w_val;
        if (style_punctuation_wrap_->get_type() == punctuation_wrap::Simple)
            w_val = L"false";
        else if (style_punctuation_wrap_->get_type() == punctuation_wrap::Hanging)
            w_val = L"true";
        
        if (!w_val.empty())
            _pPr << L"<w:kinsoku w:val=\"" << w_val << "\" />";
    }

    if (fo_background_color_)
    {
        const std::wstring w_fill = (fo_background_color_->get_type() == background_color::Enabled 
            ? fo_background_color_->get_color().get_hex_value() : L"auto");
        _pPr << L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" << w_fill << L"\" />";
    }

    if (fo_break_after_ && fo_break_after_->get_type() == fo_break::Page)
    {
        if (Context.in_automatic_style())
        {
            Context.add_page_break_after();
        }
    }

    if (
        /*style_tab_stop_distance_ || */
        (style_tab_stops_ && dynamic_cast<style_tab_stops*>(style_tab_stops_.get())->size() )
        )
    {
        _pPr << L"<w:tabs>";
        /*if (style_tab_stop_distance_)
        {
            _pPr << L"<w:tab w:pos=\"" << (int)( 20.0 * style_tab_stop_distance_->get_value_unit(length::pt) ) << "\" w:val=\"left\" />";
        }*/

        if (style_tab_stops_)
            style_tab_stops_->docx_convert(Context);

        _pPr << L"</w:tabs>";
    }
}

void style_tab_stops::docx_convert(oox::docx_conversion_context & Context)
{
    std::wstringstream & _pPr = Context.get_styles_context().paragraph_style();
    
    if (style_tab_stops_.size())
    {
        BOOST_FOREACH(const office_element_ptr & elm, style_tab_stops_)
        {
            elm->docx_convert(Context);
        }
    }
}

void style_tab_stop::docx_convert(oox::docx_conversion_context & Context)
{
    std::wstringstream & _pPr = Context.get_styles_context().paragraph_style();

    _pPr << L"<w:tab ";
    
    _pPr << L"w:pos=\"" << (int)( 20.0 * style_position_.get_value_unit(length::pt) ) << "\" ";
    
    {
        std::wstring val = L"left";
        if (style_type_)
        {
            switch(style_type_->get_type())
            {
            case style_type::Left:
                val = L"left";
                break;
            case style_type::Center:
                val = L"center";
                break;
            case style_type::Right:
                val = L"right";
                break;
            case style_type::Char:
                val = L"decimal";            
                break;
            }
        }
        if (!val.empty())
            _pPr << L"w:val=\"" << val << "\" ";
    }

    {
        std::wstring leader;

        if (style_leader_type_ && style_leader_type_->get_type() == line_type::None ||
            style_leader_style_ && style_leader_style_->get_type() == line_style::None)
        {
            leader = L"none";
        }
        else if (
            !style_leader_type_ ||
            style_leader_type_ && style_leader_type_->get_type() != line_type::None
            )
        {
            if (style_leader_style_)
            {
                _CP_LOG(info) << L"[docx_convert] convert warning (w:tab/@w:leader)\n";
                switch(style_leader_style_->get_type())
                {
                case line_style::None:
                    leader = L"";
                    break;
                case line_style::Solid:
                    if (style_leader_text_)
                    {
                        if (*style_leader_text_ == L"-")
                            leader = L"hyphen";
                        else if (*style_leader_text_ == L"_")
                            leader = L"underscore";
                    }
                    break;
                case line_style::Dotted:
                    leader = L"dot";
                    break;
                case line_style::Dash:
                    leader = L"hyphen";
                    break;
                case line_style::LongDash:
                    leader = L"middleDot";
                    break;
                case line_style::DotDash:
                    leader = L"middleDot";
                    break;
                case line_style::DotDotDash:
                    leader = L"middleDot";
                    break;
                case line_style::Wave:
                    leader = L"middleDot";
                    break;
                }
            }
        }

        if (!leader.empty())
            _pPr << L"w:leader=\"" << leader << "\" ";
    }
    
    _pPr << L" />";
}

void style_paragraph_properties::docx_convert(oox::docx_conversion_context & Context)
{
    style_paragraph_properties_content_.docx_convert(Context);
}


}
}
