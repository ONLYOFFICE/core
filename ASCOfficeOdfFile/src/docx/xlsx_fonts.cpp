#include "../odf/precompiled_cpodf.h"
#include "xlsx_font.h"
#include "xlsx_fonts.h"
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>

#include "..\odf\style_text_properties.h"

namespace cpdoccore {
namespace oox {

namespace 
{
    const double kDefaultFontSize = 10.0;
}


namespace 
{

XlsxFontCharset GetXlsxFontCharset(const odf::text_format_properties_content * textProp)
{
    // TODO
    return XCHARSET_EMPTY;    
}

XlsxFontFamily GetXlsxFontFamily(const odf::text_format_properties_content * textProp)
{
    // TODO
    return XFAMILY_EMPTY;
}

xlsx_font OdfFont2XlsxFont(const odf::text_format_properties_content * textProp,
    const odf::paragraph_format_properties * parProp,
    const odf::style_table_cell_properties_attlist * cellProp)
{
    xlsx_font font;
    if (!textProp)
        return font;

    if (textProp->fo_font_weight_)
    {
        if (textProp->fo_font_weight_.get().get_type() == odf::font_weight::WBold)
            font.bold = true;
        else
            font.bold = false;
    }

    if (textProp->fo_font_style_)
    {
        if (textProp->fo_font_style_.get().get_type() == odf::font_style::Italic)
            font.i = true;
        else
            font.i = false;
    }

    XlsxFontCharset charset = GetXlsxFontCharset(textProp);
    if (charset != XCHARSET_EMPTY)
    {
        font.charset = (unsigned int)charset;
    }

    XlsxFontFamily family = GetXlsxFontFamily(textProp);
    if (family != XFAMILY_EMPTY)
    {
        font.family = family;
    }

    if (textProp->style_font_name_)
    {
        font.name = textProp->style_font_name_.get();            
    }

    if (textProp->fo_font_size_)
    {
        font.sz = textProp->fo_font_size_->get_length().get_value_unit(odf::length::pt);
    }
    else
    {
        font.sz = kDefaultFontSize;        
    }

    if (textProp->style_text_underline_type_ &&
        textProp->style_text_underline_type_->get_type() != odf::line_type::None ||
    
        textProp->style_text_underline_style_ &&
        textProp->style_text_underline_style_->get_type() != odf::line_style::None        
        )
    {
        if (textProp->style_text_underline_type_ &&
            textProp->style_text_underline_type_->get_type() == odf::line_type::Double)
            font.u = XUNDERLINE_DOUBLE;
        else
            font.u = XUNDERLINE_SINGLE;
    }

    if (textProp->style_text_line_through_type_ &&
        textProp->style_text_line_through_type_->get_type() != odf::line_type::None ||

        textProp->style_text_line_through_style_ &&
        textProp->style_text_line_through_style_->get_type() != odf::line_style::None)
    {
        font.strike = true;
    }

    if (textProp->fo_text_shadow_)
    {
        if (textProp->fo_text_shadow_->get_type() == odf::shadow_type::Enable)
            font.shadow = true;
        else
            font.shadow = false;
    }

	bool default_color = false;
	if (textProp->style_use_window_font_color_)
		default_color = true;
   
	if (textProp->fo_color_ && !default_color)
    {
        xlsx_color color;
        // alfa + rgb
        color.rgb = L"ff" + textProp->fo_color_->get_hex_value();
		std::wstring test = color.rgb.get();
        font.color = color;

    }

    return font;
}    

} // namespace

class xlsx_fonts::Impl
{
public:
    Impl()
    {
    }

    size_t size() const;
    size_t fontId(const odf::text_format_properties_content * textProp,
        const odf::paragraph_format_properties * parProp,
        const odf::style_table_cell_properties_attlist * cellProp);
    //const xlsx_font & getFont(size_t id) const;
    void xlsx_serialize(std::wostream & _Wostream) const;

private:
    /*int getIndex(const odf::text_format_properties_content * textProp,
        const odf::paragraph_format_properties * parProp,
        const odf::style_table_cell_properties_attlist * cellProp) const;*/
    
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

void xlsx_fonts::Impl::xlsx_serialize(std::wostream & _Wostream) const
{
    std::vector<xlsx_font> fonts;
            
    BOOST_FOREACH(const xlsx_font & fnt, fonts_)
    {
        fonts.push_back(fnt);
    }

    std::sort(fonts.begin(), fonts.end(), compare_xlsx_fonts());

    _Wostream << L"<fonts count=\"" << fonts.size() << "\">";

    for (size_t i = 0; i < fonts.size(); ++i)
        ::cpdoccore::oox::xlsx_serialize(_Wostream, fonts[i]);

    _Wostream << L"</fonts>";
}

size_t xlsx_fonts::Impl::fontId(const odf::text_format_properties_content * textProp,
                          const odf::paragraph_format_properties * parProp,
                          const odf::style_table_cell_properties_attlist * cellProp)
{
    xlsx_font fnt = OdfFont2XlsxFont(textProp, parProp, cellProp);
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
int xlsx_fonts::Impl::getIndex(const odf::text_format_properties_content * textProp,
                         const odf::paragraph_format_properties * parProp,
                         const odf::style_table_cell_properties_attlist * cellProp) const
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



void xlsx_serialize(std::wostream & _Wostream, const xlsx_fonts & fonts)
{
    fonts.xlsx_serialize(_Wostream);
}

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

void xlsx_fonts::xlsx_serialize(std::wostream & _Wostream) const
{
    return impl_->xlsx_serialize(_Wostream);
}

size_t xlsx_fonts::fontId(const odf::text_format_properties_content * textProp,
    const odf::paragraph_format_properties * parProp,
    const odf::style_table_cell_properties_attlist * cellProp)
{
    return impl_->fontId(textProp, parProp, cellProp);
}

//const xlsx_font & xlsx_fonts::getFont(size_t id) const
//{
//    return impl_->getFont(id);
//}    

}
}
