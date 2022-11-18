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

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <unordered_map>

#include "oox_conversion_context.h"

#include "xlsx_textcontext.h"
#include "xlsx_tablecontext.h"

#include "xlsx_sharedstrings.h"
#include "xlsx_styles.h"
#include "xlsx_output_xml.h"
#include "xlsx_num_format_context.h"
#include "xlsx_drawing_context.h"
#include "xlsx_comments_context.h"
#include "xlsx_defined_names.h"
#include "xlsx_table_metrics.h"
#include "xlsx_pivots_context.h"
#include "xlsx_data_validation.h"
#include "oox_chart_context.h"

#include "mediaitems.h"

namespace cpdoccore {

namespace odf_reader 
{
    class odf_document;
    class office_element;
}

namespace oox {

namespace package
{
    class xlsx_document;
}

class xlsx_conversion_context : boost::noncopyable
{
public:
    xlsx_conversion_context	(cpdoccore::odf_reader::odf_document * odfDocument);
    ~xlsx_conversion_context();

	void set_output_document(package::xlsx_document * document);
    void set_font_directory	(std::wstring pathFonts);

    void start_document	();
    void end_document	();

	void set_table_structure_protected(bool val);

    void start_chart(std::wstring name);
    void end_chart	();

	void start_body	();
    void end_body	();

    void start_office_spreadsheet	(const odf_reader::office_element * elm);
    void end_office_spreadsheet		();

    const odf_reader::office_element * get_spreadsheet();

    void start_paragraph(const std::wstring & styleName);
    void end_paragraph	();

    void start_span		(const std::wstring & styleName);
    void end_span		();

    bool start_table	(std::wstring tableName, std::wstring tableStyleName);
    void end_table		();

	int find_sheet_by_name(std::wstring tableName);
    xlsx_xml_worksheet & current_sheet(int index = -1);

	void	start_table_column		(unsigned int repeated, const std::wstring & defaultCellStyleName, int & cMin, int & cMax);
    void	table_column_last_width	(double w);
    double	table_column_last_width	();
    void	end_table_column		();

	bool in_table_cell();

    void start_table_cell			(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned);
    void end_table_cell				();

    void start_table_covered_cell	();
    void end_table_covered_cell		();

    int current_table_column		();
    int current_table_row			();

    std::wstring current_cell_address();

    void start_hyperlink	(const std::wstring & styleName);
    void end_hyperlink		(std::wstring const & href);
	    
    void set_current_cell_style_id	(unsigned int xfId);
    int	 get_current_cell_style_id	();
	
    size_t	get_default_cell_style() const { return default_style_; }
	int		get_dxfId_style(const std::wstring &style_name);

//------------------------------------------------------------------------------------
	void add_pivot_sheet_source				(const std::wstring & sheet_name, int index_table_view);

	void add_jsaProject	(const std::string &content);

	void add_control_props(const std::wstring & rid, const std::wstring & target, const std::wstring & props);
	
	//int add_external_link(const std::wstring & external);

	void add_table_part(const std::wstring & table) {table_parts_.push_back(table);}
	size_t get_table_parts_size() {return table_parts_.size();}
//------------------------------------------------------------------------------------

    odf_reader::odf_document * root()
    {
        return odf_document_;
    }

    std::pair<double, double> getMaxDigitSize();

    void process_styles();

    xlsx_text_context           & get_text_context()		{ return xlsx_text_context_; }
    xlsx_table_context          & get_table_context()		{ return xlsx_table_context_; }
    const xlsx_table_context    & get_table_context() const { return xlsx_table_context_; }
    xlsx_style_manager          & get_style_manager()		{ return xlsx_style_; }   
	forms_context				& get_forms_context()		{ return forms_context_; }
    oox_chart_context           & current_chart();
	math_context				& get_math_context()			 { return math_context_; }
    num_format_context          & get_num_format_context()		 { return num_format_context_; }
    xlsx_defined_names          & get_xlsx_defined_names()		 { return xlsx_defined_names_; }
	xlsx_pivots_context			& get_pivots_context()			 { return xlsx_pivots_context_;}
    xlsx_table_metrics          & get_table_metrics();
    xlsx_drawing_context        & get_drawing_context();
    xlsx_comments_context       & get_comments_context();
	xlsx_comments_context_handle & get_comments_context_handle();
	xlsx_conditionalFormatting_context	& get_conditionalFormatting_context();

	xlsx_dataValidations_context & get_dataValidations_context()	{ return xlsx_dataValidations_context_;}

	xlsx_drawing_context_handle_ptr & get_drawing_context_handle();	
	void set_drawing_context_handle(xlsx_drawing_context_handle_ptr &handle);

	mediaitems_ptr & get_mediaitems() { return mediaitems_; }
	void			 set_mediaitems(mediaitems_ptr &items);
//---------------------------------------
	static std::unordered_map<std::wstring, int>	mapExternalLink_;
	std::map<std::wstring, int>						mapUsedNames_;
private:
	void serialize_bookViews(std::wostream & strm);
	void serialize_calcPr	(std::wostream & strm);    

    package::xlsx_document				*output_document_;
    const odf_reader::office_element	*spreadsheet_;
    odf_reader::odf_document			*odf_document_;

	bool								table_structure_protected_ = false;

    std::vector<xlsx_xml_worksheet_ptr> sheets_;
    std::vector<oox_chart_context_ptr>  charts_;
	std::vector<std::wstring>			table_parts_;
 
    std::wstringstream                  defaultOutput_;
    std::pair<double, double>			maxDigitSize_;
    num_format_context                  num_format_context_;
    size_t                              default_style_;
    mediaitems_ptr						mediaitems_;
	std::multimap<std::wstring, int>	mapPivotsTableView_;
	
 	std::map<std::wstring, std::wstring>	control_props_; 
 
    xlsx_style_manager              xlsx_style_;
    xlsx_defined_names              xlsx_defined_names_;
    xlsx_table_context              xlsx_table_context_;
    xlsx_text_context               xlsx_text_context_;
	xlsx_pivots_context				xlsx_pivots_context_;
    xlsx_comments_context_handle    xlsx_comments_context_handle_;
	xlsx_dataValidations_context	xlsx_dataValidations_context_;
    xlsx_drawing_context_handle_ptr	drawing_context_handle_;
	
	math_context					math_context_;
	forms_context					forms_context_;

	static std::wstring change_external(boost::wsmatch const & what);
};

}
}
