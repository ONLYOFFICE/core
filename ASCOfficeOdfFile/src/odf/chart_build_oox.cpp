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
#include "../docx/xlsx_textcontext.h"
#include "../docx/xlsx_num_format_context.h"

#include "serialize_elements.h"
#include <odf/odf_document.h>
#include <xml/utils.h>

#include "../formulasconvert/formulasconvert.h"

#include "style_graphic_properties.h"
#include "style_chart_properties.h"
#include "style_text_properties.h"

#include "draw_common.h"

#include "number_style.h"
#include "calcs_styles.h"
#include "chart_build_oox.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

class office_element;
typedef shared_ptr<const office_element>::Type office_element_ptr_const;

#define ACCEPT_ALL_CONTENT(VAL) \
	for (size_t ii = 0; ii < VAL.size(); ++ii) \
        { \
            if (VAL[ii]) \
                VAL[ii]->accept(*this); \
        }
        
#define ACCEPT_ALL_CONTENT_CONST(VAL) \
	for (size_t ii = 0; ii < VAL.size(); ++ii) \
        { \
            if (VAL[ii]) \
                VAL[ii]->accept(*this); \
        }


// Класс для конструирования чартов
using namespace chart;
	
void object_odf_context::set_pivot_source(std::wstring const & val)
{
	pivot_source_ = val;
	if (pivot_source_.empty() == false)
	{
		bPivotChart_ = true;
	}
}

void object_odf_context::set_width(double valPt)
{
    width_pt_ = valPt;
}

void object_odf_context::set_height(double valPt)
{
    height_pt_ = valPt;
}

void object_odf_context::set_class(std::wstring const & val)
{
    str_class_ = val;

	class_= get_series_class_type(val);
}

void object_odf_context::set_style_name(std::wstring const & val)
{
    style_name_ = val;        
}
void object_odf_context::start_axis(std::wstring const & dimensionName, std::wstring const & name, std::wstring const & styleName)
{
    in_axis_ = true;
	
	axis ax;
	ax.dimension_	= dimensionName;
	ax.chart_name_	= name;
	ax.style_name_	= styleName;

	axises_.push_back(ax);
}

void object_odf_context::end_axis()
{
    in_axis_ = false;
}

void object_odf_context::add_categories(std::wstring const & cellRange)
{
	categories_.push_back(cellRange);
}

void object_odf_context::add_grid(std::wstring const & className, std::wstring const & styleName)
{
    if (!axises_.empty())
    {
		axis::grid gr;
		gr.type_= (className == L"major" ? axis::grid::major : axis::grid::minor); // default: major
		gr.style_name_ = styleName;         
		axises_.back().grids_.push_back(gr);


    }
    else
    {
        _CP_LOG << "[warning] unexpected chart:grid" << std::endl;
    }
}
void object_odf_context::add_series(
		std::wstring const &	cellRangeAddress,
        std::wstring const &	labelCell,
        class_type				classType,
        std::wstring const &	attachedAxis,
        std::wstring const &	styleName)
{
	if (class_ == chart_ring) classType = chart_ring; 
	//if (class_ == chart_stock) classType = chart_stock; 

    series_.push_back(series(cellRangeAddress,labelCell, classType, attachedAxis, styleName));
}

void object_odf_context::add_point(unsigned int rep)
{
    if (!series_.empty())
    {
		series_.back().points_.push_back(series::point(rep));                    
    }
    else
    {
        _CP_LOG << "[warning] unexpected chart:data-point" << std::endl;
    }        
}


void object_odf_context::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (object_type_ == 1) 
	{
		if (pivot_source_.empty() == false)
		{
			pivot_source_ = Context.get_pivots_context().get_chart_source(pivot_source_);

		}
		Context.start_chart(L"");
		oox::oox_chart_context & chart = Context.current_chart();
		
		oox_convert(chart);

		Context.end_chart();
	}
	else if (object_type_ == 2 && office_text_)
	{
		//embedded
	}
	else if (object_type_ == 3 && office_math_)
	{
		Context.get_math_context().base_font_size_ = baseFontHeight_;	
		Context.get_math_context().start();
		office_math_->oox_convert(Context.get_math_context());
	}
	else if(object_type_ == 4 && office_spreadsheet_)
	{
		//embedded
	}
}
void object_odf_context::docx_convert(oox::docx_conversion_context & Context)
{
	if (object_type_ == 1) 
	{
		Context.start_chart(L"");
		oox::oox_chart_context & chart_context = Context.current_chart();

		oox_convert(chart_context);

		if (false == embeddedData.empty())
		{
			chart_context.set_externalData(embeddedData);
		}
		else
		{
			chart_context.set_cache_only(true);
		}

		if (false == userShapes.first.empty())
		{
			chart_context.set_userShapes(userShapes);
		}

		Context.end_chart();
	}
	else if (object_type_ == 2 && office_text_)
	{
		//embedded
	}
	else if (object_type_ == 3 && office_math_)
	{
		oox::StreamsManPtr prev = Context.get_stream_man();
		
		std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_frame());
		Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));	
		
		Context.reset_context_state();

		Context.get_math_context().base_font_size_ = baseFontHeight_;	
		
		Context.start_math_formula();
			office_math_->oox_convert(Context.get_math_context());
		Context.end_math_formula();

		Context.get_drawing_context().get_text_stream_frame() = temp_stream.str();
		
		Context.set_stream_man(prev);
		
		Context.back_context_state();	
	}
	else if(object_type_ == 4 && office_spreadsheet_)
	{
		//embedded
	}
}
void object_odf_context::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (object_type_ == 1) 
	{
		Context.start_chart(L"");
		oox::oox_chart_context & chart_context = Context.current_chart();
		
		oox_convert(chart_context);

		if (embeddedData.empty())
		{
			chart_context.set_cache_only(true);
		}
		else
		{
			chart_context.set_externalData(embeddedData);
		}
		Context.end_chart();
	}
	else if (object_type_ == 2 && office_text_)
	{
		//embedded
	}
	else if (object_type_ == 3 && office_math_)
	{
		Context.get_math_context().base_font_size_ = baseFontHeight_;	
		Context.get_math_context().start();
		office_math_->oox_convert(Context.get_math_context());
	}
	else if(object_type_ == 4 && office_spreadsheet_)
	{
		//embedded
	}
}
void object_odf_context::calc_cache_series(std::wstring adress, std::vector<std::wstring> & cash)
{
	if (adress.empty()) return;

	if (bPivotChart_)
	{
		std::map<std::wstring, _cell>::iterator pFind = cash_pivot.find(adress);
		if (pFind != cash_pivot.end())
		{
			int col = pFind->second.col;
			int row = pFind->second.row;
 			
			if (col == 0 || row == 0)
			{
				for (size_t i = 0; i < cash_values.size(); i++)
				{
					if (col == 0)
					{
						if (cash_values[i].col == col)
						{
							cash.push_back(cash_values[i].val);
						}
					}
					if (row == 0)
					{
						if (cash_values[i].row == row)
						{
							cash.push_back(cash_values[i].val);
						}
					}
				}		
			}
			else
			{
				cash.push_back(pFind->second.val);
			}
		}
	}
	else
	{
		formulasconvert::odf2oox_converter converter;

		std::wstring ref_1, ref_2, table;
		size_t col_1 = 0, row_1 = 0, col_2 = 0, row_2 = 0;
	   
		if (!converter.find_first_last_ref(adress, table, ref_1, ref_2))return;
		//if ((res = table.find(L"local-table"))<0)return;

		oox::getCellAddressInv(ref_1, col_1, row_1);
		oox::getCellAddressInv(ref_2, col_2, row_2);

 		for (size_t i = 0; i < cash_values.size(); i++)
		{
			if (cash_values[i].col >= col_1 && cash_values[i].col <= col_2 &&
				cash_values[i].row >= row_1 && cash_values[i].row <= row_2)
			{
				cash.push_back(cash_values[i].val);
			}
		}
	}
}
//----------------------------------------------------------------------------------------
struct axises_sort
{
    inline bool operator() (const chart::axis& a1, const chart::axis& a2)
    {
        return (a1.type_ < a2.type_);
    }
};

void object_odf_context::oox_convert(oox::oox_chart_context & chart_context)
{
	chart_context.set_pivot_chart	(pivot_source_);

	chart_context.set_title		(title_);
	chart_context.set_wall		(wall_);
	chart_context.set_floor		(floor_);
	chart_context.set_legend	(legend_);
	
	chart_context.set_plot_area_properties		(plot_area_.properties_, plot_area_.properties_3d_, plot_area_.fill_);
	chart_context.set_chart_graphic_properties	(chart_graphic_properties_, chart_fill_);
	
	//chart_context.set_footer(footer_);
	//chart_context.set_chart_properties(chart_graphic_properties_);

	class_type last_set_type = chart_unknown; 

	int series_id =0;

	if (series_.empty())
	{
		chart_context.add_chart(class_);
	}

 	for (size_t i = 0; i < series_.size(); i++)
	{
		if (series_[i].class_ != last_set_type)			//разные типы серий в диаграмме - например бар и линия.
		{
			chart_context.add_chart(series_[i].class_);
			last_set_type = series_[i].class_;
		}
		oox::oox_chart_ptr current = chart_context.get_current_chart();

		if (!current) continue;

		if (1 == series_.size())
		{
			current->varyColors_ = false;
		}

		current->set_properties(plot_area_.properties_);
		current->set_additional_properties(chart_graphic_properties_);
	
		current->add_series(series_id++);
		
		if (series_[i].cell_range_address_.empty() ) 
			series_[i].cell_range_address_ = plot_area_.cell_range_address_; //SplitByColumn	(ind_ser,range);
																	//SplitByRow	(ind_ser,range);
		if (series_[i].cell_range_address_.empty())
			series_[i].cell_range_address_ = domain_cell_range_adress2_;
		
		//тут данные нужно поделить по столбцам или строкам - так как в плот-ареа общий диапазон
		//первый столбец-строка МОЖЕт использоваться для подписей
		//каждая серия берет каждый последующий диапазрн
		//такое определение - редкость = todooo
		
		std::vector<std::wstring>		domain_cash;
		std::vector<std::wstring>		cell_cash;
		std::vector<std::wstring>		cat_cash;
		std::vector<std::wstring>		label_cash;

		calc_cache_series (domain_cell_range_adress_,			domain_cash);
		calc_cache_series (series_[i].cell_range_address_,		cell_cash);
		calc_cache_series (series_[i].label_cell_address_,		label_cash);
		
		if (false == categories_.empty())
			calc_cache_series (categories_[0],	cat_cash);

		std::wstring			formatCode	= L"General";
		_CP_OPT(std::wstring)	strVal;
		_CP_OPT(bool)			boolVal;
		
		odf_reader::GetProperty(series_[i].properties_, L"num_format", strVal);
		odf_reader::GetProperty(series_[i].properties_, L"link-data-style-to-source", boolVal);

		if ((strVal) && (strVal->length() > 1))
		{
			formatCode = *strVal;
		}

		if (false == series_[i].label_cell_address_.empty())
		{
			current->set_label_series(series_[i].label_cell_address_, label_cash);//_oox_strRef  
		}
		
		if (domain_cell_range_adress_.empty() == false || 
			last_set_type == chart_scatter) 
		{
			if (last_set_type == chart_bubble)
			{	//bubble(x)
				if (!bPivotChart_)
					current->set_formula_series(4, domain_cell_range_adress_, formatCode, boolVal.get_value_or(true));	
				current->set_values_series (4, domain_cash);
				//y	
				if (!bPivotChart_)
					current->set_formula_series(3, series_[i].cell_range_address_, formatCode, boolVal.get_value_or(true));			
				current->set_values_series (3, cell_cash);
			}
			else
			{	//x
				if (false == domain_cash.empty() || false == cash_values.empty())
				{
					if (!bPivotChart_)
						current->set_formula_series(2, domain_cell_range_adress_, formatCode, boolVal.get_value_or(true));	
					current->set_values_series (2, domain_cash);		
				}
				//y
				if (false == cell_cash.empty() || false == cash_values.empty())
				{
					if (!bPivotChart_)
						current->set_formula_series(3, series_[i].cell_range_address_, formatCode, boolVal.get_value_or(true));				
					current->set_values_series (3, cell_cash);		
				}
			}
		}
		else
		{	//common
			if (!bPivotChart_)
				current->set_formula_series(1, series_[i].cell_range_address_, formatCode, boolVal.get_value_or(true));	
			current->set_values_series(1, cell_cash);
		}

		if (categories_.empty() == false)//названия
		{			
			if (!bPivotChart_)
				current->set_formula_series(0, categories_[0], L"General", true);
			current->set_values_series(0, cat_cash);
		}
		current->set_name(series_[i].name_);
		
		current->set_content_series(series_[i]);
	}

	std::sort(axises_.begin(), axises_.end(), axises_sort());//file_1_ (1).odp
	
	bool x_enabled	= false;
	bool y_enabled	= false;
	bool z_enabled	= false;
	bool is3D		= false;
	
	_CP_OPT(bool) boolVal;
	odf_reader::GetProperty(plot_area_.properties_, L"three-dimensional", boolVal);

	if ((boolVal) && (*boolVal)) 
	{
		is3D = true;
	}

	for (size_t i = 0; i < axises_.size(); i++)
	{
		axis & a  = axises_[i];

		if	(a.dimension_ == L"x" && x_enabled)continue;
		if	(a.dimension_ == L"y" && y_enabled)continue;
		if	(a.dimension_ == L"z" && z_enabled)continue;

		if	(a.dimension_ == L"x")//могут быть типы 1, 2, 3, 4
		{			
			if (last_set_type == chart_scatter ||
					last_set_type == chart_bubble) a.type_ = 2;

			if (class_ == chart_stock && a.type_ == 3 )		
				a.type_ = 4; //шкала дат.

			if (is3D) a.type_ = 1; // шкала категорий
			
			x_enabled = true;
		}
		else if (a.dimension_ == L"y")
		{
			a.type_ = 2;
			if (last_set_type ==  chart_bar)
			{
				//вот нахрена свойства относящиеся к серии и самому чарту воткнули в оси ???? (ооо писали идиеты???)
				//или это банальная ошибка которую так никогда и не исправили???
				//overlap & gap-width
				oox::oox_chart_ptr current = chart_context.get_current_chart();
				current->set_additional_properties(a.properties_);
			}
			y_enabled = true;
		}
		else if (a.dimension_ == L"z")
		{
			is3D = true;
			continue;
			a.type_ = 2;
			z_enabled = true;
		}

		chart_context.add_axis(a.type_, a);
	}

	if (is3D)
	{
		if (!z_enabled)
		{
			chart::axis a;
			a.type_ = 0;	// blank

			chart_context.add_axis(a.type_, a);
		}
		chart_context.set_3D_chart (true);
	}
}

//----------------------------------------------------------------------------------------
process_build_object::process_build_object(object_odf_context & object_odf, odf_read_context & context) :	
						 stop_				(false)
						,object_odf_context_(object_odf)
						,styles_			(context.styleContainer())
						,settings_			(context.Settings())
						,draw_styles_		(context.drawStyles())
						,number_styles_		(context.numberStyles())
						,num_format_context_(context)
{
	_CP_OPT(std::wstring) sFontHeight	= settings_.find_by_name(L"BaseFontHeight");
	
	if (sFontHeight)
	{
		try
		{
			object_odf_context_.baseFontHeight_ =  boost::lexical_cast<int>(*sFontHeight);
		}
		catch(...)
		{
		}
	}
}
void process_build_object::ApplyChartProperties(std::wstring style, std::vector<_property> & propertiesOut)
{
	style_instance* styleInst = styles_.style_by_name(style, odf_types::style_family::Chart, false);
    if(styleInst)
	{
		const style_content * Content				= styleInst->content();
		const style_chart_properties *properties	= Content->get_style_chart_properties();

		std::wstring data_style_name = styleInst->data_style_name();
		
		if (!data_style_name.empty())
		{
			office_element_ptr elm = number_styles_.find_by_style_name(data_style_name);
			number_style_base *number_style = dynamic_cast<number_style_base*>(elm.get());

			if (number_style)
			{
				num_format_context_.start_complex_format();
				number_style->oox_convert(num_format_context_);
				num_format_context_.end_complex_format();
				
				std::wstring num_format = num_format_context_.get_last_format();
 				
				_property p(L"num_format", num_format); 
				propertiesOut.push_back(p);		
			}
		}

		if (!properties)return;

 		for (size_t i = 0; i < properties->content_.size(); i++)
		{
			propertiesOut.push_back(properties->content_[i]);
		}
    }
}
void process_build_object::ApplyTextProperties(std::wstring style, std::vector<_property> & propertiesOut)
{
	style_instance* styleInst = styles_.style_by_name(style, odf_types::style_family::Chart, false/*Context.process_headers_footers_*/);
    if(styleInst)
	{
		text_format_properties_content properties = calc_text_properties_content(styleInst);
		properties.apply_to(propertiesOut);
    }
}
void process_build_object::ApplyGraphicProperties(std::wstring style, std::vector<_property> & propertiesOut, oox::_oox_fill & fill)
{
	style_instance* styleInst = styles_.style_by_name(style, odf_types::style_family::Chart, false/*Context.process_headers_footers_*/);
    if(styleInst)
	{
		graphic_format_properties properties = calc_graphic_properties_content(styleInst);

		Compute_GraphicFill(properties.common_draw_fill_attlist_, properties.style_background_image_ , draw_styles_ , fill);

		if (fill.bitmap)
		{
			fill.bitmap->xlink_href_ = object_odf_context_.baseRef_ + FILE_SEPARATOR_STR + fill.bitmap->xlink_href_;
		}
		properties.apply_to(propertiesOut);
    }
}	

///////////////////////////////////////
bool process_build_object::visit_table(std::wstring const & name)
{
    object_odf_context_.table_name_ = name;
    return true;
}

void process_build_object::visit_column(unsigned int repeated)
{
    object_odf_context_.columns_count_ += repeated;
    object_odf_context_.columns_.push_back(repeated);    
}
bool process_build_object::visit_rows(unsigned int repeated)
{
    object_odf_context_.current_table_column_ = 0;
    object_odf_context_.current_table_row_ += repeated;

    return true;
}
//////////////////////////////////////////////////
void process_build_object::on_not_impl(std::string const & message)
{
    _CP_LOG << L"[process_object visitor] : not impliment for \"" << utf8_to_utf16(message) << L"\"" << std::endl;
}

//-----------------------------------------------------------------------------------
void process_build_object::visit(office_document & val)
{
   if (val.office_body_)
        val.office_body_->accept(*this);
}

void process_build_object::visit(const office_document_content& val)
{
    if (val.office_body_)
        val.office_body_->accept(*this);
}
void process_build_object::visit(office_document_content& val)
{
    if (val.office_body_)
        val.office_body_->accept(*this);
}
void process_build_object::visit(office_body& val)
{
    if (val.content_)
        val.content_->accept(*this);
}
void process_build_object::visit(office_chart& val)
{
	ACCEPT_ALL_CONTENT(val.content_);
}
void process_build_object::visit(office_text& val)
{
	object_odf_context_.object_type_ = 2;
	object_odf_context_.office_text_ = &val;	//конвертация будет уровнем выше
}
void process_build_object::visit(office_math& val)
{
	object_odf_context_.object_type_ = 3;		//= 0 - временно замещающая картинка
	object_odf_context_.office_math_ = &val;	//конвертация будет уровнем выше
}
void process_build_object::visit(office_spreadsheet& val)
{
	object_odf_context_.object_type_		= 4;	
	object_odf_context_.office_spreadsheet_ = &val;	//конвертация будет уровнем выше
}
void process_build_object::visit(chart_chart& val)
{
	object_odf_context_.object_type_ = 1;
	
	if (val.attlist_.common_draw_size_attlist_.svg_width_)
    {
        object_odf_context_.set_width(val.attlist_.common_draw_size_attlist_.svg_width_->get_value_unit(length::pt));
    }
           
    if (val.attlist_.common_draw_size_attlist_.svg_height_)
    {
        object_odf_context_.set_height(val.attlist_.common_draw_size_attlist_.svg_height_->get_value_unit(length::pt));
    }
	ApplyGraphicProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.chart_graphic_properties_, object_odf_context_.chart_fill_);

	object_odf_context_.set_class(val.attlist_.chart_class_);

	if (val.attlist_.loext_data_pilot_source_)
		object_odf_context_.set_pivot_source(*val.attlist_.loext_data_pilot_source_);

	ACCEPT_ALL_CONTENT(val.content_);

}

void process_build_object::visit(chart_title& val)
{
	title t;

	if (val.text_p_)
	{
		std::wstringstream v;
		val.text_p_->text_to_stream(v, false);
		t.content_ = v.str();
	}
	ApplyTextProperties(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""), t.text_properties_);

///////////////////////////////////////////////////////////////////////////////////////
	if (val.attlist_.common_draw_position_attlist_.svg_x_)
    {
        t.pos_x = val.attlist_.common_draw_position_attlist_.svg_x_->get_value_unit(length::pt);
	}
	if (val.attlist_.common_draw_position_attlist_.svg_y_)
	{
		t.pos_y = val.attlist_.common_draw_position_attlist_.svg_y_->get_value_unit(length::pt);
	}
	t.bEnabled = true;
	
	if (object_odf_context_.in_axis_)
		object_odf_context_.axises_.back().title_ = t; 
	else			
		object_odf_context_.title_ = t;	

}

void process_build_object::visit(chart_subtitle & val)
{
	title t;
	std::wstringstream v;
	val.text_p_->text_to_stream(v, false);
	t.content_ = v.str();

	if (val.attlist_.common_draw_position_attlist_.svg_x_)
    {
        t.pos_x = val.attlist_.common_draw_position_attlist_.svg_x_->get_value_unit(length::pt);
		t.pos_y = val.attlist_.common_draw_position_attlist_.svg_y_->get_value_unit(length::pt);
	}
	t.bEnabled = true;
	object_odf_context_.sub_title_ = t;
}

void process_build_object::visit(chart_footer& val)
{
	object_odf_context_.footer_.bEnabled = true;

	ApplyChartProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.footer_.properties_);
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.footer_.graphic_properties_, object_odf_context_.footer_.fill_);
	ApplyTextProperties		(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.footer_.text_properties_);
}

void process_build_object::visit(chart_legend& val)
{
	object_odf_context_.legend_.bEnabled = true;
	object_odf_context_.legend_.position = L"r"; 

	if (val.attlist_.chart_legend_position_)
	{
		std::wstring pos = val.attlist_.chart_legend_position_.get();
		
		if ( pos == L"bottom")		object_odf_context_.legend_.position = L"b"; 
		if ( pos == L"start")		object_odf_context_.legend_.position = L"l"; 
		if ( pos == L"top")			object_odf_context_.legend_.position = L"t"; 
		if ( pos == L"top-end")		object_odf_context_.legend_.position = L"tr"; 
		if ( pos == L"top-start")	object_odf_context_.legend_.position = L"tl"; 
		if ( pos == L"bottom-start")	object_odf_context_.legend_.position = L"bl"; 
		if ( pos == L"bottom-end")	object_odf_context_.legend_.position = L"br"; 
	}
	if (val.attlist_.chart_legend_align_)
	{
		std::wstring align = val.attlist_.chart_legend_align_.get();
		
		//if ( pos == L"start")		object_odf_context_.legend_.align = L"b"; 
		//if ( pos == L"center")		object_odf_context_.legend_.align = L"l"; 
		//if ( pos == L"end")			object_odf_context_.legend_.align = L"t"; 
	}
	
	ApplyChartProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.legend_.properties_);
	ApplyGraphicProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.legend_.graphic_properties_,object_odf_context_.legend_.fill_);
	ApplyTextProperties		(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.legend_.text_properties_);
}

void process_build_object::visit(chart_plot_area& val)
{
	ACCEPT_ALL_CONTENT(val.content_);

	object_odf_context_.plot_area_.cell_range_address_ = val.attlist_.table_cell_range_address_.get_value_or(L"");

	odf_types::common_dr3d_attlist attr_3d = val.attlist_.common_dr3d_attlist_;

	if (attr_3d.transform_)		object_odf_context_.plot_area_.properties_3d_.push_back(_property(L"transform",	attr_3d.transform_.get()) );
	if (attr_3d.distance_)		object_odf_context_.plot_area_.properties_3d_.push_back(_property(L"distance",		attr_3d.distance_->get_value_unit(length::pt)) );
	if (attr_3d.focal_length_)	object_odf_context_.plot_area_.properties_3d_.push_back(_property(L"focal",		attr_3d.focal_length_->get_value_unit(length::pt)) );
	
	ApplyChartProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""), object_odf_context_.plot_area_.properties_);
	ApplyGraphicProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""), object_odf_context_.plot_area_.graphic_properties_, object_odf_context_.plot_area_.fill_);
	ApplyTextProperties		(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""), object_odf_context_.plot_area_.text_properties_);
}


void process_build_object::visit(chart_axis& val)
{
    object_odf_context_.start_axis(val.attlist_.chart_dimension_.get_value_or(L""),
							val.attlist_.chart_name_.get_value_or(L""),
							val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""));

    ACCEPT_ALL_CONTENT(val.content_);

	std::wstring style_name	= val.attlist_.common_attlist_.chart_style_name_.get_value_or(L"");

	ApplyChartProperties	(style_name, object_odf_context_.axises_.back().properties_);
	ApplyGraphicProperties	(style_name, object_odf_context_.axises_.back().graphic_properties_, object_odf_context_.axises_.back().fill_);
	ApplyTextProperties		(style_name, object_odf_context_.axises_.back().text_properties_);

    object_odf_context_.end_axis();        
}

void process_build_object::visit(chart_series& val)
{
    const chart_series_attlist & att = val.attlist_;
   
	chart::class_type chartClass = get_series_class_type(att.chart_class_.get_value_or(object_odf_context_.str_class_));

    object_odf_context_.add_series(
							att.chart_values_cell_range_address_.get_value_or(L""),
							att.chart_label_cell_address_.get_value_or(L""),
							chartClass,
							att.chart_attached_axis_.get_value_or(L""),
							att.common_attlist_.chart_style_name_.get_value_or(L"")
        );
	
	ACCEPT_ALL_CONTENT(val.content_);

	ApplyChartProperties	(att.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().properties_);
	ApplyGraphicProperties	(att.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().graphic_properties_,object_odf_context_.series_.back().fill_);
	ApplyTextProperties		(att.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().text_properties_);

}

void process_build_object::visit(chart_domain& val)
{
	if (object_odf_context_.domain_cell_range_adress_.empty())	
		object_odf_context_.domain_cell_range_adress_	= val.table_cell_range_address_.get_value_or(L"");
	else
		object_odf_context_.domain_cell_range_adress2_ = val.table_cell_range_address_.get_value_or(L"");
}
void process_build_object::visit(chart_grid& val)
{
    object_odf_context_.add_grid(val.attlist_.chart_class_.get_value_or(L""),
        val.attlist_.common_attlist_.chart_style_name_.get_value_or(L"") );
	
	oox::_oox_fill fill;
	
	ApplyGraphicProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	(object_odf_context_.axises_.back()).grids_.back().graphic_properties_, fill);

 }
void process_build_object::visit(chart_wall& val)
{      
	object_odf_context_.wall_.bEnabled = true;

	ApplyChartProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.wall_.properties_);
	ApplyGraphicProperties	(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.wall_.graphic_properties_,object_odf_context_.wall_.fill_);
	ApplyTextProperties		(val.attlist_.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.wall_.text_properties_);
} 

void process_build_object::visit(chart_floor& val)
{   
	object_odf_context_.floor_.bEnabled = true;

	ApplyChartProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.floor_.properties_);
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.floor_.graphic_properties_,object_odf_context_.floor_.fill_);
	ApplyTextProperties		(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.floor_.text_properties_);
} 

void process_build_object::visit(chart_data_point & val)
{
	object_odf_context_.add_point(	val.attlist_.chart_repeated_.get_value_or(1));

	if (val.attlist_.common_attlist_.chart_style_name_)
	{
		object_odf_context_.series_.back().points_.back().bEnabled = true;
		std::wstring style_name = val.attlist_.common_attlist_.chart_style_name_.get_value_or(L"");
		
		ApplyGraphicProperties	(style_name,	object_odf_context_.series_.back().points_.back().graphic_properties_, 
												object_odf_context_.series_.back().points_.back().fill_);
		ApplyTextProperties		(style_name,	object_odf_context_.series_.back().points_.back().text_properties_);
	}

}
void process_build_object::visit(chart_mean_value & val)
{
	ApplyChartProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().mean_value_.properties_);
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().mean_value_.graphic_properties_, object_odf_context_.series_.back().mean_value_.fill_);
}
void process_build_object::visit(chart_error_indicator & val)
{
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().error_indicator_.graphic_properties_,object_odf_context_.series_.back().error_indicator_.fill_ );
}	
void process_build_object::visit(chart_regression_curve & val)
{
	oox::_oox_fill fill;
	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().regression_curve_.line_properties_, fill);

	if (val.chart_equation_)
	{
		object_odf_context_.series_.back().regression_curve_.bEquation = true;

		val.chart_equation_->accept(*this);
	}
}
void process_build_object::visit(chart_equation & val)
{
	if (object_odf_context_.series_.back().regression_curve_.bEquation == false)return;
	
	
	if (val.display_r_square_)
		object_odf_context_.series_.back().regression_curve_.bREquation = val.display_r_square_.get();

	ApplyGraphicProperties	(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().regression_curve_.equation_properties_.graphic_properties_,object_odf_context_.series_.back().regression_curve_.equation_properties_.fill_);
	ApplyTextProperties		(val.common_attlist_.chart_style_name_.get_value_or(L""),	object_odf_context_.series_.back().regression_curve_.equation_properties_.text_properties_);

}
void process_build_object::visit(chart_categories& val)
{     
	if (object_odf_context_.in_axis_)
	{
		object_odf_context_.axises_.back().type_ = 1;
		object_odf_context_.axises_.back().bCategories_ = true;
	}

	if (val.table_cell_range_address_) 
		object_odf_context_.add_categories(*val.table_cell_range_address_);
}    
void process_build_object::visit(table_table& val)
{        
	object_odf_context_.table_table_ = &val;	
	
	ACCEPT_ALL_CONTENT(val.table_columns_and_groups_.content_);
	ACCEPT_ALL_CONTENT(val.table_rows_and_groups_.content_);
}   
void process_build_object::visit(table_table_rows& val)
{        
    ACCEPT_ALL_CONTENT(val.table_table_row_);
}

void process_build_object::visit(table_table_row & val)
{       
    unsigned int repeated = val.attlist_.table_number_rows_repeated_;
    ACCEPT_ALL_CONTENT(val.content_);
	visit_rows(repeated);
}
void process_build_object::visit(table_table_column& val)
{
    const unsigned int columnsRepeated = val.table_table_column_attlist_.table_number_columns_repeated_;
  
	visit_column(columnsRepeated);
}
void process_build_object::visit(table_table_row_group& val)
{
    ACCEPT_ALL_CONTENT(val.table_rows_and_groups_.content_);
}
void process_build_object::visit(table_table_column_group& val)
{
    ACCEPT_ALL_CONTENT(val.table_columns_and_groups_.content_);
}
void process_build_object::visit(table_table_columns& val)
{
    ACCEPT_ALL_CONTENT(val.table_table_column_);
}
void process_build_object::visit(table_columns_no_group& val)
{
    if (val.table_columns_1_.table_table_columns_)
        val.table_columns_1_.table_table_columns_->accept(*this);

    ACCEPT_ALL_CONTENT(val.table_columns_1_.table_table_column_);

    if (val.table_table_header_columns_)
        val.table_table_header_columns_->accept(*this);

    if (val.table_columns_2_.table_table_columns_)
        val.table_columns_2_.table_table_columns_->accept(*this);

    ACCEPT_ALL_CONTENT(val.table_columns_2_.table_table_column_);
}
void process_build_object::visit(table_rows_no_group& val)
{
 	if (val.table_rows_1_.table_table_rows_)
        val.table_rows_1_.table_table_rows_->accept(*this);
	else
		ACCEPT_ALL_CONTENT(val.table_rows_1_.table_table_row_);
  
	if (val.table_table_header_rows_)
        val.table_table_header_rows_->accept(*this);
   
 	if (val.table_rows_2_.table_table_rows_)
        val.table_rows_2_.table_table_rows_->accept(*this);
	else
		ACCEPT_ALL_CONTENT(val.table_rows_2_.table_table_row_);
}
void process_build_object::visit(table_table_cell& val)
{
	const table_table_cell_attlist & attlist = val.attlist_;

    unsigned int repeated = val.attlist_.table_number_columns_repeated_;
  
	std::wstring cell_val, cell_cash, cell_desc;
	
	for (size_t i = 0 ; i < val.content_.elements_.size(); i++)
	{
		std::wstringstream  wstream_temp;	
		val.content_.elements_[i]->text_to_stream(wstream_temp, false);
		
		if (val.content_.elements_[i]->get_type() == typeTextP)
		{
			cell_cash += wstream_temp.str();
		}
		else if (val.content_.elements_[i]->get_type() == typeDrawG)
		{
			cell_desc += wstream_temp.str();
		}
	}   

	if (attlist.common_value_and_type_attlist_.office_value_) 				cell_val = *attlist.common_value_and_type_attlist_.office_value_;
	else if (attlist.common_value_and_type_attlist_.office_currency_)		cell_val = *attlist.common_value_and_type_attlist_.office_currency_;
	else if (attlist.common_value_and_type_attlist_.office_date_value_)		cell_val = *attlist.common_value_and_type_attlist_.office_date_value_;
	else if (attlist.common_value_and_type_attlist_.office_time_value_)		cell_val = *attlist.common_value_and_type_attlist_.office_time_value_;
	else if (attlist.common_value_and_type_attlist_.office_boolean_value_)	cell_val = *attlist.common_value_and_type_attlist_.office_boolean_value_;
	else if (attlist.common_value_and_type_attlist_.office_string_value_)	cell_val = *attlist.common_value_and_type_attlist_.office_string_value_;

	if (cell_cash.empty())
		cell_cash = cell_val;

    object_odf_context::_cell cell_= {(size_t)object_odf_context_.current_table_column_, (size_t)object_odf_context_.current_table_row_, cell_cash};

	object_odf_context_.cash_values.push_back(cell_);
	
	object_odf_context_.current_table_column_+=repeated;

	if (object_odf_context_.bPivotChart_)
	{
		object_odf_context_.cash_pivot.insert(std::make_pair(cell_desc, cell_));
	}
}
void process_build_object::visit(table_covered_table_cell& val)
{
    unsigned int repeated = val.attlist_.table_number_columns_repeated_;

	if ( repeated <2) 
		return;
}
void process_build_object::visit(table_table_header_columns& val)
{
    ACCEPT_ALL_CONTENT(val.table_table_column_);
}
void process_build_object::visit(table_table_header_rows& val)
{        
    ACCEPT_ALL_CONTENT(val.table_table_row_);
}
}
}
