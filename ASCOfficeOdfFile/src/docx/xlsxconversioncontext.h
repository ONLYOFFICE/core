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

#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

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
#include "oox_chart_context.h"

#include "mediaitems.h"

class CApplicationFonts;

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
    xlsx_conversion_context(cpdoccore::odf_reader::odf_document * odfDocument);
    ~xlsx_conversion_context();

	void set_output_document(package::xlsx_document * document);
    void set_font_directory	(std::wstring pathFonts);

    void start_document();
    void end_document();

    void start_chart(std::wstring const & name);
    void end_chart();
	
	void start_body();
    void end_body();

    void start_office_spreadsheet(const odf_reader::office_element * elm);
    void end_office_spreadsheet();

    const odf_reader::office_element * get_spreadsheet();

    void start_paragraph(const std::wstring & styleName);
    void end_paragraph();

    void start_span(const std::wstring & styleName);
    void end_span();

    bool start_table(std::wstring tableName,std::wstring tableStyleName);
    void end_table();

    void	start_table_column(unsigned int repeated, const std::wstring & defaultCellStyleName, int & cMin, int & cMax);
    void	table_column_last_width(double w);
    double	table_column_last_width();
    void	end_table_column();

	void set_table_row_group(int count, bool collapsed, int level);
    void start_table_row(const std::wstring & styleName, const std::wstring & defaultCellStyleName);
    void non_empty_row();
    void end_table_row();
    bool is_empty_row();
    void set_current_row_height(size_t height_pt);    

	bool in_table_cell();

    void start_table_cell(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned);
    void end_table_cell();
    
    void set_current_cell_style_id(unsigned int xfId);
    int get_current_cell_style_id();

    void start_table_covered_cell();
    void end_table_covered_cell();

    int current_table_column();
    int current_table_row();

    void start_hyperlink(const std::wstring & styleName);
    void end_hyperlink(std::wstring const & href);

    std::wstring current_cell_address();

//------------------------------------------------------------------------------------
	void start_conditional_format			(std::wstring ref);
	void end_conditional_format				(){}

	void start_conditional_format_rule		(int type);
	void end_conditional_format_rule		(){}

	void set_conditional_format_formula		(std::wstring f);
	void set_conditional_format_dxf			(int dxfId);
	void set_conditional_format_showval		(bool val);

	void add_conditional_format_color		(std::wstring col);
	void add_conditional_format_entry		(int type, std::wstring value);
	void set_conditional_format_dataBar		(_CP_OPT(int) min, _CP_OPT(int) max);
//------------------------------------------------------------------------------------

    odf_reader::odf_document * root()
    {
        return odf_document_;
    }

    std::pair<float,float> getMaxDigitSize();

    void process_styles();

    xlsx_text_context           & get_text_context()		{ return xlsx_text_context_; }
    xlsx_table_context          & get_table_context()		{ return xlsx_table_context_; }
    const xlsx_table_context    & get_table_context() const { return xlsx_table_context_; }
    xlsx_style_manager          & get_style_manager()		{ return xlsx_style_; }
    xlsx_xml_worksheet          & current_sheet();
   
    oox_chart_context           & current_chart();
    num_format_context          & get_num_format_context()		 { return num_format_context_; }
    size_t                        get_default_cell_style() const { return default_style_; }
    xlsx_defined_names          & get_xlsx_defined_names()		 { return xlsx_defined_names_; }
    xlsx_table_metrics          & get_table_metrics();
    xlsx_drawing_context        & get_drawing_context();
    xlsx_drawing_context_handle & get_drawing_context_handle();	
    xlsx_comments_context       & get_comments_context();
	xlsx_comments_context_handle & get_comments_context_handle();

    mediaitems & get_mediaitems() { return mediaitems_; }

private:
    void create_new_sheet(std::wstring const & name);

    package::xlsx_document				*output_document_;
    const odf_reader::office_element	*spreadsheet_;
    odf_reader::odf_document			*odf_document_;

    CApplicationFonts					*applicationFonts_;

    std::vector<xlsx_xml_worksheet_ptr> sheets_;
    std::vector<oox_chart_context_ptr>  charts_;
  
    std::wstringstream                  defaultOutput_;
    std::pair<float,float>              maxDigitSize_;
    num_format_context                  num_format_context_;
    size_t                              default_style_;
    mediaitems                          mediaitems_;
  
    xlsx_style_manager              xlsx_style_;
    xlsx_defined_names              xlsx_defined_names_;
    xlsx_table_context              xlsx_table_context_;
    xlsx_text_context               xlsx_text_context_;
    xlsx_drawing_context_handle     xlsx_drawing_context_handle_;
    xlsx_comments_context_handle    xlsx_comments_context_handle_;
    
};

}
}
