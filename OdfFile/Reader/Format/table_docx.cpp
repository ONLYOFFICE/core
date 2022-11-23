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

#include "serialize_elements.h"
#include "odfcontext.h"
#include "odf_document.h"

#include "style_table_properties.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

bool table_table_cell_content::docx_convert(oox::docx_conversion_context & Context)
{
    bool wasPar = false;
 	for (size_t i = 0; i < elements_.size(); i++)
    {
		ElementType type = elements_[i]->get_type();
        
		if (type == typeTextP || type== typeTextH)
            wasPar = true;

        elements_[i]->docx_convert(Context);
    }
    return wasPar;
}

void table_table_row::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();

    const std::wstring styleName = attlist_.table_style_name_.get_value_or(L"");
    const std::wstring defaultCellStyle = attlist_.table_default_cell_style_name_.get_value_or(L"");

    const style_instance * inst = 
        Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableRow,Context.process_headers_footers_);
	
	style_table_cell_properties	*cell_props = NULL;
	style_table_row_properties	*row_props = NULL;
	
	if (inst && inst->content())
	{
		cell_props = inst->content()->get_style_table_cell_properties(true);
		row_props = inst->content()->get_style_table_row_properties();

		if ((row_props) && (row_props->attlist_.common_background_color_attlist_.fo_background_color_))
		{
			if (!cell_props->attlist_.common_background_color_attlist_.fo_background_color_)
				cell_props->attlist_.common_background_color_attlist_.fo_background_color_ = row_props->attlist_.common_background_color_attlist_.fo_background_color_;
		}
	}
    for (unsigned int i = 0; i < attlist_.table_number_rows_repeated_; ++i)
    {
        _Wostream << L"<w:tr>";
        
        _Wostream << L"<w:trPr>";   

		if (bHeader) _Wostream << L"<w:tblHeader/>";

			bool bCantSplit = false;

			if (row_props)
			{
				if (row_props->attlist_.fo_keep_together_ && row_props->attlist_.fo_keep_together_->get_type() == keep_together::Always)
				{
					_Wostream << L"<w:cantSplit w:val=\"true\"/>"; 
				}
				if (row_props->attlist_.style_row_height_)
				{
					int val = (int)( 0.5 + 20.0 * row_props->attlist_.style_row_height_->get_value_unit(length::pt) );
					if (val < 0)
						val = 0;
					_Wostream << L"<w:trHeight w:val=\"" << val << L"\" w:hRule=\"exact\"/>";
				}
				else if (row_props->attlist_.style_min_row_height_)
				{
					int val = (int)( 0.5 + 20.0 * row_props->attlist_.style_min_row_height_->get_value_unit(length::pt) );
					if (val < 0)
						val = 0;
					_Wostream << L"<w:trHeight w:val=\"" << val << L"\" w:hRule=\"atLeast\"/>";
				}	

			}

			if (cell_props)
				cell_props->docx_convert(Context);

        _Wostream << L"</w:trPr>";

        Context.get_table_context().start_row(styleName, defaultCellStyle);
        
 		for (size_t i = 0; i < content_.size(); i++)
        {
            content_[i]->docx_convert(Context);
        }

        Context.get_table_context().end_row();

        _Wostream << L"</w:tr>";
    }
}

void table_table_rows::docx_convert(oox::docx_conversion_context & Context)
{
 	for (size_t i = 0; i < table_table_row_.size(); i++)
    {
        table_table_row_[i]->docx_convert(Context);
    }
}

void table_table_header_rows::docx_convert(oox::docx_conversion_context & Context)
{
 	for (size_t i = 0; i < table_table_row_.size(); i++)
    {
        table_table_row_[i]->docx_convert(Context);
    }
}

void table_rows::docx_convert(oox::docx_conversion_context & Context)
{
    if (table_table_rows_)
        table_table_rows_->docx_convert(Context);
    else
    {
 		for (size_t i = 0; i < table_table_row_.size(); i++)
        {
            table_table_row_[i]->docx_convert(Context);
        }
    }    
}

void table_rows_no_group::docx_convert(oox::docx_conversion_context & Context)
{
    table_rows_1_.docx_convert(Context);
    
    if (table_table_header_rows_)
        table_table_header_rows_->docx_convert(Context);

    table_rows_2_.docx_convert(Context);    
}

void table_rows_and_groups::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
        content_[i]->docx_convert(Context);
    }
}

void table_table::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();

	std::wstring sDumpPageProperties;
	if (false == Context.get_paragraph_state())
	{
		std::wstringstream strm;
		if (Context.process_page_properties(strm))
		{
			sDumpPageProperties = strm.str();
		}
	}

	bool sub_table = attlist_.table_is_sub_table_.get_value_or(false);
	//todooo придумать как сделать внешние границы sub-таблицы границами внешней ячейки (чтоб слияние произошло)
	
	std::wstring tableStyleName = attlist_.table_style_name_.get_value_or(L"");

	_Wostream << L"<w:tbl>";    

	Context.start_changes(); //TblPrChange
	
	Context.get_table_context().start_table(tableStyleName);

    style_instance * inst = 
        Context.root()->odf_context().styleContainer().style_by_name(tableStyleName, style_family::Table, Context.process_headers_footers_);
    
    Context.get_styles_context().start();

    if (inst && inst->content())
        inst->content()->docx_convert(Context);

	Context.get_styles_context().docx_serialize_table_style(_Wostream, Context.get_text_tracked_context().dumpTblPr_);

	_Wostream << L"<w:tblGrid>";
	table_columns_and_groups_.docx_convert(Context);
	_Wostream << L"</w:tblGrid>";

    table_rows_and_groups_.docx_convert(Context);

	Context.get_table_context().end_table();
	_Wostream << L"</w:tbl>";

	if (false == sDumpPageProperties.empty())
	{			
		Context.output_stream() << L"<w:p>";
		Context.output_stream() << L"<w:pPr>";
			Context.output_stream() << sDumpPageProperties;
		Context.output_stream() << L"</w:pPr>";
		Context.output_stream() << L"</w:p>";
	}
}

void table_columns::docx_convert(oox::docx_conversion_context & Context)
{
    if (table_table_columns_)
        table_table_columns_->docx_convert(Context);

	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->docx_convert(Context);
    }
}

void table_table_columns::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->docx_convert(Context);
    }
}

void table_columns_no_group::docx_convert(oox::docx_conversion_context & Context)
{
    table_columns_1_.docx_convert(Context);
    
    if (table_table_header_columns_)
        table_table_header_columns_->docx_convert(Context);

    table_columns_2_.docx_convert(Context);
}

void table_columns_and_groups::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
    //if (table_table_column_group_)
    //    table_table_column_group_->docx_convert(Context);
    //else
    //    table_columns_no_group_.docx_convert(Context);
}

void table_table_header_columns::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < table_table_column_.size(); i++)
    {
        table_table_column_[i]->docx_convert(Context);
    }    
}

void table_table_column_group::docx_convert(oox::docx_conversion_context & Context)
{
    table_columns_and_groups_.docx_convert(Context);
}

void table_table_column::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();
    const unsigned int columnsRepeated = table_table_column_attlist_.table_number_columns_repeated_;
    const std::wstring defaultCellStyle = table_table_column_attlist_.table_default_cell_style_name_.get_value_or(L"");
    Context.get_table_context().start_column(columnsRepeated, defaultCellStyle);

    for (unsigned int i = 0; i < columnsRepeated; ++i)
    {
		bool bAddWidth = false;
        if (table_table_column_attlist_.table_style_name_)
        {
            const std::wstring colStyleName = table_table_column_attlist_.table_style_name_.get();
            if (style_instance * inst = 
                Context.root()->odf_context().styleContainer().style_by_name( colStyleName , style_family::TableColumn, Context.process_headers_footers_ ))
            {
                if (inst->content())
				{
                    inst->content()->docx_convert(Context);
					bAddWidth = true;
				}
            }
        }
		if (false == bAddWidth)
		{
			Context.get_table_context().add_column_width(0);
		}
    }
}

const wchar_t * emptyPar = L"<w:p><w:pPr></w:pPr><w:r><w:rPr></w:rPr></w:r></w:p>";

void table_table_cell::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();

    for (unsigned int r = 0; r < attlist_.table_number_columns_repeated_; ++r)
    {
        int pushTextPropCount = 0;

        Context.get_table_context().start_cell();
        _Wostream << L"<w:tc>";
        _Wostream << L"<w:tcPr>";

		const std::wstring styleName = attlist_.table_style_name_.get_value_or(L""); 
		
        if (attlist_extra_.table_number_rows_spanned_ > 1)
        {
            _Wostream << L"<w:vMerge w:val=\"restart\"/>"; 
            Context.get_table_context().set_rows_spanned(Context.get_table_context().current_column(), 
                attlist_extra_.table_number_rows_spanned_ - 1,
                attlist_extra_.table_number_columns_spanned_ - 1,
                styleName
                );
        }        		

		if (attlist_extra_.table_number_columns_spanned_ > 1)
        {
            _Wostream << L"<w:gridSpan w:val=\"" << attlist_extra_.table_number_columns_spanned_ << "\"/>";
            Context.get_table_context().set_columns_spanned(attlist_extra_.table_number_columns_spanned_ - 1);
        }	
		double width = Context.get_table_context().get_current_cell_width();
		
		if (width > 0.01)
		{
			_Wostream << L"<w:tcW w:w=\"" << (int)width << L"\" w:type=\"dxa\"/>";
		}
		


		const style_instance * inst = 
            Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableCell, Context.process_headers_footers_);
        
        const std::wstring & currentRowStyle = Context.get_table_context().current_row_style();

        const style_instance * inst_row = 
            Context.root()->odf_context().styleContainer().style_by_name(currentRowStyle, style_family::TableRow, Context.process_headers_footers_);

		style_table_cell_properties *row_cell_props = NULL;
		if (inst_row && inst_row->content())
		{
			row_cell_props = inst_row->content()->get_style_table_cell_properties();
		}
		if (inst && inst->content())
		{
			style_table_cell_properties merge_cell_props;

			style_table_cell_properties * cell_props = inst->content()->get_style_table_cell_properties();

			if (row_cell_props)
				merge_cell_props.attlist_.apply_from(row_cell_props->attlist_);
			if (cell_props)
				merge_cell_props.attlist_.apply_from(cell_props->attlist_);
            
			merge_cell_props.docx_convert(Context);            

            if (inst->content()->get_style_text_properties())
            {
                Context.push_text_properties(inst->content()->get_style_text_properties());
                pushTextPropCount++;
            }
        }
        _Wostream << L"</w:tcPr>";

        /// Обрабатываем стиль по умолчанию для данного столбца
        {
            const std::wstring & defaultCellStyle =
                Context.get_table_context().get_default_cell_style_col(Context.get_table_context().current_column());

            if (const style_instance * inst = 
                Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyle, style_family::TableCell, Context.process_headers_footers_))
            {
                if (const style_content * content = inst->content())
                {
                    if (content->get_style_text_properties())
                    {
                        Context.push_text_properties(content->get_style_text_properties());
                        pushTextPropCount++;
                    }
                }
            }
        }

        /// Стиль по умолчанию для данной строки
        {
            const std::wstring & defaultCellStyle = Context.get_table_context().get_default_cell_style_row();

            if (const style_instance * inst = 
                Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyle, style_family::TableCell, Context.process_headers_footers_))
            {
                if (const style_content * content = inst->content())
                {
                    if (content->get_style_text_properties())
                    {
                        Context.push_text_properties(content->get_style_text_properties());
                        pushTextPropCount++;
                    }
                }
            }
        }
		
        // если одержимое не содержит ниодного параграфа, то добавляем параграф, иначе word считает файл битым
        if (!content_.docx_convert(Context))
        {
            _Wostream << emptyPar;
        }

        for (int i = 0; i < pushTextPropCount; ++i)
            Context.pop_text_properties();

        _Wostream << L"</w:tc>";
        Context.get_table_context().end_cell();
    }
}

void table_covered_table_cell::docx_convert(oox::docx_conversion_context & Context) 
{
    std::wostream & _Wostream = Context.output_stream();

    for (unsigned int i = 0; i < attlist_.table_number_columns_repeated_; ++i)
    {
        if (Context.get_table_context().start_covered_cell(Context))
        {
            if (!content_.docx_convert(Context))
            {
                _Wostream << emptyPar;
            }
        }
        Context.get_table_context().end_covered_cell();
    }
}

}
}
