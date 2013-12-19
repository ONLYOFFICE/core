#include "precompiled_cpodf.h"
#include "table.h"

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>

#include "style_table_properties.h"
#include "style_graphic_properties.h"
#include "draw_common.h"
namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

bool table_table_cell_content::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool wasPar = false;
    BOOST_FOREACH(const office_element_ptr & elm, text_content_)
    {
        if (elm->get_type() == typeTextP || 
            elm->get_type() == typeTextH)
            wasPar = true;
        elm->pptx_convert(Context);
    }
    return wasPar;
}

void table_table_row::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.get_table_context().tableData();

    const std::wstring styleName = table_table_row_attlist_.table_style_name_.get_value_or( style_ref(L"") ).style_name();
    const std::wstring defaultCellStyle = table_table_row_attlist_.table_default_cell_style_name_.get_value_or( style_ref(L"") ).style_name();

    for (unsigned int i = 0; i < table_table_row_attlist_.table_number_rows_repeated_; ++i)
    {
		int height = 0;
       
		const style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableRow,false);
	
		if ((inst) && (inst->content()) && (inst->content()->get_style_table_row_properties()))
		{
			style_table_row_properties_attlist & row_attlist = inst->content()->get_style_table_row_properties()->style_table_row_properties_attlist_;
			if (row_attlist.style_row_height_)
			{
				height = (int)( 0.5 +row_attlist.style_row_height_->get_value_unit(length::emu) );
			}
			else if (row_attlist.style_min_row_height_)
			{
				height = (int)( 0.5 + row_attlist.style_min_row_height_->get_value_unit(length::emu));
			}
		}
		if (height<1) height = 100000;
    
		_Wostream << L"<a:tr h=\"" << height << L"\">";
  
        Context.get_table_context().start_row(styleName, defaultCellStyle);
        
        BOOST_FOREACH(const office_element_ptr & elm, content_)
        {
            elm->pptx_convert(Context);
        }

        Context.get_table_context().end_row();

        _Wostream << L"</a:tr>";
    }
}

void table_table_rows::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
    {
        elm->pptx_convert(Context);
    }
}

void table_table_header_rows::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
    {
        elm->pptx_convert(Context);
    }
}

void table_rows::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (table_table_rows_)
        table_table_rows_->pptx_convert(Context);
    else
    {
        BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
        {
            elm->pptx_convert(Context);
        }
    }    
}

void table_rows_no_group::pptx_convert(oox::pptx_conversion_context & Context)
{
    table_rows_1_.pptx_convert(Context);
    
    if (table_table_header_rows_)
        table_table_header_rows_->pptx_convert(Context);

    table_rows_2_.pptx_convert(Context);    
}

void table_rows_and_groups::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->pptx_convert(Context);
    }
}

void table_table::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_table();
//////////////////////////////////////////////////////////////////

	std::wstring tableStyleName = L"";
    if (table_table_attlist_.table_style_name_)
        tableStyleName = table_table_attlist_.table_style_name_->style_name() ;

    Context.get_table_context().start_table(tableStyleName);
	
    std::wostream & _Wostream = Context.get_table_context().tableData();  
	_Wostream << L"<a:tbl>";

 	_Wostream << L"<a:tblPr";
		if (table_table_attlist_.table_use_banding_rows_styles_)
			_Wostream << L"	bandRow=\"1\"";
		if (table_table_attlist_.table_use_first_row_styles_)
			_Wostream << L"	firstRow=\"1\"";
		if (table_table_attlist_.table_use_banding_columns_styles_)
			_Wostream << L"	bandCol=\"1\"";
		if (table_table_attlist_.table_use_first_column_styles_)
			_Wostream << L"	firstCol=\"1\"";		
	_Wostream << ">";
	
	style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name( tableStyleName , style_family::Table,false);

    if ((inst) && (inst->content()))
	{
		//table_properties
		if (inst->content()->get_style_graphic_properties())
		{
			const graphic_format_properties & style_graphic = inst->content()->get_style_graphic_properties()->content();
			oox::_oox_fill fill;
			Compute_GraphicFill((graphic_format_properties &)style_graphic, Context.root()->odf_context().drawStyles() ,fill);	

			oox::oox_serialize_fill(_Wostream, fill);
		}
	}
 	_Wostream << L"</a:tblPr>";
    

    _Wostream << L"<a:tblGrid>";
		table_columns_and_groups_.pptx_convert(Context);
    _Wostream << L"</a:tblGrid>";

    table_rows_and_groups_.pptx_convert(Context);

    Context.get_table_context().end_table();
    _Wostream << L"</a:tbl>";

////////////////////////////////////////////////////////////////////////	
	std::wstring table_content_ = Context.get_table_context().tableData().str();

	if (table_content_.length()>0)
	{
		Context.get_slide_context().set_property(_property(L"table-content",table_content_));
	}
    Context.get_slide_context().end_table();    
}

void table_columns::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (table_table_columns_)
        table_table_columns_->pptx_convert(Context);

    BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
    {
        elm->pptx_convert(Context);
    }
}

void table_table_columns::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
    {
        elm->pptx_convert(Context);
    }
}

void table_columns_no_group::pptx_convert(oox::pptx_conversion_context & Context)
{
    table_columns_1_.pptx_convert(Context);
    
    if (table_table_header_columns_)
        table_table_header_columns_->pptx_convert(Context);

    table_columns_2_.pptx_convert(Context);
}

void table_columns_and_groups::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->pptx_convert(Context);
    }
}

void table_table_header_columns::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
    {
        elm->pptx_convert(Context);
    }    
}

void table_table_column_group::pptx_convert(oox::pptx_conversion_context & Context)
{
    table_columns_and_groups_.pptx_convert(Context);
}

void table_table_column::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.get_table_context().tableData();

    const unsigned int columnsRepeated = table_table_column_attlist_.table_number_columns_repeated_;
    const std::wstring defaultCellStyle = table_table_column_attlist_.table_default_cell_style_name_.get_value_or(style_ref(L"")).style_name();
   
	Context.get_table_context().start_column(columnsRepeated, defaultCellStyle);

    for (unsigned int i = 0; i < columnsRepeated; ++i)
    {
        if (table_table_column_attlist_.table_style_name_)
        {
            const std::wstring colStyleName = table_table_column_attlist_.table_style_name_->style_name();
          
			style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name( colStyleName , style_family::TableColumn,false );
			if ((inst) && (inst->content()))
            {
				//column properies
				if (inst->content()->get_style_table_column_properties())
				   inst->content()->get_style_table_column_properties()->pptx_convert(Context);				
            }
        }
    }
}

const wchar_t * emptyParTable = L"<a:txBody><a:bodyPr/><a:p><a:endParaRPr/></a:p></a:txBody>";

void table_table_cell::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.get_table_context().tableData();

	CP_XML_WRITER(_Wostream)
    {
		for (unsigned int r = 0; r < table_table_cell_attlist_.table_number_columns_repeated_; ++r)
		{
			int pushTextPropCount = 0;

			Context.get_table_context().start_cell();
			CP_XML_NODE(L"a:tc")
			{
				const std::wstring cellStyleName = table_table_cell_attlist_.table_style_name_ ?
					table_table_cell_attlist_.table_style_name_->style_name() : L""; 
				
				const std::wstring & defaultColumnStyleName = Context.get_table_context().get_default_cell_style_col(Context.get_table_context().current_column());
				const std::wstring & defaultRowStyleName	= Context.get_table_context().get_default_cell_style_row();

				if (table_table_cell_attlist_extra_.table_number_rows_spanned_ > 1)
				{
					CP_XML_ATTR(L"rowSpan" , table_table_cell_attlist_extra_.table_number_rows_spanned_);
				//	CP_XML_ATTR(L"vMerge", true);
					
					Context.get_table_context().set_rows_spanned(Context.get_table_context().current_column(), 
						table_table_cell_attlist_extra_.table_number_rows_spanned_ - 1,
						table_table_cell_attlist_extra_.table_number_columns_spanned_ - 1,
						cellStyleName);
				}        		
			
				if (table_table_cell_attlist_extra_.table_number_columns_spanned_ > 1)
				{
					//CP_XML_ATTR(L"hMerge", true);
					CP_XML_ATTR(L"gridSpan" , table_table_cell_attlist_extra_.table_number_columns_spanned_);

					Context.get_table_context().set_columns_spanned(table_table_cell_attlist_extra_.table_number_columns_spanned_ - 1);
				}		

				Context.get_text_context().start_object();

				bool presentText = table_table_cell_content_.pptx_convert(Context);
				
				std::wstring cellContent = Context.get_text_context().end_object();

				if (cellContent.length()>0)
				{
					CP_XML_NODE(L"a:txBody")
					{
						CP_XML_NODE(L"a:bodyPr");
						CP_XML_STREAM() << cellContent;
					}
				}else
					CP_XML_STREAM() << emptyParTable;
			
				CP_XML_NODE(L"a:tcPr")
				{				
					const style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name(cellStyleName, style_family::TableCell,false);
			        
					if (inst && inst->content())
					{
						if (inst->content()->get_style_table_cell_properties())
						{
							style_table_cell_properties_attlist & style_cell_attlist = inst->content()->get_style_table_cell_properties()->style_table_cell_properties_attlist_;

							if (style_cell_attlist.style_vertical_align_)
							{
								std::wstring vAlign;
								switch(style_cell_attlist.style_vertical_align_->get_type())
								{
								case vertical_align::Baseline: 
								case vertical_align::Top:      vAlign = L"t"; break;
								case vertical_align::Middle:   vAlign = L"ctr"; break;
								case vertical_align::Bottom:   vAlign = L"b"; break;
								case vertical_align::Auto:  break;
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
						}
						if (inst->content()->get_style_graphic_properties())
						{
							const graphic_format_properties & style_graphic = inst->content()->get_style_graphic_properties()->content();
							oox::_oox_fill fill;
							Compute_GraphicFill((graphic_format_properties &)style_graphic, Context.root()->odf_context().drawStyles() ,fill);	

							oox::oox_serialize_fill(CP_XML_STREAM(), fill);
						}
					}
				}
			}

			Context.get_table_context().end_cell();
		}
	}
}

void table_covered_table_cell::pptx_convert(oox::pptx_conversion_context & Context) 
{
   
	std::wostream & _Wostream = Context.get_table_context().tableData();

    for (unsigned int i = 0; i < table_table_cell_attlist_.table_number_columns_repeated_; ++i)
    {
        if (Context.get_table_context().start_covered_cell(Context))
        {
            if (!table_table_cell_content_.pptx_convert(Context))
            {
                _Wostream << emptyParTable;
            }
        }
        Context.get_table_context().end_covered_cell();
    }
}

}
}
