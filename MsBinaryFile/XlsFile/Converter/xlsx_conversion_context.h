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

#include <sstream>
#include <boost/noncopyable.hpp>

#include "../../Common/Utils/OptPtr.h"

#include "xlsx_textcontext.h"
#include "xlsx_sheet_context.h"
#include "xlsx_drawing_context.h"
#include "xlsx_chart_context.h"
#include "xlsx_comments_context.h"
#include "xlsx_pivots_context.h"
#include "xlsx_external_context.h"
#include "xlsx_activeX_context.h"
#include "xlsx_tablecontext.h"

#include "xlsx_output_xml.h"

#include "external_items.h"

namespace oox {

namespace package
{
    class xlsx_document;
}
class xlsx_conversion_context : boost::noncopyable
{
public:
    xlsx_conversion_context	(package::xlsx_document * outputDocument);
    ~xlsx_conversion_context();

    //void set_font_directory(std::wstring pathFonts);

    void start_document();
    void end_document();

	bool start_sheet();
		void set_sheet_state(const std::wstring & state);
		void set_sheet_type(int type);
 		void set_sheet_name(const std::wstring & name);
		void set_sheet_id(int id);
   void end_sheet();

    void start_chart();
	void end_chart(){}

    void start_table();
	void end_table();

	std::wstring start_activeX();
	void end_activeX(){}

    void start_external();
    void end_external();

	std::wostream					& shared_strings()		{ return xlsx_shared_strings_; }    
 	std::wostream					& defined_names()		{ return xlsx_defined_names_; }    
	std::wostream					& workbook_views()		{ return xlsx_workbook_views_; }
	std::wostream					& custom_views()		{ return xlsx_custom_views_; }
	std::wostream					& workbook_format()		{ return xlsx_workbook_pr_; }	
	std::wostream					& workbook_protection()	{ return xlsx_workbookProtection_; }
	std::wostream					& workbook_calcpr()		{ return xlsx_workbookCalcPr_; }

	xlsx_text_context				& get_text_context()	{ return text_context_; }
    xlsx_sheet_context				& get_sheet_context()	{ return sheet_context_; }
    xlsx_xml_worksheet				& current_sheet();
 
	oox_chart_context				& current_chart();
	oox_external_context			& current_external();
	oox_activeX_context				& current_activeX();

	xlsx_pivots_context				& get_pivots_context()	{return pivots_context_;}
	xlsx_drawing_context			& get_drawing_context();
	xlsx_drawing_context_handle		& get_drawing_context_handle();	
 	xlsx_comments_context			& get_comments_context();
	xlsx_comments_context_handle	& get_comments_context_handle();
 	xlsx_tables_context				& get_tables_context()	{return tables_context_;}

	external_items & get_mediaitems() { return mediaitems_; }

	void add_connections		(const std::wstring & connections);
	void add_query_table		(const std::wstring & query_table);
	void add_control_props		(const std::wstring & target, const std::wstring &props);
private:

    void create_new_sheet(std::wstring const & name);

    package::xlsx_document				*output_document_;

	external_items						mediaitems_;
    xlsx_sheet_context					sheet_context_;
    xlsx_text_context					text_context_;	
	xlsx_pivots_context					pivots_context_;
	xlsx_tables_context					tables_context_;

    std::vector<oox_chart_context_ptr>		charts_;    
	std::vector<oox_external_context_ptr>	externals_;
	std::vector<oox_activeX_context_ptr>	activeXs_;

	size_t								next_vml_file_id_; //используется для footer/header & comments

	std::map<int, int>					sheets_map_;
    std::vector<xlsx_xml_worksheet_ptr>	sheets_;

	std::wstring						connections_;
	std::map<std::wstring, std::wstring>query_tables_; 
	std::map<std::wstring, std::wstring>control_props_; 

	std::wstringstream					xlsx_shared_strings_;
	std::wstringstream					xlsx_defined_names_;
	std::wstringstream					xlsx_workbook_views_;
	std::wstringstream					xlsx_custom_views_;
	std::wstringstream					xlsx_workbook_pr_;
	std::wstringstream					xlsx_workbookProtection_;
	std::wstringstream					xlsx_workbookCalcPr_;

    xlsx_drawing_context_handle			xlsx_drawing_context_handle_;
    xlsx_comments_context_handle		xlsx_comments_context_handle_;
    
};

}
