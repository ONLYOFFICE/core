#include "precompiled_cpodf.h"
#include "table.h"

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>

#include "style_table_properties.h"

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
        _Wostream << L"<a:tr h=\"822264\">";
        const style_instance * inst = 
            Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableRow,false);
        
        //_Wostream << L"<a:trPr>";        
		//	_Wostream << L"<a:cantSplit a:val=\"false\" />"; 
		//todoooo	if (inst && inst->content())inst->content()->pptx_convert(Context);
        //_Wostream << L"</a:trPr>";

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

    style_instance * inst = 
        Context.root()->odf_context().styleContainer().style_by_name( tableStyleName , style_family::Table,false);
    
    Context.get_text_context().get_styles_context().start();

    if (inst && inst->content())
        inst->content()->pptx_convert(Context);
    
    Context.get_text_context().get_styles_context().pptx_serialize_table_style(_Wostream);

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
            if (style_instance * inst = 
                Context.root()->odf_context().styleContainer().style_by_name( colStyleName , style_family::TableColumn,false ))
            {
                if (inst->content())
                    inst->content()->pptx_convert(Context);
            }
        }
    }
}

const wchar_t * emptyParTable = L"<a:p><a:pPr></a:pPr><a:r><a:rPr></a:rPr></a:r></a:p>";

void table_table_cell::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.get_table_context().tableData();

    for (unsigned int r = 0; r < table_table_cell_attlist_.table_number_columns_repeated_; ++r)
    {
        int pushTextPropCount = 0;

        Context.get_table_context().start_cell();
        _Wostream << L"<a:tc>";

		const std::wstring styleName = table_table_cell_attlist_.table_style_name_ ?
            table_table_cell_attlist_.table_style_name_->style_name() : L""; 

		//_Wostream << L"<a:tcW a:w=\"0\" a:type=\"auto\" />";
		
        if (table_table_cell_attlist_extra_.table_number_rows_spanned_ > 1)
        {
            _Wostream << L"<a:vMerge a:val=\"restart\" />"; 
            Context.get_table_context().set_rows_spanned(Context.get_table_context().current_column(), 
                table_table_cell_attlist_extra_.table_number_rows_spanned_ - 1,
                table_table_cell_attlist_extra_.table_number_columns_spanned_ - 1,
                styleName
                );
        }        		
		
		if (table_table_cell_attlist_extra_.table_number_columns_spanned_ > 1)
        {
            _Wostream << L"<a:gridSpan a:val=\"" << table_table_cell_attlist_extra_.table_number_columns_spanned_ << "\" />";
            Context.get_table_context().set_columns_spanned(table_table_cell_attlist_extra_.table_number_columns_spanned_ - 1);
        }

		const style_instance * inst = 
            Context.root()->odf_context().styleContainer().style_by_name( styleName , style_family::TableCell,false);
        
        if (inst && inst->content())
        {
            if (inst->content()->get_style_table_cell_properties())
            {
                inst->content()->get_style_table_cell_properties()->pptx_convert(Context);            
            }

            if (inst->content()->get_style_text_properties())
            {
//todooo        Context.push_text_properties(inst->content()->get_style_text_properties());
                pushTextPropCount++;
            }
        }
        //_Wostream << L"</a:tcPr>";

        /// Обрабатываем стиль по умолчанию для данного столбца
        const std::wstring & defaultColumnStyle =
            Context.get_table_context().get_default_cell_style_col(Context.get_table_context().current_column());

        if (const style_instance * inst = 
            Context.root()->odf_context().styleContainer().style_by_name(defaultColumnStyle, style_family::TableCell,false))
        {
            if (const style_content * content = inst->content())
            {
                if (content->get_style_text_properties())
                {
//todooo                Context.push_text_properties(content->get_style_text_properties());
                    pushTextPropCount++;
                }
            }
        }
        /// Стиль по умолчанию для данной строки
        const std::wstring & defaultRowStyle =
            Context.get_table_context().get_default_cell_style_row();

        if (const style_instance * inst = 
            Context.root()->odf_context().styleContainer().style_by_name(defaultRowStyle, style_family::TableCell,false))
        {
            if (const style_content * content = inst->content())
            {
                if (content->get_style_text_properties())
                {
//todooo				Context.push_text_properties(content->get_style_text_properties());
                    pushTextPropCount++;
                }
            }
        }
		
		_Wostream << L"<a:txBody>";
		_Wostream << L"<a:bodyPr/>";
		Context.get_text_context().start_object();

		bool presentText = table_table_cell_content_.pptx_convert(Context);
		
		std::wstring cellContent = Context.get_text_context().end_object();

		if (cellContent.length()>0)
			_Wostream << cellContent;
		else
            _Wostream << emptyParTable;

		_Wostream << L"</a:txBody>";

		//todooo
        //for (int i = 0; i < pushTextPropCount; ++i)
        //    Context.pop_text_properties();

        _Wostream << L"<a:tcPr/>";
        _Wostream << L"</a:tc>";
        Context.get_table_context().end_cell();
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
