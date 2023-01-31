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

#include "table.h"

#include <xml/simple_xml_writer.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include "odf_document.h"

#include "style_table_properties.h"
#include "style_graphic_properties.h"
#include "draw_common.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


bool table_table_cell_content::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool wasPar = false;
  	for (size_t i = 0; i < elements_.size(); i++)
    {
        if (elements_[i]->get_type() == typeTextP || 
            elements_[i]->get_type() == typeTextH)
            wasPar = true;
        elements_[i]->pptx_convert(Context);
    }
    return wasPar;
}

void table_table_row::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.get_table_context().tableData();

    const std::wstring styleName = attlist_.table_style_name_.get_value_or(L"");
    const std::wstring defaultCellStyle = attlist_.table_default_cell_style_name_.get_value_or(L"");

    for (unsigned int i = 0; i < attlist_.table_number_rows_repeated_; ++i)
    {
		int height = 0;
       
		const style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableRow,false);
	
		if ((inst) && (inst->content()) && (inst->content()->get_style_table_row_properties()))
		{
			style_table_row_properties_attlist & row_attlist = inst->content()->get_style_table_row_properties()->attlist_;
			if (row_attlist.style_row_height_)
			{
				height = (int)( 0.5 + row_attlist.style_row_height_->get_value_unit(length::emu) );
			}
			else if (row_attlist.style_min_row_height_)
			{
				height = (int)( 0.5 + row_attlist.style_min_row_height_->get_value_unit(length::emu));
			}
		}
		if (height < 1) height = 100000;
    
		_Wostream << L"<a:tr h=\"" << height << L"\">";
  
        Context.get_table_context().start_row(styleName, defaultCellStyle);
        
		for (size_t i = 0; i < content_.size(); i++)
        {
            content_[i]->pptx_convert(Context);
        }

        Context.get_table_context().end_row();

        _Wostream << L"</a:tr>";
    }
}

void table_table_rows::pptx_convert(oox::pptx_conversion_context & Context)
{
	for (size_t i = 0; i < table_table_row_.size(); i++)
    {
        table_table_row_[i]->pptx_convert(Context);
    }
}

void table_table_header_rows::pptx_convert(oox::pptx_conversion_context & Context)
{
	for (size_t i = 0; i < table_table_row_.size(); i++)
    {
        table_table_row_[i]->pptx_convert(Context);
    }
}

void table_rows::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (table_table_rows_)
        table_table_rows_->pptx_convert(Context);
    else
    {
		for (size_t i = 0; i < table_table_row_.size(); i++)
        {
            table_table_row_[i]->pptx_convert(Context);
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
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
}

void table_table::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_table();
//////////////////////////////////////////////////////////////////

	std::wstring tableStyleName = attlist_.table_style_name_.get_value_or(L"");
    
	Context.get_table_context().start_table(tableStyleName);	
	
	if (attlist_.table_template_name_)
	{
		std::wstring name = L"table:" + attlist_.table_template_name_.get() ;
		if (office_element_ptr style = Context.root()->odf_context().Templates().find_by_style_name(name))
		{
			if (table_table_template* template_ = dynamic_cast<table_table_template *>(style.get()))
			{	
				//first row, columns, last row, columns, odd row, columns todoooo 
				if (template_->table_body_)
				{
					table_body_template* body_ = dynamic_cast<table_body_template* >(template_->table_body_.get());
					Context.get_table_context().set_default_cell_style(body_->table_style_name_);
				}
			}
		}
	}
	
    std::wostream & _Wostream = Context.get_table_context().tableData();  
	_Wostream << L"<a:tbl>";

 	_Wostream << L"<a:tblPr";
		if (attlist_.table_use_banding_rows_styles_)
			_Wostream << L"	bandRow=\"1\"";
		if (attlist_.table_use_first_row_styles_)
			_Wostream << L"	firstRow=\"1\"";
		if (attlist_.table_use_banding_columns_styles_)
			_Wostream << L"	bandCol=\"1\"";
		if (attlist_.table_use_first_column_styles_)
			_Wostream << L"	firstCol=\"1\"";		
	_Wostream << ">";
	
	style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name( tableStyleName , style_family::Table,false);

    if ((inst) && (inst->content()))
	{
		if (inst->content()->get_graphic_properties())
		{
			oox::_oox_fill fill;
			
			graphic_format_properties * graphic_props = inst->content()->get_graphic_properties();
			if (graphic_props)
				Compute_GraphicFill(graphic_props->common_draw_fill_attlist_, graphic_props->style_background_image_, 
																				Context.root()->odf_context().drawStyles(), fill);	

			if (fill.bitmap)
			{
				bool isMediaInternal = false;
				std::wstring ref;
				fill.bitmap->rId = Context.get_slide_context().get_mediaitems()->add_or_find(fill.bitmap->xlink_href_, oox::typeImage, isMediaInternal, ref, oox::document_place);
				Context.get_slide_context().add_rels(isMediaInternal, fill.bitmap->rId, ref, oox::typeImage);
			}
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

	if (!table_content_.empty())
	{
		Context.get_slide_context().set_property(_property(L"table-content", table_content_));
	}
    Context.get_slide_context().end_table();    
}

void table_columns::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (table_table_columns_)
        table_table_columns_->pptx_convert(Context);

	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->pptx_convert(Context);
    }
}

void table_table_columns::pptx_convert(oox::pptx_conversion_context & Context)
{
	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->pptx_convert(Context);
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
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
}

void table_table_header_columns::pptx_convert(oox::pptx_conversion_context & Context)
{
	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->pptx_convert(Context);
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
    const std::wstring defaultCellStyle = table_table_column_attlist_.table_default_cell_style_name_.get_value_or(L"");
   
	Context.get_table_context().start_column(columnsRepeated, defaultCellStyle);

    for (unsigned int i = 0; i < columnsRepeated; ++i)
    {
        if (table_table_column_attlist_.table_style_name_)
        {
            const std::wstring colStyleName = table_table_column_attlist_.table_style_name_.get();
          
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

const wchar_t * emptyParTable = L"<a:txBody><a:bodyPr/><a:p></a:p></a:txBody>";//<a:endParaRPr/>

void table_table_cell::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.get_table_context().tableData();

	CP_XML_WRITER(_Wostream)
    {
		for (unsigned int r = 0; r < attlist_.table_number_columns_repeated_; ++r)
		{
			Context.get_table_context().start_cell();
			CP_XML_NODE(L"a:tc")
			{
				std::vector<const style_instance *> style_instances;

				std::wstring	style_name;
				style_instance *style_inst = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::TableCell);
				if (style_inst) style_instances.push_back(style_inst);

				style_name = Context.get_table_context().get_default_cell_style();
				if (!style_name.empty())
				{
					style_inst = Context.root()->odf_context().styleContainer().style_by_name(style_name, style_family::TableCell,false);
					if (style_inst) style_instances.push_back(style_inst);
				}
				style_name = Context.get_table_context().get_default_cell_style_col(Context.get_table_context().current_column());
				if (!style_name.empty())
				{
					style_inst = Context.root()->odf_context().styleContainer().style_by_name(style_name, style_family::TableCell,false);
					if (style_inst)style_instances.push_back(style_inst);
				}
				style_name = Context.get_table_context().get_default_cell_style_row();
				if (!style_name.empty())
				{
					style_inst = Context.root()->odf_context().styleContainer().style_by_name(style_name, style_family::TableCell,false);
					if (style_inst) style_instances.push_back(style_inst);
				}
				style_name = attlist_.table_style_name_.get_value_or(L"");
				if (!style_name.empty())
				{
					style_inst = Context.root()->odf_context().styleContainer().style_by_name(style_name, style_family::TableCell,false);
					if (style_inst) style_instances.push_back(style_inst);
				}

				if (attlist_extra_.table_number_rows_spanned_ > 1)
				{
					CP_XML_ATTR(L"rowSpan" , attlist_extra_.table_number_rows_spanned_);
					CP_XML_ATTR(L"vMerge", 1);
					
					Context.get_table_context().set_rows_spanned(Context.get_table_context().current_column(), 
						attlist_extra_.table_number_rows_spanned_ - 1,
						attlist_extra_.table_number_columns_spanned_ - 1,
						style_name);
				}        		
			
				if (attlist_extra_.table_number_columns_spanned_ > 1)
				{
					//CP_XML_ATTR(L"hMerge", true);
					CP_XML_ATTR(L"gridSpan" , attlist_extra_.table_number_columns_spanned_);

					Context.get_table_context().set_columns_spanned(attlist_extra_.table_number_columns_spanned_ - 1);
				}		

				Context.get_text_context().start_object();

				bool presentText = content_.pptx_convert(Context);
				
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
			
				oox_serialize_tcPr(CP_XML_STREAM(), style_instances, Context);

			}

			Context.get_table_context().end_cell();
		}
	}
}

void table_covered_table_cell::pptx_convert(oox::pptx_conversion_context & Context) 
{   
	std::wostream & _Wostream = Context.get_table_context().tableData();

    for (unsigned int i = 0; i < attlist_.table_number_columns_repeated_; ++i)
    {
        if (Context.get_table_context().start_covered_cell(Context))
        {
            if (!content_.pptx_convert(Context))
            {
                _Wostream << emptyParTable;
            }
        }
        Context.get_table_context().end_covered_cell();
    }
}

}
}
