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

#include <xml/utils.h>
#include "odf_document.h"
#include <xml/simple_xml_writer.h>

#include <boost/lexical_cast.hpp>

#include "serialize_elements.h"
#include "odfcontext.h"
#include "number_style.h"
#include "calcs_styles.h"
#include "search_table_cell.h"

#include "../../Reader/Converter/xlsx_utils.h"
#include "../../Reader/Converter/xlsx_cell_format.h"
#include "../../Formulas/formulasconvert.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

static formulasconvert::odf2oox_converter formulas_converter;


int table_table_cell_content::xlsx_convert(oox::xlsx_conversion_context & Context, text_format_properties_content_ptr text_properties, bool need_cache)
{
	if (elements_.empty()) return -1;

    Context.get_table_context().start_cell_content();
	Context.get_text_context().set_cell_text_properties(text_properties);
    
	for (size_t i = 0 ; i < elements_.size(); i++)
    {
        elements_[i]->xlsx_convert(Context);
	}
   
	const int sharedStrId = Context.get_table_context().end_cell_content(need_cache);

    return sharedStrId;
}

void table_table_row::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	bool bEndTable = Context.get_table_context().state()->get_end_table();
	
	if (attlist_.table_number_rows_repeated_ > 1 && empty())
	{
		Context.get_table_context().state()->add_empty_row(attlist_.table_number_rows_repeated_);
		return;
	}
	if (attlist_.table_number_rows_repeated_ > 0x0f00 && empty_content_cells(false) || bEndTable)//0xf000 - conv_KDZO3J3xLIbZ5fC0HR0__xlsx.ods
	{
		Context.get_table_context().state()->set_end_table();
		Context.get_table_context().state()->add_empty_row(attlist_.table_number_rows_repeated_);
		return;		//conv_hSX8n3lVbhALjt0aafg__xlsx.ods, conv_MA2CauoNfX_7ejKS5eg__xlsx.ods
	}

    std::wostream & strm = Context.current_sheet().sheetData();
///обработка чтилей для роу -
	size_t Default_Cell_style_in_row_ = 0; 

    const std::wstring rowStyleName			= attlist_.table_style_name_.get_value_or(L"");
    const std::wstring defaultCellStyleName = attlist_.table_default_cell_style_name_.get_value_or( L"");

	style_instance * instStyle_CellDefault = 
				Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyleName, style_family::TableCell, false/*false*/);
	
	style_table_cell_properties * prop_CellDefault = NULL;
	
	if ((instStyle_CellDefault) && (instStyle_CellDefault->content())) 
		prop_CellDefault = instStyle_CellDefault->content()->get_style_table_cell_properties();
//кастомные настройки стиля ячеек в данном роу

	if (prop_CellDefault) //проверим что есть вообще кастом для роу- а потом уже посчитаем стиль
	{
		odf_reader::style_table_cell_properties_attlist	cellFormatProperties = calc_table_cell_properties(instStyle_CellDefault);
		Default_Cell_style_in_row_ = Context.get_style_manager().xfId(NULL,NULL, &cellFormatProperties, NULL, L"", 0, true);	
	}
	else //стиля ячеек для строки нет глянем что там внутри строки в последней ячейке
	{
		//возьмем стиль из последнего cell если он повторяющийся (тогда и ячейки вхолостую ненадо писать)
		int ind_last_cell = content_.size()-1;
		table_table_cell *last_cell = NULL;
		if (ind_last_cell > 0)
			last_cell = dynamic_cast<table_table_cell *>(content_[ind_last_cell].get());

	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool skip_next_row = false;

    std::wstring ht		= L"";
    double row_height	= 0.0;

	bool bBreakAfter = false, bBreakBefore = false;

    odf_read_context & odfContext = Context.root()->odf_context();

    odf_reader::style_instance * rowStyle = odfContext.styleContainer().style_by_name(rowStyleName, odf_types::style_family::TableRow,false/*false*/);
    if ((rowStyle) && (rowStyle->content()))
	{
		const odf_reader::style_table_row_properties * prop = rowStyle->content()->get_style_table_row_properties();
		if ((prop) && (prop->attlist_.style_row_height_))
		{
			row_height = prop->attlist_.style_row_height_->get_value_unit(odf_types::length::pt);

			if ((prop->attlist_.style_use_optimal_row_height_) && 
						(*prop->attlist_.style_use_optimal_row_height_==true))
			{
				//автоматическая подстройка высоты.
				//нету в оох
				//todooo высилить по текущему шрифту размер у (двойной) и сравнить с заданным - перебить !!!
			}

			std::wstringstream ht_s;
			ht_s.precision(3);
			ht_s << std::fixed << row_height;
			ht = ht_s.str();    
		}
		if ((prop->attlist_.common_break_attlist_.fo_break_before_) && 
			(prop->attlist_.common_break_attlist_.fo_break_before_->get_type() == odf_types::fo_break::Page))
		{
			bBreakBefore = true;
		}
		else if ((prop) && ((prop->attlist_.common_break_attlist_.fo_break_after_) && 
			(prop->attlist_.common_break_attlist_.fo_break_after_->get_type() == odf_types::fo_break::Page)))
		{
			bBreakAfter = true;
		}
	}
	int row_current = Context.current_table_row() + 1;

    bool hidden = attlist_.table_visibility_.get_type() == table_visibility::Collapse;

    for (unsigned int i = 0; i < attlist_.table_number_rows_repeated_; ++i)
    {
        Context.get_table_context().state()->start_row(rowStyleName, defaultCellStyleName);
		
		if (bBreakBefore)	Context.get_table_context().state()->set_row_break_before();
		if (bBreakAfter)	Context.get_table_context().state()->set_row_break_after();

        if (!skip_next_row)
        {
           CP_XML_WRITER(strm)
            {
                CP_XML_NODE(L"row")
                {
                    CP_XML_ATTR(L"r", Context.current_table_row() + 1);

					if (false == Context.get_table_context().state()->group_rows_.empty())
					{
						//std::wstring str_spans = std::to_wstring(Context.get_table_context().state()->group_row_.count);
						//str_spans = str_spans + L":";
						std::wstring str_spans = L"1:" + std::to_wstring(Context.get_table_context().columns_count());
						ht = L"";

						CP_XML_ATTR(L"collapsed", Context.get_table_context().state()->group_rows_.back());
						CP_XML_ATTR(L"outlineLevel", Context.get_table_context().state()->group_rows_.size());
						CP_XML_ATTR(L"spans", str_spans);						
						
						if (Context.get_table_context().state()->group_rows_.back()) hidden = false;
					}					

                    if (hidden)
                    {
                        CP_XML_ATTR(L"hidden", 1);                        
                    }

                    if (!ht.empty())
                    {
                        CP_XML_ATTR(L"ht", ht);                                            
                        CP_XML_ATTR(L"customHeight", 1);
                    }
					if (Default_Cell_style_in_row_ > 0)
					{
						CP_XML_ATTR(L"customFormat", 1);
						CP_XML_ATTR(L"s", Default_Cell_style_in_row_ );
					}


                    CP_XML_STREAM();

					for (size_t i = 0 ; i < content_.size(); i++)
                    {
						office_element_ptr & elm = content_[i];

						if (i  == content_.size() - 1) //mark last cell in row (for skip empty styled)
						{
							table_table_cell			*cell			= dynamic_cast<table_table_cell*>		(elm.get());
							table_covered_table_cell	*covered_cell	= dynamic_cast<table_covered_table_cell*>(elm.get());
							
							if (cell)				cell->last_cell_			= true;
							else if (covered_cell)	covered_cell->last_cell_	= true;
						}
						elm->xlsx_convert(Context);
                    }              

                }
            }
        }
        Context.get_table_context().state()->end_row();        

		if (Context.get_table_context().state()->is_empty_row() && Context.get_table_context().state()->group_rows_.empty())
		{
            skip_next_row = true;  
			if (attlist_.table_number_rows_repeated_ > 0xf000)
				break;//Уведомление_о_вручении.ods (1 лист)
		}
		if (content_.size() > 0 && attlist_.table_number_rows_repeated_ > 1000)
		{
			table_table_cell * table_cell = dynamic_cast<table_table_cell *>(content_[0].get());
			if ((table_cell) && (table_cell->attlist_.table_number_columns_repeated_ > 1000))
			{
				break;//Уведомление_о_вручении.ods  (2 лист)
			}
		}
    }

    Context.get_table_metrics().add_rows(attlist_.table_number_rows_repeated_, !hidden ? row_height : 0.0);

}

void table_table_rows::xlsx_convert(oox::xlsx_conversion_context & Context)
{
   	for (size_t i = 0; i < table_table_row_.size(); i++)
	{
        table_table_row_[i]->xlsx_convert(Context);
    }
}

void table_table_header_rows::xlsx_convert(oox::xlsx_conversion_context & Context)
{
   	for (size_t i = 0; i < table_table_row_.size(); i++)
	{
        table_table_row_[i]->xlsx_convert(Context);
    }
}

void table_rows::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    if (table_table_rows_)
        table_table_rows_->xlsx_convert(Context);
    else
    {
		while (table_table_row_.size() > 1)
		{
			table_table_row* row_last	= dynamic_cast<table_table_row*>(table_table_row_[table_table_row_.size() - 1].get());
			table_table_row* row_last_1	= dynamic_cast<table_table_row*>(table_table_row_[table_table_row_.size() - 2].get());

			std::wstring style		= row_last->attlist_.table_style_name_.get_value_or(L"");
			std::wstring style_1	= row_last->attlist_.table_style_name_.get_value_or(L"");
				
			if (style != style_1)break;			
			if (row_last_1->empty_content_cells() == false) break;				
			if (row_last->empty_content_cells() == false) break;

			row_last_1->attlist_.table_number_rows_repeated_ += row_last->attlist_.table_number_rows_repeated_;
			table_table_row_.pop_back();
		}
   		for (size_t i = 0; i < table_table_row_.size(); i++)
        {
            table_table_row_[i]->xlsx_convert(Context);
        }
    }    
}

void table_rows_no_group::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    table_rows_1_.xlsx_convert(Context);
    
    if (table_table_header_rows_)
        table_table_header_rows_->xlsx_convert(Context);

    table_rows_2_.xlsx_convert(Context);    
}

void table_rows_and_groups::xlsx_convert(oox::xlsx_conversion_context & Context)
{
 	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
}

void table_table_row_group::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_table_context().state()->start_table_row_group(attlist_.table_display_);
		table_rows_and_groups_.xlsx_convert(Context);
	Context.get_table_context().state()->end_table_row_group();
}

void table_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    const std::wstring tableStyleName	= attlist_.table_style_name_.get_value_or(L"");
    const std::wstring tableName		= attlist_.table_name_.get_value_or(L"");

    _CP_LOG << L"[info][xlsx] process table \"" << tableName << L"\"\n" << std::endl;

	if (office_forms_)
	{
		office_forms_->xlsx_convert(Context);
	}

	if (table_table_source_)
	{
		table_table_source* table_source = dynamic_cast<table_table_source*>( table_table_source_.get() );
		if ( table_source)
		{
			if (table_source->table_linked_source_attlist_.xlink_attlist_.href_)return;
		} 

	}
    Context.start_table(tableName, tableStyleName);

	if (attlist_.table_protected_)
	{
		Context.get_table_context().set_protection(true, attlist_.table_protection_key_.get_value_or(L""), 
															attlist_.table_protection_key_digest_algorithm_.get_value_or(L""));
		table_table_protection* prot = dynamic_cast<table_table_protection*>( table_protection_.get() );
		if (prot)
		{
			if (prot->select_protected_cells)
				Context.get_table_context().state()->set_protection_select_protected_cells(*prot->select_protected_cells);
			if (prot->select_unprotected_cells)
				Context.get_table_context().state()->set_protection_select_unprotected_cells(*prot->select_unprotected_cells);

			if (prot->insert_columns)
				Context.get_table_context().state()->set_protection_insert_columns(*prot->insert_columns);
			if (prot->insert_rows)
				Context.get_table_context().state()->set_protection_insert_rows(*prot->insert_rows);
			if (prot->delete_columns)
				Context.get_table_context().state()->set_protection_delete_columns(*prot->delete_columns);
			if (prot->delete_rows)
				Context.get_table_context().state()->set_protection_delete_rows(*prot->delete_rows);
		}
	}

	table_columns_and_groups_.xlsx_convert(Context);

// check last rows for equal style and empties - collapsed

//<table:table-row table:style-name="ro3" table:number-rows-repeated="65353">
// <table:table-cell table:style-name="ce14" table:number-columns-repeated="5"/>
// <table:table-cell table:number-columns-repeated="1019"/>
//</table:table-row>
//<table:table-row table:style-name="ro3" table:number-rows-repeated="983017">
// <table:table-cell table:number-columns-repeated="1024"/>
//</table:table-row>
//<table:table-row table:style-name="ro3">
// <table:table-cell table:number-columns-repeated="1024"/>
//</table:table-row>

	if (table_rows_and_groups_.content_.empty() == false && table_rows_and_groups_.content_.back()->get_type() == typeTableTableRowNoGroup)
	{
		table_rows_no_group * rows = dynamic_cast<table_rows_no_group*>(table_rows_and_groups_.content_.back().get());
		rows->table_rows_1_.remove_equals_empty();
		rows->table_rows_2_.remove_equals_empty();
	}

    table_rows_and_groups_.xlsx_convert(Context);

    if (table_shapes_)
        table_shapes_->xlsx_convert(Context);

 	if (conditional_formats_)
		conditional_formats_->xlsx_convert(Context);

	for (size_t i = 0 ; i < table_named_.size(); i++)
	{
		table_named_[i]->xlsx_convert(Context);
	}

	Context.end_table();
}

void table_columns::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    if (table_table_columns_)
        table_table_columns_->xlsx_convert(Context);

  	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->xlsx_convert(Context);
    }
}

void table_columns_no_group::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    table_columns_1_.xlsx_convert(Context);
    
    if (table_table_header_columns_)
        table_table_header_columns_->xlsx_convert(Context);

    table_columns_2_.xlsx_convert(Context);
}

void table_columns_and_groups::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
  	for (size_t i = 0; i < content_.size(); i++)
    {
		office_element_ptr & elm = content_[i];
        elm->xlsx_convert(Context);
    }
}

void table_table_header_columns::xlsx_convert(oox::xlsx_conversion_context & Context)
{
  	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->xlsx_convert(Context);
    }    
}

void table_table_column_group::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	Context.get_table_context().state()->start_table_column_group(attlist_.table_display_);
	table_columns_and_groups_.xlsx_convert(Context);
	Context.get_table_context().state()->end_table_column_group();
}

namespace {
// see ECMA-376 page 1768

double pixToSize(double pixels, double maxDigitSize)
{ 
	if (pixels < 8) pixels = 8; //УВЕДОМЛЕНИЕ О ПРИБЫТИИ ИНОСТРАННОГО ГРАЖДАНИНА.ods

	return (int(( pixels /*/ 0.75*/ - 5)/ maxDigitSize * 100. + 0.5)) /100. * 0.9; // * 9525. * 72.0 / (360000.0 * 2.54);
}

}

void table_table_column::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wostream & strm = Context.current_sheet().cols();

    const unsigned int columnsRepeated		= table_table_column_attlist_.table_number_columns_repeated_;
    const std::wstring styleName			= table_table_column_attlist_.table_style_name_.get_value_or(L"");
    const std::wstring defaultCellStyleName = table_table_column_attlist_.table_default_cell_style_name_.get_value_or(L"");


    int cMin = 0, cMax = 0;
    Context.start_table_column(columnsRepeated, defaultCellStyleName, cMin, cMax);

	if ( cMin > 16384 ) return;
	if ( cMax > 16384 ) return;

    double pt_width = 0.0;
	double cm_width = 0.0;  
	double in_width = 0.0;  
    
    CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"col")
        {
            bool hidden = table_table_column_attlist_.table_visibility_.get_type() == table_visibility::Collapse;

			if (false == Context.get_table_context().state()->group_columns_.empty())
			{
				CP_XML_ATTR(L"outlineLevel", Context.get_table_context().state()->group_columns_.size());
				CP_XML_ATTR(L"collapsed", Context.get_table_context().state()->group_columns_.back());

				if (Context.get_table_context().state()->group_columns_.back()) hidden = false;
			}
			if (hidden)
            {
                CP_XML_ATTR(L"hidden", L"true");
            }

			CP_XML_ATTR(L"max", cMax);
			CP_XML_ATTR(L"min", (cMin + 1));

            if (table_table_column_attlist_.table_style_name_)
            {

				if (style_instance * inst = 
					Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyleName, style_family::TableCell, false/*false*/))
				{
                    if (inst->content())
                    {
						if (const style_table_cell_properties * prop = inst->content()->get_style_table_cell_properties())
						{//сделать проверку чтоб сюда не попал дефолтный, то  сть пустой стиль
							odf_reader::style_table_cell_properties_attlist	cellFormatProperties = calc_table_cell_properties(inst);
							
							bool set_default = false;
							if (columnsRepeated > 100) set_default = true;

							size_t style_ = Context.get_style_manager().xfId(NULL,NULL, &cellFormatProperties, NULL, L"", 0, set_default);	

							//if (set_default)
								CP_XML_ATTR(L"style", style_ );
						}
					}
				}
				_CP_OPT(double) width;
                const std::wstring colStyleName = table_table_column_attlist_.table_style_name_.get_value_or(L"");
                if (style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name(colStyleName, style_family::TableColumn, false))
                {
                    if (inst->content())
                    {
                        if (const style_table_column_properties * prop = inst->content()->get_style_table_column_properties())
                        {  
                            if (prop->attlist_.style_column_width_)
                            {
                                pt_width = prop->attlist_.style_column_width_->get_value_unit(length::pt);                        
                                cm_width = prop->attlist_.style_column_width_->get_value_unit(length::cm);        
								in_width = prop->attlist_.style_column_width_->get_value_unit(length::inch);

                                if (hidden)
                                {
                                    in_width = 0.0;
                                }
								
                                const double pixDpi = in_width * 96.;                
                                width = pixToSize(pixDpi, Context.getMaxDigitSize().first); 

								// see ECMA-376 page 1768
                                if (in_width > 0)
									CP_XML_ATTR(L"width", *width);

                                CP_XML_ATTR(L"customWidth", true);
                                Context.table_column_last_width(*width);
                            }
							if ((prop->attlist_.common_break_attlist_.fo_break_before_) && 
								(prop->attlist_.common_break_attlist_.fo_break_before_->get_type() == odf_types::fo_break::Page))
							{
								Context.get_table_context().state()->set_column_break_before();
							}
							else if ((prop->attlist_.common_break_attlist_.fo_break_after_) && 
								(prop->attlist_.common_break_attlist_.fo_break_after_->get_type() == odf_types::fo_break::Page))
							{
								Context.get_table_context().state()->set_column_break_after();
							}
                        }
                    }                
                }
				if (!width)//??? default set
				{
					width = 11.52;
                    CP_XML_ATTR(L"width", *width);
                    CP_XML_ATTR(L"customWidth", false);
                    Context.table_column_last_width(*width);
				}
			}

        } // col
    } // CP_XML_WRITER 

    
	Context.get_table_metrics().add_cols(cMax - cMin, pt_width);
	Context.end_table_column();
}

namespace {

std::wstring SearchCellDataStyle(oox::xlsx_conversion_context & Context, std::wstring const & styleName)
{
    odf_read_context & odfContext = Context.root()->odf_context();
    
    std::wstring styleStr = styleName;
    
    for (;;)
    {
        style_instance * styleInstance = 
			odfContext.styleContainer().style_by_name(styleStr, style_family::TableCell,false);
       
		if (!styleInstance->data_style_name().empty())
            return styleInstance->data_style_name();   

        if (!styleInstance->parent_name().empty())
            styleStr = styleInstance->parent_name();
        else
            break;
    }
    return L"";
}

std::wstring SearchCellDataStyle(oox::xlsx_conversion_context & Context, style_instance * styleInstance)
{
    if (!styleInstance)
        return L"";

    odf_read_context & odfContext = Context.root()->odf_context();

    for (;;)
    {
        if (!styleInstance->data_style_name().empty())
            return styleInstance->data_style_name();   

        if (styleInstance->parent())
            styleInstance = styleInstance->parent();
        else
            break;
    }
    return L"";
}

std::wstring CalcCellDataStyle(oox::xlsx_conversion_context & Context,
                               std::wstring const & defaultColumnCellStyleName,
                               std::wstring const & defaultRowCellStyleName,
                               std::wstring const & cellStyleName
                               )
{
    odf_read_context & odfContext = Context.root()->odf_context();    
    
	style_instance * defaultCellStyle		= odfContext.styleContainer().style_default_by_type(style_family::TableCell);
    style_instance * defaultColumnCellStyle = odfContext.styleContainer().style_by_name	(defaultColumnCellStyleName, style_family::TableCell,false);
    style_instance * defaultRowCellStyle	= odfContext.styleContainer().style_by_name(defaultRowCellStyleName, style_family::TableCell,false);
    style_instance * cellStyle				= odfContext.styleContainer().style_by_name(cellStyleName, style_family::TableCell,false);

    std::wstring data_style = L"";
    if (defaultCellStyle)
        data_style = SearchCellDataStyle(Context, defaultCellStyle);

    {
        std::wstring ds1 = SearchCellDataStyle(Context, defaultColumnCellStyle);
        if (!ds1.empty())
            data_style = ds1;
    }

    {
        std::wstring ds1 = SearchCellDataStyle(Context, defaultRowCellStyle);
        if (!ds1.empty())
            data_style = ds1;
    }

    {
        std::wstring ds1 = SearchCellDataStyle(Context, cellStyle);
        if (!ds1.empty())
            data_style = ds1;    
    }
    return data_style;
}

}

void table_table_cell::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wostream & strm = Context.current_sheet().sheetData();
    
	const common_value_and_type_attlist & attr	= attlist_.common_value_and_type_attlist_;
	std::wstring formula = attlist_.table_formula_.get_value_or(L"");
   
// вычислить стиль для ячейки

    std::wstring cellStyleName		= attlist_.table_style_name_.get_value_or(L"");
	std::wstring columnStyleName	= Context.get_table_context().default_column_cell_style();
	std::wstring rowStyleName		= Context.get_table_context().default_row_cell_style();

	if (attlist_.table_number_columns_repeated_ > 1)
		columnStyleName.clear(); // могут быть разные стили колонок Book 24.ods

	odf_read_context & odfContext = Context.root()->odf_context();   

	style_instance *defaultCellStyle = NULL, *defaultColumnCellStyle = NULL,  *defaultRowCellStyle = NULL, *cellStyle = NULL;
	try
	{
		if (is_present_hyperlink_)
		{
			defaultCellStyle	= odfContext.styleContainer().style_by_name(L"Hyperlink", style_family::TableCell, true);
		}
		if (!defaultCellStyle)
		{
			defaultCellStyle	= odfContext.styleContainer().style_default_by_type(style_family::TableCell); 
		}

		defaultColumnCellStyle	= odfContext.styleContainer().style_by_name(columnStyleName,	style_family::TableCell, false);
		defaultRowCellStyle		= odfContext.styleContainer().style_by_name(rowStyleName,		style_family::TableCell, false);        
		cellStyle				= odfContext.styleContainer().style_by_name(cellStyleName,		style_family::TableCell, false);
	}
	catch(...)
	{
        _CP_LOG << L"[error]: style wrong\n";
	}

	std::wstring data_style = CalcCellDataStyle(Context, columnStyleName, rowStyleName, cellStyleName);

    // стили не наследуются
    std::vector<const style_instance *> instances;
    instances.push_back(defaultCellStyle);

    if (defaultColumnCellStyle)
        instances.push_back(defaultColumnCellStyle);

    if (defaultRowCellStyle)
    {
        if (instances.size() > 1)
            instances[1] = defaultRowCellStyle;
        else
            instances.push_back(defaultRowCellStyle);
    }

    if (cellStyle)
    {
        if (instances.size() > 1)
            instances[1] = cellStyle;
        else
            instances.push_back(cellStyle);
    }

    text_format_properties_content_ptr	textFormatProperties	= calc_text_properties_content		(instances);          
	paragraph_format_properties			parFormatProperties		= calc_paragraph_properties_content	(instances);
    style_table_cell_properties_attlist cellFormatProperties	= calc_table_cell_properties		(instances);
//-------------------------------------------------------------------------------------------------------------------------------
	std::wstring			num_format;
	office_value_type::type num_format_type = office_value_type::Custom;

	if (false == data_style.empty())
    {
		num_format = Context.get_num_format_context().find_complex_format(data_style, num_format_type);

		if (num_format.empty())
		{
			office_element_ptr elm = odfContext.numberStyles().find_by_style_name(data_style);
			number_style_base *num_style = dynamic_cast<number_style_base*>(elm.get());

			if (num_style)
			{
				Context.get_num_format_context().start_complex_format(data_style);
				num_style->oox_convert(Context.get_num_format_context());
				Context.get_num_format_context().end_complex_format();

				num_format = Context.get_num_format_context().get_last_format();
				num_format_type = Context.get_num_format_context().type();
			}
		}
    }
//------------------------------------------------------------------------
	std::wstring			number_val;
    _CP_OPT(bool)			bool_val;
	_CP_OPT(std::wstring)	str_val;

	oox::XlsxCellType::type	xlsx_value_type = oox::XlsxCellType::s;

 	office_value_type::type	odf_value_type = office_value_type::Custom;
	if (attr.office_value_type_)
		odf_value_type = attr.office_value_type_->get_type();
	
	if ((odf_value_type == office_value_type::Float) || 
		(odf_value_type == office_value_type::Custom && attr.office_value_))// ??
    {
        xlsx_value_type = oox::XlsxCellType::n;
        number_val = attr.office_value_.get_value_or(L"");

		if (num_format_type == office_value_type::Time ||
			num_format_type == office_value_type::Date ||
			num_format_type == office_value_type::Currency)
		{//тип формата данных из стиля не соответствует формату анных ячейки
				num_format.clear();
				num_format_type = office_value_type::Custom;
		}
    }
    else if (odf_value_type == office_value_type::Percentage)
    {
        xlsx_value_type = oox::XlsxCellType::n;
        number_val	= attr.office_value_.get_value_or(L"");
		
		if (num_format_type == office_value_type::Time ||
			num_format_type == office_value_type::Date ||
			num_format_type == office_value_type::Currency)
		{//тип формата данных из стиля не соответствует формату анных ячейки
				num_format.clear();
				num_format_type = office_value_type::Percentage;
		}
    }
    else if (odf_value_type == office_value_type::Currency)
    {
        xlsx_value_type = oox::XlsxCellType::n;
        number_val	= attr.office_value_.get_value_or(L"");
    }
    else if ((odf_value_type == office_value_type::Date) || 
			 (odf_value_type == office_value_type::Custom && attr.office_date_value_))
    { 
        if (attr.office_date_value_)
        {
            int y, m, d;
			_CP_OPT(int) h, min, sec;
            if (oox::parseDateTime(attr.office_date_value_.get(), y, m, d, h, min, sec))
            {
				boost::int64_t intDate = oox::convertDate(y, m, d);
				_CP_OPT(double) dTime;
				if (h && min)
				{
					dTime = oox::convertTime(*h, *min, sec.get_value_or(0));
				}
				if (intDate > 0)
				{
					if (dTime)
					{
						number_val = XmlUtils::ToString(*dTime + intDate);
					}
					else
					{
						number_val = boost::lexical_cast<std::wstring>(intDate);
					}
					xlsx_value_type = oox::XlsxCellType::n;    

					if (num_format_type == office_value_type::Currency)
					{//тип формата данных из стиля не соответствует формату данных ячейки
						num_format.clear();
						num_format_type = office_value_type::Date;
					}
					
					if (num_format.empty()) 
						num_format = Context.get_num_format_context().get_last_date_format();
				}
				else
				{
					str_val = attr.office_date_value_.get();
				}
            }
        }
    }
    else if ((odf_value_type == office_value_type::Time) ||
			 (odf_value_type == office_value_type::Custom && attr.office_time_value_))
    {
        if (attr.office_time_value_)
        {
            const std::wstring tv = attr.office_time_value_.get();
            int h, m;
            double s;
            if (oox::parseTime(tv, h, m, s))
            {
				double dTime = oox::convertTime(h, m, s);
				if (dTime >= 0)
				{
					number_val = boost::lexical_cast<std::wstring>(dTime);
					xlsx_value_type = oox::XlsxCellType::n;              
					
					if (num_format_type == office_value_type::Currency)
					{//тип формата данных из стиля не соответствует формату анных ячейки
						num_format.clear();
						num_format_type = office_value_type::Time;
					}

					if (num_format.empty()) 
						num_format = Context.get_num_format_context().get_last_time_format();
				}
				else
				{
					str_val = tv;
				}
            }
        }
    }
    else if ((odf_value_type == office_value_type::Boolean) ||
			 (odf_value_type == office_value_type::Custom && attr.office_boolean_value_))
    {
        xlsx_value_type = oox::XlsxCellType::b;
        if (attr.office_boolean_value_)	bool_val = oox::parseBoolVal(attr.office_boolean_value_.get());
 		
		num_format.clear();
		num_format_type = office_value_type::Boolean;
   }
    else if ((odf_value_type == office_value_type::String) ||
			 (odf_value_type == office_value_type::Custom && attr.office_string_value_))
    {
        xlsx_value_type = oox::XlsxCellType::str;
		if (attr.office_string_value_)	str_val = attr.office_string_value_.get();
		
		num_format.clear();
		num_format_type = office_value_type::String;
   }
//----------------------------------------------------------------------------------------------------------------------------------
    oox::xlsx_cell_format cellFormat;
    
	cellFormat.set_cell_type(xlsx_value_type);
    cellFormat.set_num_format(oox::odf_string_to_build_in(odf_value_type));
    
	bool is_style_visible = (!cellStyleName.empty() || defaultColumnCellStyle || !num_format.empty()) ? true : false;

	bool is_data_visible = ( false == content_.elements_.empty() ||
		attlist_.table_content_validation_name_ || !formula.empty() || !number_val.empty() || str_val || bool_val );

	if (attlist_.table_number_columns_repeated_ < 199 && last_cell_)	last_cell_ = false;
	
	int cell_repeated_max = Context.current_table_column() + attlist_.table_number_columns_repeated_ + 1;

	if (cell_repeated_max >= 1024 && cellStyleName.empty() && last_cell_ && !is_data_visible)
	{//Book 24.ods
		return;
	}
    
	size_t	xfId_last_set = 0;
	if (is_style_visible)
	{
		xfId_last_set = Context.get_style_manager().xfId(textFormatProperties, &parFormatProperties, &cellFormatProperties, 
			&cellFormat, num_format, num_format_type, false, is_style_visible);
	}

	bool need_cache_convert = false;
	if (number_val.empty() && !bool_val)
	{
		if (false == formula.empty())
			xlsx_value_type = oox::XlsxCellType::str;
		else
		{
			need_cache_convert = true;
		}
	}		
//---------------------------------------------------------------------------------------------------------	
	int sharedStringId = -1;
	int	 empty_cell_count = 0;
	bool skip_next_cell	 = false;

	for (unsigned int r = 0; r < attlist_.table_number_columns_repeated_; ++r)
    {
        Context.start_table_cell (	formula,	attlist_extra_.table_number_columns_spanned_	- 1 ,
												attlist_extra_.table_number_rows_spanned_		- 1	);
		if (is_style_visible)
			Context.set_current_cell_style_id(xfId_last_set);
//---------------------------------------------------------------------------------------------------------	
		sharedStringId = content_.xlsx_convert(Context, textFormatProperties, need_cache_convert);

		if (xlsx_value_type == oox::XlsxCellType::str || xlsx_value_type == oox::XlsxCellType::inlineStr)
		{
			int index = Context.get_table_context().in_database_range();
			
			if (index >= 0)
			{
				if (sharedStringId >= 0)
				{
					std::wstringstream strm;
					content_.text_to_stream(strm);

					Context.get_table_context().set_database_range_value(index, strm.str());
				}
				else if (str_val)
				{
					Context.get_table_context().set_database_range_value(index, str_val.get());
				}
			}
		}
		if (sharedStringId >= 0)
		{
			xlsx_value_type = oox::XlsxCellType::s;//в случае текста, если он есть берем кэшированное значение
		}		
//---------------------------------------------------------------------------------------------------------			
		if (skip_next_cell) break;
	
	// пустые ячейки пропускаем.
        if ( is_data_visible || ((cellStyle || defaultColumnCellStyle) && is_style_visible))
        {
			std::wstring ref = oox::getCellAddress(Context.current_table_column(), Context.current_table_row());
			if (attlist_.table_content_validation_name_)
			{
				Context.get_dataValidations_context().activate(*attlist_.table_content_validation_name_, /*ref*/Context.current_table_column(), Context.current_table_row());
			}   
			CP_XML_WRITER(strm)
            {
                CP_XML_NODE(L"c")
                {
                    CP_XML_ATTR(L"r", ref);
					
					CP_XML_ATTR(L"s", xfId_last_set);

					std::wstring type = oox::cellType2Str(xlsx_value_type);
                    if (false == type.empty()) CP_XML_ATTR(L"t", type);                    

                    if (!formula.empty())
                    {
                        const std::wstring xlsxFormula = formulas_converter.convert(formula);
                        if (!xlsxFormula.empty())
                        {
                            CP_XML_NODE(L"f")
                            {
								if (attlist_extra_.table_number_matrix_columns_spanned_ && attlist_extra_.table_number_matrix_rows_spanned_)
								{
									std::wstring ref = oox::getCellAddress(Context.current_table_column(), Context.current_table_row());
									ref += L":";
									ref += oox::getCellAddress(Context.current_table_column() + *attlist_extra_.table_number_matrix_columns_spanned_ - 1,
										 Context.current_table_row() + *attlist_extra_.table_number_matrix_rows_spanned_ - 1);
									CP_XML_ATTR(L"ref", ref);
									CP_XML_ATTR(L"t", L"array");
									CP_XML_ATTR(L"aca", false);
								}
                                CP_XML_CONTENT(XmlUtils::EncodeXmlString(xlsxFormula));
                            }
                        }
                    }

					if (sharedStringId >= 0 && xlsx_value_type == oox::XlsxCellType::s)
                    {
                        CP_XML_NODE(L"v")	{ CP_XML_CONTENT(sharedStringId); }
                    }
					else if ((xlsx_value_type == oox::XlsxCellType::str || xlsx_value_type == oox::XlsxCellType::inlineStr)  && str_val) 
                    {    
						CP_XML_NODE(L"v")	{ CP_XML_CONTENT(str_val.get()); }
					}
                    else if (!number_val.empty())
                    {
                        CP_XML_NODE(L"v")	{ CP_XML_CONTENT(number_val);}
					}
                    else if (xlsx_value_type == oox::XlsxCellType::b && bool_val)
                    {
                        CP_XML_NODE(L"v")   { CP_XML_CONTENT((int)(bool_val.get())); }
                    }

                }
				if ( is_data_visible || (cellStyle && is_style_visible && !last_cell_))
				{
					Context.get_table_context().state()->non_empty_row();
					empty_cell_count = 0 ;					
				} 
				else
				{
					empty_cell_count++;
					//Уведомление_о_вручении.ods - 13 повторов пустых с cellStyle=NULL - нужные !!!
					if (empty_cell_count > 19 && last_cell_&& (attlist_.table_number_columns_repeated_> 299 || cellStyle == NULL)) 
					{//пишем простыню только если задан стиль тока для этих ячеек
						skip_next_cell = true;
					}
				}				
			}
		}
        else
        {
            if (last_cell_) // Vehicle log book.ods (row = 24 and more)
				skip_next_cell = true;
        }

        Context.end_table_cell();
    }
}

void table_covered_table_cell::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wostream & strm = Context.current_sheet().sheetData();
    
	const common_value_and_type_attlist & attr	= attlist_.common_value_and_type_attlist_;

 	office_value_type::type	odf_value_type	= office_value_type::Custom;
	oox::XlsxCellType::type	xlsx_value_type	= oox::XlsxCellType::s;
	std::wstring formula					= attlist_.table_formula_.get_value_or(L"");
    
	std::wstring			number_val;
    _CP_OPT(bool)			bool_val;
	_CP_OPT(std::wstring)	str_val;
   
	std::wstring			num_format;

	size_t	xfId_last_set		= 0;
	int		empty_cell_count	= 0;
	bool	skip_next_cell		= false;
	bool	is_style_visible	= true;
	bool	is_data_visible		= false;
// вычислить стиль для ячейки

    std::wstring cellStyleName		= attlist_.table_style_name_.get_value_or(L"");
	std::wstring columnStyleName	= Context.get_table_context().default_column_cell_style();
	std::wstring rowStyleName		= Context.get_table_context().default_row_cell_style();

	if (attlist_.table_number_columns_repeated_ > 1)
		columnStyleName.clear(); // могут быть разные стили колонок Book 24.ods

	odf_read_context & odfContext = Context.root()->odf_context();   

	style_instance *defaultCellStyle=NULL, *defaultColumnCellStyle = NULL,  *defaultRowCellStyle =NULL, *cellStyle = NULL;
	try
	{
		defaultCellStyle		= odfContext.styleContainer().style_default_by_type(style_family::TableCell);

		defaultColumnCellStyle	= odfContext.styleContainer().style_by_name(columnStyleName,	style_family::TableCell, false);
		defaultRowCellStyle		= odfContext.styleContainer().style_by_name(rowStyleName,		style_family::TableCell, false);        
		cellStyle				= odfContext.styleContainer().style_by_name(cellStyleName,		style_family::TableCell, false);
	}
	catch(...)
	{
        _CP_LOG << L"[error]: style wrong\n";
	}

    std::wstring data_style = CalcCellDataStyle(Context, columnStyleName, rowStyleName, cellStyleName);

    // стили не наследуются
    std::vector<const style_instance *> instances;
    instances.push_back(defaultCellStyle);

    if (defaultColumnCellStyle)
        instances.push_back(defaultColumnCellStyle);

    if (defaultRowCellStyle)
    {
        if (instances.size() > 1)
            instances[1] = defaultRowCellStyle;
        else
            instances.push_back(defaultRowCellStyle);
    }

    if (cellStyle)
    {
        if (instances.size() > 1)
            instances[1] = cellStyle;
        else
            instances.push_back(cellStyle);
    }

    text_format_properties_content_ptr	textFormatProperties	= calc_text_properties_content		(instances);          
	paragraph_format_properties			parFormatProperties		= calc_paragraph_properties_content	(instances);
    style_table_cell_properties_attlist cellFormatProperties	= calc_table_cell_properties		(instances);

	if (attr.office_value_type_)
		odf_value_type = attr.office_value_type_->get_type();
	
	if ((odf_value_type == office_value_type::Float) || 
		(odf_value_type == office_value_type::Custom && attr.office_value_))
    {
        xlsx_value_type = oox::XlsxCellType::n;
        number_val = attr.office_value_.get_value_or(L"");
    }
    else if (odf_value_type == office_value_type::Percentage)
    {
        xlsx_value_type = oox::XlsxCellType::n;
        number_val	= attr.office_value_.get_value_or(L"");
    }
    else if (odf_value_type == office_value_type::Currency)
    {
        xlsx_value_type = oox::XlsxCellType::n;
        number_val	= attr.office_value_.get_value_or(L"");
    }
    else if ((odf_value_type == office_value_type::Date) || 
			 (odf_value_type == office_value_type::Custom && attr.office_date_value_))
    { 
        xlsx_value_type = oox::XlsxCellType::n;              
        if (attr.office_date_value_)
        {
            int y, m, d;
			_CP_OPT(int) h, min, s;
            if (oox::parseDateTime(attr.office_date_value_.get(), y, m, d, h, min, s))
            {
				boost::int64_t intDate = oox::convertDate(y, m, d);
				_CP_OPT(double) dTime;

				if (h && min)
				{
					dTime = oox::convertTime(*h, *min,  s.get_value_or(0));
				}
				if (intDate > 0)
				{
					if (dTime)
					{
						number_val = XmlUtils::ToString(*dTime + intDate);
					}
					else
					{
						number_val = std::to_wstring(intDate);
					}
				}
				else
				{
					str_val = attr.office_date_value_.get();
				}
            }
        }
    }
    else if ((odf_value_type == office_value_type::Time) ||
			 (odf_value_type == office_value_type::Custom && attr.office_time_value_))
    {
        xlsx_value_type = oox::XlsxCellType::n;
        if (attr.office_time_value_)
        {
            const std::wstring tv = attr.office_time_value_.get();
            int h,m;
            double s;
            if (oox::parseTime(tv, h, m, s))
            {
                number_val = boost::lexical_cast<std::wstring>(oox::convertTime(h, m, s));
            }                
        }
    }
    else if ((odf_value_type == office_value_type::Boolean) ||
			 (odf_value_type == office_value_type::Custom && attr.office_boolean_value_))
    {
        xlsx_value_type = oox::XlsxCellType::b;
        if (attr.office_boolean_value_)	bool_val = oox::parseBoolVal(attr.office_boolean_value_.get());
    }
    else if ((odf_value_type == office_value_type::String) ||
			 (odf_value_type == office_value_type::Custom && attr.office_string_value_))
    {
        xlsx_value_type = oox::XlsxCellType::str;
		if (attr.office_string_value_)	str_val = attr.office_string_value_.get();
    }

    if (!data_style.empty())
    {
		num_format = Context.get_num_format_context().find_complex_format(data_style, odf_value_type);
		
		if (num_format.empty())
		{
			office_element_ptr elm = odfContext.numberStyles().find_by_style_name(data_style);
			number_style_base *num_style = dynamic_cast<number_style_base*>(elm.get());

			if (num_style)
			{
				Context.get_num_format_context().start_complex_format(data_style);
				num_style->oox_convert(Context.get_num_format_context());
				Context.get_num_format_context().end_complex_format();

				num_format = Context.get_num_format_context().get_last_format();
			}
		}
    }

    oox::xlsx_cell_format cellFormat;
    
	cellFormat.set_cell_type(xlsx_value_type);
    cellFormat.set_num_format(oox::odf_string_to_build_in(odf_value_type));
    
	is_style_visible = (!cellStyleName.empty() || defaultColumnCellStyle) ? true : false;

	if ( content_.elements_.size() > 0	|| attlist_.table_content_validation_name_ || 
		!formula.empty()	||
		(	xlsx_value_type == oox::XlsxCellType::n										&& !number_val.empty()) || 
		(	xlsx_value_type == oox::XlsxCellType::b										&& bool_val) ||
		((	xlsx_value_type == oox::XlsxCellType::str || oox::XlsxCellType::inlineStr)	&& str_val))	is_data_visible = true;

	if (attlist_.table_number_columns_repeated_ < 199 && last_cell_)	last_cell_ = false;
	
	int cell_repeated_max = Context.current_table_column() + attlist_.table_number_columns_repeated_ + 1;

	if (cell_repeated_max >= 1024 && cellStyleName.empty() && last_cell_ && !is_data_visible)
	{//Book 24.ods
		return;
	}
    
	if (is_style_visible)
	{
		xfId_last_set = Context.get_style_manager().xfId(textFormatProperties, &parFormatProperties, &cellFormatProperties, &cellFormat, num_format, false, is_style_visible);
	}

	for (unsigned int r = 0; r < attlist_.table_number_columns_repeated_; ++r)
    {
        Context.start_table_covered_cell ();
		
		if (attlist_.table_content_validation_name_)
		{
			Context.get_dataValidations_context().activate(*attlist_.table_content_validation_name_, /*ref*/Context.current_table_column(), Context.current_table_row());
		}
		if (is_style_visible)
			Context.set_current_cell_style_id(xfId_last_set);
		
		const int sharedStringId = content_.xlsx_convert(Context, textFormatProperties);

		if (xlsx_value_type == oox::XlsxCellType::str && sharedStringId >= 0)
			xlsx_value_type = oox::XlsxCellType::s;//в случае текста, если он есть берем кэшированное значение
			
		if (skip_next_cell)break;

        // пустые ячейки пропускаем.
        if ( is_data_visible || ((cellStyle || defaultColumnCellStyle) && is_style_visible))
        {
            CP_XML_WRITER(strm)
            {
                CP_XML_NODE(L"c")
                {
                    CP_XML_ATTR(L"r", oox::getCellAddress(Context.current_table_column(), Context.current_table_row()));
					
					std::wstring type = oox::cellType2Str(xlsx_value_type);
                    if (false == type.empty()) CP_XML_ATTR(L"t", type);
                   
					CP_XML_ATTR(L"s", xfId_last_set);

                    if (!formula.empty())
                    {
						const std::wstring xlsxFormula = formulas_converter.convert(formula);
                        if (!xlsxFormula.empty())
                        {
                            CP_XML_NODE(L"f")
                            {
                                CP_XML_CONTENT(xlsxFormula);
                            }
                        }
                    }

					if (sharedStringId >= 0 && xlsx_value_type == oox::XlsxCellType::s)
                    {
                        CP_XML_NODE(L"v")	{ CP_XML_CONTENT(sharedStringId); }
                    }
					else if ((xlsx_value_type == oox::XlsxCellType::str || xlsx_value_type == oox::XlsxCellType::inlineStr)  && str_val) 
                    {    
						CP_XML_NODE(L"v")	{ CP_XML_CONTENT(str_val.get()); }
					}
                    else if (xlsx_value_type == oox::XlsxCellType::n && !number_val.empty())
                    {
                        CP_XML_NODE(L"v")	{ CP_XML_CONTENT(number_val);}
					}
                    else if (xlsx_value_type == oox::XlsxCellType::b && bool_val)
                    {
                        CP_XML_NODE(L"v")   { CP_XML_CONTENT((int)(bool_val.get())); }
                    }
                }
				if ( is_data_visible || (cellStyle && is_style_visible && !last_cell_))
				{
					Context.get_table_context().state()->non_empty_row();
					empty_cell_count = 0 ;					
				} 
				else
				{
					empty_cell_count++;
					//Уведомление_о_вручении.ods - 13 повторов пустых с cellStyle=NULL - нужные !!!
					if (empty_cell_count > 19 && last_cell_&& (attlist_.table_number_columns_repeated_> 299 || cellStyle == NULL)) 
					{//пишем простыню только если задан стиль тока для этих ячеек
						skip_next_cell = true;
					}
				}				
			}
		}
        else
        {
            if (last_cell_) // Vehicle log book.ods (row = 24 and more)
				skip_next_cell = true;
        }

        Context.end_table_covered_cell();
    }
}

void table_content_validations::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	for (size_t i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
    }
}
void table_content_validation::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	std::wstring name = table_name_.get_value_or(L"");
	
	Context.get_dataValidations_context().add(name, table_base_cell_address_.get_value_or(L""));
	Context.get_dataValidations_context().add_formula(name, table_condition_.get_value_or(L""));
	
	for (size_t i = 0 ; i < content_.size(); i++)
	{
		if (content_[i]->get_type() == typeTableErrorMassage)
		{
			table_error_message* error = dynamic_cast<table_error_message*>(content_[i].get());

			Context.get_text_context().start_only_text();
			for (size_t j = 0 ; j < error->content_.size(); j++)
			{
				error->content_[j]->xlsx_convert(Context);
			}
			std::wstring content = Context.get_text_context().end_only_text();

			Context.get_dataValidations_context().add_error_msg(name, error->table_title_.get_value_or(L""), content, 
																error->table_display_ ? error->table_display_->get() : true,
																error->table_message_type_.get_value_or(message_type::stop).get_type());
		}
		else if (content_[i]->get_type() == typeTableHelpMassage)
		{
			table_help_message* help = dynamic_cast<table_help_message*>(content_[i].get());
			
			Context.get_text_context().start_only_text();
			for (size_t j = 0 ; j < help->content_.size(); j++)
			{
				help->content_[j]->xlsx_convert(Context);
			}
			std::wstring content = Context.get_text_context().end_only_text();
			
			Context.get_dataValidations_context().add_help_msg(name, help->table_title_.get_value_or(L""), content, 
																help->table_display_ ? help->table_display_->get() : true);
		}
		content_[i]->xlsx_convert(Context);
    }
}
void table_error_message::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
}
void table_help_message::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
}
}
}
