#include "precompiled_cpodf.h"
#include "table.h"

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/formulasconvert.h>

#include <boost/lexical_cast.hpp>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>
#include "calcs_styles.h"
#include "../docx/xlsx_utils.h"
#include "../docx/xlsx_cell_format.h"

#include <cpdoccore/formulasconvert.h>
#include "search_table_cell.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {

static formulasconvert::odf2oox_converter formulas_converter;

using xml::xml_char_wc;

int table_table_cell_content::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    Context.get_table_context().start_cell_content();
    BOOST_FOREACH(office_element_ptr const & elm, text_content_)
    {
        elm->xlsx_convert(Context);
    }
    const int sharedStrId = Context.get_table_context().end_cell_content();
    return sharedStrId;
}

void table_table_row::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wostream & strm = Context.current_sheet().sheetData();
///обработка чтилей дл€ роу - 
	size_t Default_Cell_style_in_row_ = 0; 

    const std::wstring styleName = table_table_row_attlist_.table_style_name_.get_value_or( style_ref(L"") ).style_name();
    const std::wstring defaultCellStyleName = table_table_row_attlist_.table_default_cell_style_name_.get_value_or( style_ref(L"") ).style_name();

	style_instance * instStyle_CellDefault = 
				Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyleName, style_family::TableCell,false/*false*/);
	style_table_cell_properties * prop_CellDefault = NULL;
	
	if ((instStyle_CellDefault) && (instStyle_CellDefault->content())) prop_CellDefault = instStyle_CellDefault->content()->get_style_table_cell_properties();
//кастомные настройки стил€ €чеек в данном роу
	if (prop_CellDefault)
		//то есть проверим что есть вообще кастом дл€ роу- а потом уже посчитаем стиль
	{
		odf::style_table_cell_properties_attlist	cellFormatProperties	= calc_table_cell_properties(instStyle_CellDefault);
		Default_Cell_style_in_row_ = Context.get_style_manager().xfId(NULL,NULL, &cellFormatProperties, NULL, L"",true);	
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool skip_next_row = false;

#ifdef _DEBUG
	_CP_LOG(info) << "[info][xlsx] process repeated rows " << table_table_row_attlist_.table_number_rows_repeated_ << std::endl;
#endif
    std::wstring ht = L"";
    double row_height = 0.0;

    odf_read_context & odfContext = Context.root()->odf_context();

    do 
    {        
        odf::style_instance * rowStyle = odfContext.styleContainer().style_by_name(styleName, odf::style_family::TableRow,false/*false*/);
        if (!rowStyle)
            break;

        if (!rowStyle->content())
            break;

        const odf::style_table_row_properties * prop = rowStyle->content()->get_style_table_row_properties();
        if (!prop)
            break;

        if (const _CP_OPT(odf::length) & height = prop->style_table_row_properties_attlist_.style_row_height_)
        {
            row_height = height->get_value_unit(odf::length::pt);

			if ((prop->style_table_row_properties_attlist_.style_use_optimal_row_height_) && 
						(*prop->style_table_row_properties_attlist_.style_use_optimal_row_height_==true))
			{
				//автоматическа€ подстройка высоты.
			}
			else
			{
				std::wstringstream ht_s;
				ht_s.precision(3);
				ht_s << std::fixed << row_height;
				ht = ht_s.str();
			}
        }
    }
    while (0); 

    bool hidden = table_table_row_attlist_.table_visibility_.get_type() == table_visibility::Collapse;

    for (unsigned int i = 0; i < table_table_row_attlist_.table_number_rows_repeated_; ++i)
    {
        Context.start_table_row(styleName, defaultCellStyleName);
        
        if (!skip_next_row)
        {
           CP_XML_WRITER(strm)
            {
                CP_XML_NODE(L"row")
                {
                    CP_XML_ATTR(L"r", Context.current_table_row() + 1);

					if (Context.get_table_context().state().group_row_.enabled)
					{
						//std::wstring str_spans = boost::lexical_cast<std::wstring>(Context.get_table_context().state().group_row_.count);
						//str_spans = str_spans + L":";
						std::wstring str_spans = L"1:" + boost::lexical_cast<std::wstring>(Context.get_table_context().columns_count());
						ht = L"";

						CP_XML_ATTR(L"collapsed",	Context.get_table_context().state().group_row_.collapsed);
						CP_XML_ATTR(L"outlineLevel", Context.get_table_context().state().group_row_.level); 
						CP_XML_ATTR(L"spans", str_spans);						
						
						if (Context.get_table_context().state().group_row_.collapsed)hidden = false;
						Context.get_table_context().state().group_row_.count--;

						if (Context.get_table_context().state().group_row_.count<1)
							Context.get_table_context().state().group_row_.enabled = false;
					}					

                    if (hidden)
                    {
                        CP_XML_ATTR(L"hidden", L"true");                        
                    }

                    if (!ht.empty())
                    {
                        CP_XML_ATTR(L"customHeight", L"true");
                        CP_XML_ATTR(L"ht", ht);                                            
                    }
					if (Default_Cell_style_in_row_>0)
					{
						CP_XML_ATTR(L"customFormat", 1);
						CP_XML_ATTR(L"s", Default_Cell_style_in_row_ );
					}									

                    CP_XML_STREAM();

                    BOOST_FOREACH(const office_element_ptr & elm, content_)
                    {
                        elm->xlsx_convert(Context);
                    }                                                        
                }
            }
        }
        Context.end_table_row();        

        if (Context.is_empty_row())
            skip_next_row = true;  
    }

    Context.get_table_metrics().add_rows(table_table_row_attlist_.table_number_rows_repeated_, !hidden ? row_height : 0.0);

}

void table_table_rows::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    BOOST_FOREACH(office_element_ptr const & elm, table_table_row_)
    {
        elm->xlsx_convert(Context);
    }
}

void table_table_header_rows::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    BOOST_FOREACH(office_element_ptr const & elm, table_table_row_)
    {
        elm->xlsx_convert(Context);
    }
}

void table_rows::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    if (table_table_rows_)
        table_table_rows_->xlsx_convert(Context);
    else
    {
        BOOST_FOREACH(office_element_ptr const & elm, table_table_row_)
        {
            elm->xlsx_convert(Context);
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
    //BOOST_FOREACH(office_element_ptr const & elm, content_)
	int i=0;
	int size = content_.size();
	while(true)
    {
		if (i>=size)break;
        content_[i]->xlsx_convert(Context);
		i++;
    }
}

void table_table_row_group::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	int count = table_rows_and_groups_.get_count();

	int level = 1;
	
	Context.set_table_row_group(count,table_table_row_group_attlist_.table_display_,level);
	table_rows_and_groups_.xlsx_convert(Context);
}

void table_table::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    const std::wstring tableStyleName = table_table_attlist_.table_style_name_ ? table_table_attlist_.table_style_name_->style_name() : L"";
    const std::wstring tableName = table_table_attlist_.table_name_.get_value_or(L"");

    _CP_LOG(info) << L"[info][xlsx] process table \"" << tableName /*L"" */<< L"\"" << std::endl;

	if (table_table_source_)
	{
		table_table_source* table_source = dynamic_cast<table_table_source*>( table_table_source_.get() );
		if ( table_source)
		{
			if (table_source->table_linked_source_attlist_.common_xlink_attlist_.href_)return;
		}

	}
    //office-dde-sourcetable_linked_source_attlist
    Context.start_table(tableName, tableStyleName);

	table_columns_and_groups_.xlsx_convert(Context);
    table_rows_and_groups_.xlsx_convert(Context);

    if (table_shapes_)
        table_shapes_->xlsx_convert(Context);

    Context.end_table();
}

void table_columns::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    if (table_table_columns_)
        table_table_columns_->xlsx_convert(Context);

    BOOST_FOREACH(office_element_ptr const & elm, table_table_column_)
    {
        elm->xlsx_convert(Context);
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
    /*
    if (table_table_column_group_)
        table_table_column_group_->xlsx_convert(Context);
    else
        table_columns_no_group_.xlsx_convert(Context);
    */
    //BOOST_FOREACH(office_element_ptr const & elm, content_)
    //{
    //    elm->xlsx_convert(Context);
    //}
	int i=0;
	int size = content_.size();
	while(true)
    {
		if (i>=size)break;
		office_element_ptr const & elm = content_[i];
        elm->xlsx_convert(Context);
		i++;
    }
}

void table_table_header_columns::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    BOOST_FOREACH(office_element_ptr const & elm, table_table_column_)
    {
        elm->xlsx_convert(Context);
    }    
}

void table_table_column_group::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    table_columns_and_groups_.xlsx_convert(Context);
}

namespace {
// see ECMA-376 page 1768

double pixToSize(double pixels, double maxDigitSize)
{
	return (int((pixels-5)/maxDigitSize*100.+0.5))/100.;
}
double cmToChars (double cm)
{
	double pt = cm * 28.34467120181406;
	return pt * 48. /(12.75 * 8.43 *2.);
}
}

void table_table_column::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wostream & strm = Context.current_sheet().cols();

    const unsigned int columnsRepeated		= table_table_column_attlist_.table_number_columns_repeated_;
    const std::wstring styleName			= table_table_column_attlist_.table_style_name_.get_value_or(style_ref(L"")).style_name();
    const std::wstring defaultCellStyleName = table_table_column_attlist_.table_default_cell_style_name_.get_value_or(style_ref(L"")).style_name();


    int cMin = 0, cMax = 0;
    Context.start_table_column(columnsRepeated, defaultCellStyleName, cMin, cMax);

    double pt_width = 0.0;
	double cm_width = 0.0;  
	double in_width = 0.0;  
    
    CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"col")
        {
            CP_XML_ATTR(L"min", (cMin+1));
            CP_XML_ATTR(L"max", cMax);

            const bool collapsed = table_table_column_attlist_.table_visibility_.get_type() == table_visibility::Collapse;

            if (collapsed)
            {
                CP_XML_ATTR(L"hidden", L"true");
            }
            //else
            //{
            //    CP_XML_ATTR(L"hidden", L"false");            
            //}

            if (table_table_column_attlist_.table_style_name_)
            {

				if (style_instance * inst = 
					Context.root()->odf_context().styleContainer().style_by_name(defaultCellStyleName, style_family::TableCell,false/*false*/))
				{
                    if (inst->content())
                    {
						if (const style_table_cell_properties * prop = inst->content()->get_style_table_cell_properties())
						{//сделать проверку чтоб сюда не попал дефолтный, то  сть пустой стиль
							//fo:background-color
							//if (prop->common_background_color_attlist_.fo_background_color)
							{
								//oox::xlsx_cell_format cellFormat;
								//
								//cellFormat.set_cell_type(XlsxCellType::s);
								//cellFormat.set_num_format(oox::odf_string_to_build_in(L""));
								odf::style_table_cell_properties_attlist	cellFormatProperties	= calc_table_cell_properties(inst);
								size_t style_ = Context.get_style_manager().xfId(NULL,NULL, &cellFormatProperties, NULL, L"",true);	

								if (style_>0)
								CP_XML_ATTR(L"style", style_ );
							}
								
						}
					}
				}
                const std::wstring colStyleName = table_table_column_attlist_.table_style_name_->style_name();
                if (style_instance * inst = Context.root()->odf_context().styleContainer().style_by_name(colStyleName, style_family::TableColumn,false))
                {
                    if (inst->content())
                    {
                        if (const style_table_column_properties * prop = inst->content()->get_style_table_column_properties())
                        {  
                            if (prop->style_table_column_properties_attlist_.style_column_width_)
                            {
                                pt_width = prop->style_table_column_properties_attlist_.style_column_width_->get_value_unit(length::pt);                        
                                cm_width = prop->style_table_column_properties_attlist_.style_column_width_->get_value_unit(length::cm);        
								in_width = prop->style_table_column_properties_attlist_.style_column_width_->get_value_unit(length::inch);

                                if (collapsed)
                                {
                                    in_width = 0.0;
                                }
								
                                const double pixDpi = in_width * getDefaultDpi();                
                                const double width = pixToSize(pixDpi, Context.getMaxDigitSize().first); 

								//const double width = cmToChars(prop->style_table_column_properties_attlist_.style_column_width_->get_value_unit(length::cm));
                              
								// see ECMA-376 page 1768
                                CP_XML_ATTR(L"width", width);
                                CP_XML_ATTR(L"customWidth", true);
                                Context.table_column_last_width(width);
                            }
                            else
                            {
                                CP_XML_ATTR(L"customWidth", 0);
                                Context.table_column_last_width(0.0);
                            }
                        }
                    }                
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
    style_instance * defaultCellStyle = odfContext.styleContainer().style_default_by_type(style_family::TableCell);
    style_instance * defaultColumnCellStyle = odfContext.styleContainer().style_by_name(defaultColumnCellStyleName, style_family::TableCell,false);
    style_instance * defaultRowCellStyle = odfContext.styleContainer().style_by_name(defaultRowCellStyleName, style_family::TableCell,false);
    style_instance * cellStyle = odfContext.styleContainer().style_by_name(cellStyleName, style_family::TableCell,false);

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
    
	const std::wstring formula = table_table_cell_attlist_.table_formula_.get_value_or(L"");
    const std::wstring styleName = table_table_cell_attlist_.table_style_name_.get_value_or(style_ref(L"")).style_name();

    bool skip_next_cell = false;
	size_t xfId_last_set = 0;
	bool is_style_visible = true;

    for (unsigned int r = 0; r < table_table_cell_attlist_.table_number_columns_repeated_; ++r)
    {
        Context.start_table_cell(formula,
            table_table_cell_attlist_extra_.table_number_columns_spanned_ - 1,
            table_table_cell_attlist_extra_.table_number_rows_spanned_ - 1
            );

        if (!skip_next_cell)
        {
            // вычислить стиль дл€ €чейки
            odf_read_context & odfContext = Context.root()->odf_context();    
            
			style_instance * defaultCellStyle = 
				odfContext.styleContainer().style_default_by_type(style_family::TableCell);
            style_instance * defaultColumnCellStyle = 
				odfContext.styleContainer().style_by_name(Context.get_table_context().default_column_cell_style(), style_family::TableCell,false);
            style_instance * defaultRowCellStyle = 
				odfContext.styleContainer().style_by_name(Context.get_table_context().default_row_cell_style(), style_family::TableCell,false);
            
			style_instance * cellStyle = odfContext.styleContainer().style_by_name(styleName, style_family::TableCell,false);

            std::wstring data_style = CalcCellDataStyle(Context,
                Context.get_table_context().default_column_cell_style(),
                Context.get_table_context().default_row_cell_style(),
                styleName);

            // стили не наследуютс€
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

            text_format_properties_content textFormatProperties = calc_text_properties_content(instances);
          
			paragraph_format_properties parFormatProperties = calc_paragraph_properties_content(instances);
            style_table_cell_properties_attlist cellFormatProperties = calc_table_cell_properties(instances);

            using oox::XlsxCellType;
            XlsxCellType::type t_val = XlsxCellType::s;
            std::wstring number_val = L"";
            _CP_OPT(bool) bool_val;

			int odf_value_type = office_value_type::Custom;

            if (table_table_cell_attlist_.common_value_and_type_attlist_)
            {
                const common_value_and_type_attlist & attr = *table_table_cell_attlist_.common_value_and_type_attlist_;
				if (attr.office_value_type_)
					odf_value_type = attr.office_value_type_->get_type();
				
				if ((odf_value_type == office_value_type::Float) || 
					(odf_value_type == office_value_type::Custom && attr.office_value_))
                {
                    t_val = XlsxCellType::n;
                    number_val = attr.office_value_.get_value_or(L"");
                }
                else if (odf_value_type == office_value_type::Percentage)
                {
                    t_val = XlsxCellType::n;
                    number_val = attr.office_value_.get_value_or(L"");
                }
                else if (odf_value_type == office_value_type::Currency)
                {
                    t_val = XlsxCellType::n;
                    number_val = attr.office_value_.get_value_or(L"");
                }
                else if ((odf_value_type == office_value_type::Date) || 
						 (odf_value_type == office_value_type::Custom && attr.office_date_value_))
                { 
                    t_val = XlsxCellType::n;              
                    if (attr.office_date_value_)
                    {
                        int y, m, d;
                        if (oox::parseDate(attr.office_date_value_.get(), y, m, d))
                        {
                            number_val = boost::lexical_cast<std::wstring>(oox::convertDate(y, m, d));
                        }
                    }
                }
                else if ((odf_value_type == office_value_type::Time) ||
						 (odf_value_type == office_value_type::Custom && attr.office_time_value_))
                {
                    t_val = XlsxCellType::n;
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
                    t_val = XlsxCellType::b;
                    if (attr.office_boolean_value_)
                        bool_val = oox::parseBoolVal(attr.office_boolean_value_.get());
                }
                else if ((odf_value_type == office_value_type::String) ||
						 (odf_value_type == office_value_type::Custom && attr.office_date_value_))
                {
                    t_val = XlsxCellType::s;//берем не значение а текстовые элементы
                }
            }

            std::wstring num_format = L"";
            if (data_style.empty() && !formula.empty())
            {
                cpdoccore::formulasconvert::odf2oox_converter converter;

                do {
                    std::wstring table, ref;
                    if (converter.find_first_ref(formula, table, ref))
                    {
                        if (table.empty())
                            table = Context.get_table_context().get_current_table_name();

                        std::wstring defaultColumnStyle, defaultRowStyle;
                        if (const office_element * elm = 
                            search_table_cell(Context.get_spreadsheet(), table, ref, defaultColumnStyle, defaultRowStyle))
                        {
                            if (const table_table_cell * cell = dynamic_cast<const table_table_cell *>(elm))
                            {
                                const std::wstring cellStyleStr = 
                                    cell->table_table_cell_attlist_.table_style_name_.get_value_or(style_ref(L"")).style_name();

                                data_style = CalcCellDataStyle(Context, cellStyleStr, defaultColumnStyle, defaultRowStyle);
                            }
                        }
                    }
                }
                while (0); 
            }

            if (!data_style.empty())
            {
                if (office_element_ptr num_style = odfContext.numberStyles().find_by_style_name(data_style))
                {
                    Context.get_num_format_context().start_complex_format();
                    num_style->xlsx_convert(Context);
                    Context.get_num_format_context().end_complex_format();
                    num_format = Context.get_num_format_context().get_last_format();
                }
            }


            oox::xlsx_cell_format cellFormat;
            cellFormat.set_cell_type(t_val);
            cellFormat.set_num_format(oox::odf_string_to_build_in(odf_value_type));
            
			is_style_visible = true;
            
			xfId_last_set= Context.get_style_manager().xfId(&textFormatProperties, &parFormatProperties, &cellFormatProperties, &cellFormat, num_format,false,is_style_visible);
            const int sharedStringId = table_table_cell_content_.xlsx_convert(Context);
			
            Context.set_current_cell_style_id(xfId_last_set);
			
			bool  is_data_visible = false;
			
			if (sharedStringId >= 0 || !formula.empty()	||
                (t_val == XlsxCellType::n && !number_val.empty()) || (t_val == XlsxCellType::b && !!bool_val))is_data_visible = true;

            // пустые €чейки пропускаем .
            if ( is_data_visible || (cellStyle && is_style_visible))
            {
                Context.non_empty_row();

                CP_XML_WRITER(strm)
                {
                    CP_XML_NODE(L"c")
                    {
                        CP_XML_ATTR(L"r", oox::getCellAddress(Context.current_table_column(), Context.current_table_row()));
                        CP_XML_ATTR(L"t", oox::cellType2Str(t_val));
                        CP_XML_ATTR(L"s", xfId_last_set);

                        if (!formula.empty())
                        {
                            const std::wstring xlsxFormula = formulas_converter.convert(formula);
                            if (!xlsxFormula.empty())
                            {
                                CP_XML_NODE(L"f")
                                {
                                    //CP_XML_CONTENT(xml::utils::replace_lt_gt(xlsxFormula));
                                    CP_XML_CONTENT(xlsxFormula);
                                }
                            }
                        }

                        if (sharedStringId >= 0 && t_val == XlsxCellType::s)
                        {
                            CP_XML_NODE(L"v")
                            {
                                CP_XML_CONTENT(sharedStringId);
                            }
                        }
                        else if (t_val == XlsxCellType::n && !number_val.empty())
                        {
                            CP_XML_NODE(L"v")
                            {
                                CP_XML_CONTENT(number_val);
                            }
                        }
                        else if (t_val == XlsxCellType::b && !!bool_val)
                        {
                            CP_XML_NODE(L"v")
                            {
                                CP_XML_CONTENT((int)(bool_val.get()));
                            }
                        }
                    } // c
                }
            }
            else
            {
                skip_next_cell = true;
            }

        } // if (!skip_next_cell)

        Context.end_table_cell();
    }
}

void table_covered_table_cell::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wostream & strm = Context.current_sheet().sheetData();

    for (unsigned int r = 0; r < table_table_cell_attlist_.table_number_columns_repeated_; ++r)
    {
        Context.start_table_covered_cell();
        const int xfId = Context.get_current_cell_style_id();

        CP_XML_WRITER(strm)
        {
            CP_XML_NODE(L"c")
            {
                CP_XML_ATTR(L"r", Context.current_cell_address());
                
                if (xfId >= 0)
                    CP_XML_ATTR(L"s", xfId);                    
            } // c
        }

        Context.end_table_covered_cell();
    }
}

}
}
