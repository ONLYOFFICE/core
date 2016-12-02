/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>

#include "style_table_properties.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

bool table_table_cell_content::docx_convert(oox::docx_conversion_context & Context)
{
    bool wasPar = false;
    BOOST_FOREACH(const office_element_ptr & elm, elements_)
    {
		ElementType type = elm->get_type();
        
		if (type == typeTextP || type== typeTextH)
            wasPar = true;

        elm->docx_convert(Context);
    }
    return wasPar;
}

void table_table_row::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();

    const std::wstring styleName = table_table_row_attlist_.table_style_name_.get_value_or(L"");
    const std::wstring defaultCellStyle = table_table_row_attlist_.table_default_cell_style_name_.get_value_or(L"");

    for (unsigned int i = 0; i < table_table_row_attlist_.table_number_rows_repeated_; ++i)
    {
        _Wostream << L"<w:tr>";
        const style_instance * inst = 
            Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableRow,Context.process_headers_footers_);
        
        _Wostream << L"<w:trPr>";        
			_Wostream << L"<w:cantSplit w:val=\"false\" />"; 
			if (inst && inst->content())inst->content()->docx_convert(Context);
        _Wostream << L"</w:trPr>";

        Context.get_table_context().start_row(styleName, defaultCellStyle);
        
        BOOST_FOREACH(const office_element_ptr & elm, content_)
        {
            elm->docx_convert(Context);
        }

        Context.get_table_context().end_row();

        _Wostream << L"</w:tr>";
    }
}

void table_table_rows::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
    {
        elm->docx_convert(Context);
    }
}

void table_table_header_rows::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
    {
        elm->docx_convert(Context);
    }
}

void table_rows::docx_convert(oox::docx_conversion_context & Context)
{
    if (table_table_rows_)
        table_table_rows_->docx_convert(Context);
    else
    {
        BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
        {
            elm->docx_convert(Context);
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
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
}

void table_table::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();

	bool sub_table = table_table_attlist_.table_is_sub_table_.get_value_or(false);
	//todooo придумать как сделать внешние границы sub-таблицы границами внешней ячейки (чтоб слияние произошло)
	
	std::wstring tableStyleName = table_table_attlist_.table_style_name_.get_value_or(L"");

	_Wostream << L"<w:tbl>";    

	Context.start_changes(); //TblPrChange
	
	Context.get_table_context().start_table(tableStyleName);

    style_instance * inst = 
        Context.root()->odf_context().styleContainer().style_by_name( tableStyleName , style_family::Table, Context.process_headers_footers_);
    
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
}

void table_columns::docx_convert(oox::docx_conversion_context & Context)
{
    if (table_table_columns_)
        table_table_columns_->docx_convert(Context);

    BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
    {
        elm->docx_convert(Context);
    }
}

void table_table_columns::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
    {
        elm->docx_convert(Context);
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
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }
    //if (table_table_column_group_)
    //    table_table_column_group_->docx_convert(Context);
    //else
    //    table_columns_no_group_.docx_convert(Context);
}

void table_table_header_columns::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
    {
        elm->docx_convert(Context);
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
        if (table_table_column_attlist_.table_style_name_)
        {
            const std::wstring colStyleName = table_table_column_attlist_.table_style_name_.get();
            if (style_instance * inst = 
                Context.root()->odf_context().styleContainer().style_by_name( colStyleName , style_family::TableColumn,Context.process_headers_footers_ ))
            {
                if (inst->content())
                    inst->content()->docx_convert(Context);
            }
        }
    }
}

const wchar_t * emptyPar = L"<w:p><w:pPr></w:pPr><w:r><w:rPr></w:rPr></w:r></w:p>";

void table_table_cell::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();

    for (unsigned int r = 0; r < table_table_cell_attlist_.table_number_columns_repeated_; ++r)
    {
        int pushTextPropCount = 0;

        Context.get_table_context().start_cell();
        _Wostream << L"<w:tc>";
        _Wostream << L"<w:tcPr>";

		const std::wstring styleName = table_table_cell_attlist_.table_style_name_.get_value_or(L""); 

		//_Wostream << L"<w:tcW w:w=\"0\" w:type=\"auto\" />";
		
        if (table_table_cell_attlist_extra_.table_number_rows_spanned_ > 1)
        {
            _Wostream << L"<w:vMerge w:val=\"restart\" />"; 
            Context.get_table_context().set_rows_spanned(Context.get_table_context().current_column(), 
                table_table_cell_attlist_extra_.table_number_rows_spanned_ - 1,
                table_table_cell_attlist_extra_.table_number_columns_spanned_ - 1,
                styleName
                );
        }        		
		
		if (table_table_cell_attlist_extra_.table_number_columns_spanned_ > 1)
        {
            _Wostream << L"<w:gridSpan w:val=\"" << table_table_cell_attlist_extra_.table_number_columns_spanned_ << "\" />";
            Context.get_table_context().set_columns_spanned(table_table_cell_attlist_extra_.table_number_columns_spanned_ - 1);
        }

		const style_instance * inst = 
            Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableCell,Context.process_headers_footers_);
        
        if (inst && inst->content())
        {
            if (inst->content()->get_style_table_cell_properties())
            {
                inst->content()->get_style_table_cell_properties()->docx_convert(Context);            
            }

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
                Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyle, style_family::TableCell,Context.process_headers_footers_))
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
            const std::wstring & defaultCellStyle =
                Context.get_table_context().get_default_cell_style_row();

            if (const style_instance * inst = 
                Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyle, style_family::TableCell,Context.process_headers_footers_))
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
        if (!table_table_cell_content_.docx_convert(Context))
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

    for (unsigned int i = 0; i < table_table_cell_attlist_.table_number_columns_repeated_; ++i)
    {
        if (Context.get_table_context().start_covered_cell(Context))
        {
            if (!table_table_cell_content_.docx_convert(Context))
            {
                _Wostream << emptyPar;
            }
        }
        Context.get_table_context().end_covered_cell();
    }
}

}
}
