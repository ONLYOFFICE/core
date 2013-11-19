#include "precompiled_cpodf.h"
#include <boost/foreach.hpp>
#include "..\odf\style_text_properties.h"
#include "..\odf\style_paragraph_properties.h"

#include "xlsx_styles.h"
#include "xlsx_fonts.h"
#include "xlsx_xf.h"
#include "xlsx_borders.h"
#include "xlsx_fills.h"
#include "xlsx_cell_format.h"
#include "xlsx_cell_styles.h"
#include "xlsx_numFmts.h"

#include <boost/unordered_set.hpp>
#include <boost/functional.hpp>

namespace cpdoccore {
namespace oox {

class xlsx_style_manager::Impl
{
public:
    //typedef std::vector<xlsx_xf> xlsx_xf_array;
    typedef boost::unordered_set<xlsx_xf, boost::hash<xlsx_xf> > xlsx_xf_array;

public:
    Impl();
    size_t size() const;
    size_t xfId(const odf::text_format_properties_content * textProp,
        const odf::paragraph_format_properties * parProp,
        const odf::style_table_cell_properties_attlist * cellProp,
        const xlsx_cell_format * xlxsCellFormat,
        const std::wstring &num_format, bool  default_set, bool & is_visible);

    void xlsx_serialize(std::wostream & _Wostream);
    void xlsx_serialize_xf(std::wostream & _Wostream, const xlsx_xf_array & xfArray, const std::wstring & nodeName);

private:
    xlsx_fonts fonts_;
    xlsx_borders borders_;
    xlsx_fills fills_;
    xlsx_xf_array cellXfs_;

    xlsx_cell_styles cellStyles_;
    xlsx_xf_array cellStyleXfs_;
    xlsx_num_fmts numFmts_;

private:
    size_t next_index_;
    void insert(xlsx_xf const & xf)
    {
        xf.index = next_index_++;
        cellXfs_.insert(xf);
    } 
};

xlsx_style_manager::xlsx_style_manager() : impl_(new xlsx_style_manager::Impl() )
{}

size_t xlsx_style_manager::size() const
{
    return impl_->size();
}

size_t xlsx_style_manager::xfId(const odf::text_format_properties_content * textProp,
                                const odf::paragraph_format_properties * parProp,
                                const odf::style_table_cell_properties_attlist * cellProp,
                                const xlsx_cell_format * xlxsCellFormat,
                                const std::wstring &num_format, bool  default_set)
{
    bool is_visible;
    return impl_->xfId(textProp, parProp, cellProp, xlxsCellFormat, num_format,default_set, is_visible);
}

size_t xlsx_style_manager::xfId(const odf::text_format_properties_content * textProp,
                                const odf::paragraph_format_properties * parProp,
                                const odf::style_table_cell_properties_attlist * cellProp,
                                const xlsx_cell_format * xlxsCellFormat,
                                const std::wstring &num_format, bool  default_set, bool & is_visible)
{
    return impl_->xfId(textProp, parProp, cellProp, xlxsCellFormat, num_format, default_set,is_visible);
}

void xlsx_style_manager::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->xlsx_serialize(_Wostream);
}

xlsx_style_manager::~xlsx_style_manager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xlsx_style_manager::Impl::Impl() : next_index_(0)
{
    {
        xlsx_xf xfRecord;
        xfRecord.xfId = 0;
        xfRecord.borderId = 0;
        xfRecord.numFmtId = 0;
        xfRecord.fillId = 0;
        xfRecord.fontId = 0;
        insert(xfRecord);
        //cellXfs_.insert(xfRecord);
        //cellXfs_.push_back(xfRecord); // default
    }

    {
        xlsx_xf xfRecord;
        cellStyleXfs_.insert(xfRecord);
        //cellStyleXfs_.push_back(xfRecord);
    }
}

size_t xlsx_style_manager::Impl::size() const
{
    return cellXfs_.size();
}

size_t xlsx_style_manager::Impl::xfId(const odf::text_format_properties_content * textProp,
                                      const odf::paragraph_format_properties * parProp,
                                      const odf::style_table_cell_properties_attlist * cellProp,
                                      const xlsx_cell_format * xlxsCellFormat,
                                      const std::wstring &num_format, bool  default_set, bool & is_visible )
{
    const size_t fontId = fonts_.fontId(textProp, parProp, cellProp);
    is_visible = false;
    
	bool default_border = false;
    const size_t borderId = borders_.borderId(cellProp, default_border);

    bool default_fill = false;
    const size_t fillId = fills_.fillId(textProp, parProp, cellProp, default_set,default_fill);

    if (!default_border || !default_fill)
        is_visible = true;

    xlsx_alignment alignment = OdfProperties2XlsxAlignment(textProp, parProp, cellProp);
    const unsigned int id = next_index_;//static_cast<unsigned int>(cellXfs_.size());    

    xlsx_xf xfRecord;
    const bool dbgApplyAlignment = !is_default(alignment);
    xfRecord.applyAlignment = dbgApplyAlignment;
    
    xfRecord.applyBorder = true;
    xfRecord.borderId = borderId;

    xfRecord.applyFill = true; // TODO
    xfRecord.fillId = fillId;
    
    xfRecord.applyFont = true;
    xfRecord.fontId = fontId;
    
    xfRecord.applyProtection = false; // TODO

    if (!num_format.empty())
    {
        xfRecord.applyNumberForm = true;
        xfRecord.numFmtId = numFmts_.num_format_id(num_format);
    }
    else
    if (xlxsCellFormat && 
        xlxsCellFormat->get_cell_type() != XlsxCellType::null)
    {
        xfRecord.applyNumberForm = true;
        xfRecord.numFmtId = xlxsCellFormat->get_num_format();
    }

    xfRecord.xfId = 0;
    xfRecord.alignment = alignment;

    //xlsx_xf_array::const_iterator i = std::find(cellXfs_.begin(), cellXfs_.end(), xfRecord);
    xlsx_xf_array::const_iterator i = cellXfs_.find(xfRecord);
    if (i != cellXfs_.end())
    {
        const std::size_t dbgId = i->index;
        //const unsigned int dbgId = i - cellXfs_.begin();
        return dbgId;
    }
    else
    {
        insert(xfRecord);
        //cellXfs_.push_back(xfRecord);
    }
    
    return id;
}

void xlsx_style_manager::Impl::xlsx_serialize(std::wostream & _Wostream)
{
    _Wostream << L"<styleSheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">";

    ::cpdoccore::oox::xlsx_serialize(_Wostream, numFmts_); 
    ::cpdoccore::oox::xlsx_serialize(_Wostream, fonts_); 
    ::cpdoccore::oox::xlsx_serialize(_Wostream, fills_); 
    ::cpdoccore::oox::xlsx_serialize(_Wostream, borders_);    

    
    xlsx_serialize_xf(_Wostream, cellStyleXfs_, L"cellStyleXfs");
    xlsx_serialize_xf(_Wostream, cellXfs_, L"cellXfs");
    cellStyles_.xlsx_serialize(_Wostream);
    

    _Wostream << L"</styleSheet>";
}

namespace 
{
struct compare_xlsx_xf
{
    bool operator() (xlsx_xf const & x1, xlsx_xf const & x2)
    {
        return x1.index < x2.index;            
    }
};
}

void xlsx_style_manager::Impl::xlsx_serialize_xf(std::wostream & _Wostream, const xlsx_xf_array & xfArray, const std::wstring & nodeName)
{
    std::vector<xlsx_xf> xfs_;
            
    BOOST_FOREACH(const xlsx_xf & xfRecord, xfArray)
    {
        xfs_.push_back(xfRecord);
    }

    std::sort(xfs_.begin(), xfs_.end(), compare_xlsx_xf());

    _Wostream << L"<" << nodeName << L" count=\"" << xfs_.size() << L"\">";
    BOOST_FOREACH(const xlsx_xf & xfRecord, xfs_)
    {
        ::cpdoccore::oox::xlsx_serialize(_Wostream, xfRecord);
    }
    _Wostream << L"</" << nodeName << L">";    
}


}
}
