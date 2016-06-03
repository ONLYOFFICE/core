
#include "xlsx_fonts.h"
#include "xlsx_font.h"

#include <cpdoccore/xml/simple_xml_writer.h>

#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>

namespace cpdoccore {
namespace oox {

class xlsx_fonts::Impl
{
public:
	Impl(){}


    size_t size() const;
    size_t fontId(	const odf_reader::text_format_properties_content		* textProp,
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
            
    BOOST_FOREACH(const xlsx_font & fnt, fonts_)
    {
        fonts.push_back(fnt);
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

size_t xlsx_fonts::Impl::fontId(const odf_reader::text_format_properties_content		* textProp,
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
int xlsx_fonts::Impl::getIndex(const odf_reader::text_format_properties_content * textProp,
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

size_t xlsx_fonts::fontId(const odf_reader::text_format_properties_content * textProp,
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
