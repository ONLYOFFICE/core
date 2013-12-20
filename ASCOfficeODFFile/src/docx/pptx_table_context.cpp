#include "precompiled_cpodf.h"
#include <boost/foreach.hpp>
#include <iostream>

#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/odf/odf_document.h>

#include "../odf/odfcontext.h"
#include "../odf/draw_common.h"
#include "../odf/calcs_styles.h"

#include "logging.h"

#include "pptx_conversion_context.h"

namespace cpdoccore { 
namespace oox {

/**/

pptx_table_state::pptx_table_state(pptx_conversion_context & Context,
    const std::wstring & StyleName) : context_(Context),
    table_style_(StyleName),
    current_table_column_(-1),
    columns_spanned_num_(0),
    close_table_covered_cell_(false)
{        
}

void pptx_table_state::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    columns_.push_back(repeated);

    for (unsigned int i = 0; i < repeated; ++i)
        columnsDefaultCellStyleName_.push_back(defaultCellStyleName);
}

std::wstring pptx_table_state::get_default_cell_style_col(unsigned int column)
{
    return columnsDefaultCellStyleName_.at(column);
}

std::wstring pptx_table_state::get_default_cell_style_row()
{
    return default_row_cell_style_name_;
}

void pptx_table_state::start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
{
    current_table_column_ = -1;
    columns_spanned_style_ = L"";
    close_table_covered_cell_ = false;
    table_row_style_stack_.push_back(StyleName);
    default_row_cell_style_name_ = defaultCellStyleName;
}

void pptx_table_state::end_row()
{
    table_row_style_stack_.pop_back();
    default_row_cell_style_name_ = L"";
}

std::wstring pptx_table_state::current_row_style() const
{
    if (table_row_style_stack_.size())
        return table_row_style_stack_.back();
    else
        return L"";
}

void pptx_table_state::start_cell()
{
    current_table_column_++;
    if (current_table_column_ >= (int)(rows_spanned_.size()))
        rows_spanned_.push_back(table_row_spanned());            
}

void pptx_table_state::end_cell()
{}

bool pptx_table_state::start_covered_cell(pptx_conversion_context & Context)
{
    std::wostream & _Wostream = context_.get_table_context().tableData();
    current_table_column_++;

    // обновл€ем вектор, в котором хран€тс€ информаци€ об объединении строк
    // добавл€ем в него новый столбец

    if (current_table_column_ >= (int)(rows_spanned_.size()))
        rows_spanned_.push_back(table_row_spanned());

    bool closeTag = false;
    if (columns_spanned_num_ == 0 && rows_spanned_[current_table_column_].num() > 0)
    {
        closeTag = true;
        _Wostream << L"<a:tc vMerge=\"1\"";

		if (rows_spanned_[current_table_column_].column_spanned() > 0)
            _Wostream << L" gridSpan=\"" << rows_spanned_[current_table_column_].column_spanned() + 1 << "\"";

		_Wostream << L">";
        odf::style_instance * inst = 
            context_.root()->odf_context().styleContainer().style_by_name( 
					rows_spanned_[current_table_column_].style() , odf::style_family::TableCell,false);

    }

    // использовали текущую €чейку, уменьшаем счетчики оставшихс€ объединенных €чеек
    // дл€ столбцов и строк

    if (columns_spanned_num_ > 0)
        columns_spanned_num_--;

    if (rows_spanned_[current_table_column_].num() > 0)
        rows_spanned_[current_table_column_].decrease();                                

    // устанавливаем флаг что €чейка была открыта, записан тег <w:tc>
    close_table_covered_cell_ = closeTag;
    return closeTag;
}

void pptx_table_state::end_covered_cell()
{
    std::wostream & _Wostream = context_.get_table_context().tableData();
    if (close_table_covered_cell_)
    {
		const std::wstring cellStyleName = default_row_cell_style_name_.length()>0 ? default_row_cell_style_name_ : default_cell_style_name_; 
		
		const odf::style_instance * style_inst = context_.root()->odf_context().styleContainer().style_by_name(cellStyleName, odf::style_family::TableCell,false);

		oox::oox_serialize_tcPr(_Wostream, style_inst, context_);

        // закрываем открытую €чейку
        _Wostream << L"</a:tc>";
        close_table_covered_cell_ = false;
    }
}

int pptx_table_state::current_column() const
{
    return current_table_column_;
}

void pptx_table_state::set_columns_spanned(unsigned int Val)
{
    if ( current_columns_spaned() > 0 )
    {

    }

    columns_spanned_num_ = Val;
}

unsigned int pptx_table_state::current_columns_spaned() const
{
    return columns_spanned_num_;
}

void pptx_table_state::set_rows_spanned(unsigned int Column, unsigned int Val, unsigned int ColumnsSpanned, const std::wstring & Style)
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG(info) << L"[warning] set_rows_spanned error\n";
    }
    else
    {
        rows_spanned_[Column].num(Val);
        rows_spanned_[Column].set_style(Style);
        rows_spanned_[Column].column_spanned(ColumnsSpanned);
    }        
}

unsigned int pptx_table_state::current_rows_spanned(unsigned int Column) const
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG(info) << L"[warning] current_rows_spanned error\n";        
        return 0;
    }
    else
    {
        return rows_spanned_[Column].num();
    }
}
void oox_serialize_tcPr(std::wostream & strm, const odf::style_instance* style_inst, oox::pptx_conversion_context & Context)
{
	const odf::style_instance * default_style_inst = Context.root()->odf_context().styleContainer().style_default_by_type(odf::style_family::TableCell);

	std::vector<const odf::style_instance *> instances;

	if (default_style_inst)	instances.push_back(default_style_inst);
	if (style_inst)			instances.push_back(style_inst);
	
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"a:tcPr")
		{				
			if (style_inst || default_style_inst)
			{
				odf::style_table_cell_properties_attlist style_cell_attlist = odf::calc_table_cell_properties(instances);

				if (style_cell_attlist.style_vertical_align_)
				{
					std::wstring vAlign;
					switch(style_cell_attlist.style_vertical_align_->get_type())
					{
					case odf::vertical_align::Baseline: 
					case odf::vertical_align::Top:      vAlign = L"t"; break;
					case odf::vertical_align::Middle:   vAlign = L"ctr"; break;
					case odf::vertical_align::Bottom:   vAlign = L"b"; break;
					case odf::vertical_align::Auto:  break;
					}
					if (!vAlign.empty())
						CP_XML_ATTR(L"anchor",  vAlign );      
				}
				if (style_cell_attlist.common_padding_attlist_.fo_padding_)
				{
					CP_XML_ATTR(L"marT", *style_cell_attlist.common_padding_attlist_.fo_padding_);
					CP_XML_ATTR(L"marB", *style_cell_attlist.common_padding_attlist_.fo_padding_);
					CP_XML_ATTR(L"marL", *style_cell_attlist.common_padding_attlist_.fo_padding_);
					CP_XML_ATTR(L"marR", *style_cell_attlist.common_padding_attlist_.fo_padding_);
				}
				else
				{
					if (style_cell_attlist.common_padding_attlist_.fo_padding_top_)
						CP_XML_ATTR(L"marT", *style_cell_attlist.common_padding_attlist_.fo_padding_top_);            
					if (style_cell_attlist.common_padding_attlist_.fo_padding_bottom_)
						CP_XML_ATTR(L"marB", *style_cell_attlist.common_padding_attlist_.fo_padding_bottom_);                        
					if (style_cell_attlist.common_padding_attlist_.fo_padding_left_)
						CP_XML_ATTR(L"marL", *style_cell_attlist.common_padding_attlist_.fo_padding_left_);
					if (style_cell_attlist.common_padding_attlist_.fo_padding_right_)
						CP_XML_ATTR(L"marR", *style_cell_attlist.common_padding_attlist_.fo_padding_right_);            
				}			
				//vert //
				//style_cell_attlist.pptx_serialize(Context, CP_XML_STREAM());    //nodes        

				oox::_oox_fill fill;

				odf::graphic_format_properties style_graphic = odf::calc_graphic_properties_content(instances);
				
				odf::Compute_GraphicFill(style_graphic, Context.root()->odf_context().drawStyles() ,fill);	
				
				if (fill.bitmap)
				{
					bool isMediaInternal = false;
					std::wstring ref;
					fill.bitmap->rId = Context.get_slide_context().get_mediaitems().add_or_find(fill.bitmap->xlink_href_, oox::mediaitems::typeImage, isMediaInternal, ref);
					Context.get_slide_context().add_rels(isMediaInternal,fill.bitmap->rId, ref, oox::mediaitems::typeImage);
				}
				oox::oox_serialize_fill(CP_XML_STREAM(), fill);
			}
		}
	}
}
}
}
