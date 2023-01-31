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
#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/regex.hpp>
//
#include "../../Common/CPString.h"
#include <xml/xmlchar.h>

#include "visitor.h"

#include "office_document.h"
#include "office_body.h"
#include "office_chart.h"
#include "office_text.h"
#include "office_spreadsheet.h"
#include "math_elements.h"
#include "table.h"
#include "odfcontext.h"

#include "chart_objects.h"

namespace cpdoccore { 
namespace odf_reader {

class object_odf_context 
{
public:
	struct _cell
	{
		size_t col;
		size_t row;
		std::wstring val;
	};

	object_odf_context(std::wstring ref) 
	: 
		width_pt_				(0), 
		height_pt_				(0), 
		in_axis_				(false),
		bPivotChart_			(false),
        current_table_column_	(0),
        current_table_row_		(0),
        columns_spanned_num_	(0),
		columns_count_			(0),
		object_type_			(0),
		office_text_			(NULL),
		office_math_			(NULL),
		office_spreadsheet_		(NULL),
		table_table_			(NULL),
		baseRef_				(ref),
		baseFontHeight_			(12)
    {
	}
	
	void set_pivot_source(std::wstring const & val);

	void set_width(double valPt);

    void set_height(double valPt);

    void set_class(odf_types::chart_class::type type);

    void set_style_name(std::wstring const & val);

    void start_axis(std::wstring const & dimensionName, std::wstring const & name, std::wstring const & styleName);

    void end_axis();

	void add_categories(std::wstring const & cellRangeAddress);

    void add_grid(std::wstring const & gridClassName, std::wstring const & styleName);
    void add_series(std::wstring const & cellRangeAddress,
            std::wstring const & labelCell,
			odf_types::chart_class::type classType,
            std::wstring const & attachedAxis,
            std::wstring const & styleName);

    void add_point(unsigned int rep);
	
    void calc_cache_series(std::wstring adress, std::vector<std::wstring> & cash);
	
	void xlsx_convert	(oox::xlsx_conversion_context & Context);
	void docx_convert	(oox::docx_conversion_context & Context);
	void pptx_convert	(oox::pptx_conversion_context & Context);
	void oox_convert	(oox::oox_chart_context & chart);

    double						width_pt_;
    double						height_pt_;

	int							object_type_;
	office_text					*office_text_;
 	office_math					*office_math_;
 	office_spreadsheet			*office_spreadsheet_;
	table_table					*table_table_;

	int							baseFontHeight_;
	std::wstring				baseRef_; 
//---------------------------------------------------------------
	odf_types::chart_class::type class_;
	std::wstring				style_name_;
 	std::wstring				name_;
  
	std::wstring				pivot_source_;
	bool						bPivotChart_;

	bool in_axis_;
    std::vector<chart::axis>	axises_;
    std::vector<chart::series>	series_;
	std::vector<std::wstring>	categories_;

	std::wstring domain_cell_range_adress_;
	std::wstring domain_cell_range_adress2_;

	chart::title				title_;

	chart::title				sub_title_;
	chart::legend				legend_;
	chart::plot_area			plot_area_;

	chart::simple				wall_;
	chart::simple				floor_;
	
	chart::simple				footer_;

	chart::simple				stock_gain_marker_;
	chart::simple				stock_loss_marker_;
	chart::simple				stock_range_line_;

	std::vector<_property>		chart_properties_;
	std::vector<_property>		chart_graphic_properties_;
	oox::_oox_fill				chart_fill_;

	std::vector<_cell>			cash_values;
	std::map<std::wstring, _cell>cash_pivot;

	std::wstring				embeddedData;
	std::pair<std::wstring, std::wstring> userShapes;
//---------------------------------------
	std::wstring				target_table_;
	std::wstring				table_name_;
   
	int							current_table_column_;
    int							current_table_row_;

	std::vector<unsigned int>	columns_;
    unsigned int				columns_count_;

	unsigned int						columns_spanned_num_;
    std::vector<oox::xlsx_row_spanned>	rows_spanned_;
   

///////////////////////////////////////////

};

class process_build_object 
	:	public base_visitor,
		public visitor<office_document>,

		public const_visitor<office_document_content>,
 		public visitor<office_document_content>,
	   
		public visitor<office_body>,
		public visitor<office_chart>,    
		public visitor<office_text>,
		public visitor<office_math>,
		public visitor<office_spreadsheet>,

		public visitor<chart_chart>,

		public visitor<chart_title>,
		public visitor<chart_subtitle>,
		public visitor<chart_footer>,
		public visitor<chart_legend>,

		public visitor<chart_plot_area>,
	    
		public visitor<chart_axis>,
		public visitor<chart_categories>,
		public visitor<chart_date_scale>,
		public visitor<chart_grid>,

		public visitor<chart_series>,
		public visitor<chart_domain>,
		public visitor<chart_data_point>,
		public visitor<chart_mean_value>,
		public visitor<chart_regression_curve>,
		public visitor<chart_equation>,
		public visitor<chart_error_indicator>,
		public visitor<chart_stock_range_line>,
		public visitor<chart_stock_loss_marker>,
		public visitor<chart_stock_gain_marker>,
		public visitor<chart_wall>,
		public visitor<chart_floor>,

		public visitor<table_table>,
	    
		public visitor<table_table_row_group>,
		public visitor<table_rows_no_group>,
		public visitor<table_table_header_rows>,
		public visitor<table_table_rows>,
		public visitor<table_table_row>,

		public visitor<table_table_cell>,
		public visitor<table_covered_table_cell>,    

		public visitor<table_table_column_group>,
		public visitor<table_table_header_columns>,
		public visitor<table_table_columns>,
		public visitor<table_table_column>,
		public visitor<table_columns_no_group>
{
public:

	process_build_object(object_odf_context & object_context, odf_read_context & context);

private:
	void ApplyChartProperties(std::wstring style, std::vector<_property> & propertiesOut);
	void ApplyGraphicProperties(std::wstring style, std::vector<_property> & propertiesOut, oox::_oox_fill & fill);

	void ApplyTextProperties(std::wstring style, text_format_properties_content_ptr & propertiesOut);
	
	bool visit_table(std::wstring const & name);
    void visit_column(unsigned int repeated);
    bool visit_rows(unsigned int repeated);
	virtual void on_not_impl(std::string const & message);

public:
	virtual void visit(office_document				& val);
	
	virtual void visit(const office_document_content& val);
	virtual void visit(office_document_content		& val);
	
	virtual void visit(office_body					& val);
    virtual void visit(office_chart					& val);
    virtual void visit(office_text					& val);   
	virtual void visit(office_math					& val);
    virtual void visit(office_spreadsheet			& val);   

	virtual void visit(chart_chart			& val);
	virtual void visit(chart_title			& val);
    virtual void visit(chart_subtitle		& val);
    virtual void visit(chart_footer			& val);
    virtual void visit(chart_legend			& val);
    virtual void visit(chart_plot_area		& val);
    virtual void visit(chart_axis			& val);
	virtual void visit(chart_series			& val);
    virtual void visit(chart_domain			& val);
	virtual void visit(chart_data_point		& val);
	virtual void visit(chart_mean_value		& val);
	virtual void visit(chart_error_indicator	& val);
	virtual void visit(chart_regression_curve	& val);
	virtual void visit(chart_stock_range_line	& val);
	virtual void visit(chart_stock_loss_marker	& val);
	virtual void visit(chart_stock_gain_marker	& val);
 	virtual void visit(chart_equation			& val);
	virtual void visit(chart_categories		& val);
	virtual void visit(chart_grid			& val);
    virtual void visit(chart_wall			& val);
    virtual void visit(chart_floor			& val);   
	virtual void visit(chart_date_scale		& val);
	
	virtual void visit(table_table			& val);

	virtual void visit(table_table_rows			& val);
	virtual void visit(table_table_row			& val);
	virtual void visit(table_rows_no_group		& val);
 	virtual void visit(table_table_row_group	& val);
	virtual void visit(table_table_header_rows	& val);

	virtual void visit(table_columns_no_group		& val);
	virtual void visit(table_table_header_columns	& val);
	virtual void visit(table_table_columns		& val);
	virtual void visit(table_table_column_group	& val);
    virtual void visit(table_table_column		& val);  
	virtual void visit(table_table_cell			& val);
	virtual void visit(table_covered_table_cell	& val);

private:
    bool stop_;
    
	object_odf_context		& object_odf_context_;

	styles_container		& styles_;
	
	settings_container		& settings_;
	styles_lite_container	& draw_styles_;
	styles_lite_container	& number_styles_;

	oox::num_format_context num_format_context_;
};



}
}
