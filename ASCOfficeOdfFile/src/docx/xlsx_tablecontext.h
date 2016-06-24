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

#include <string>
#include <map>

#include "ooxtablerowspanned.h"
#include "xlsx_table_state.h"

namespace cpdoccore {
namespace oox {

class xlsx_conversion_context;
class xlsx_text_context;

class xlsx_table_context
{
public:
    xlsx_table_context(xlsx_conversion_context * Context, xlsx_text_context & textCotnext);

public:
    void start_table(std::wstring tableName, std::wstring tableStyleName, int id);
    void end_table();

    void start_cell(const std::wstring & formula,
                    size_t columnsSpanned,
                    size_t rowsSpanned);

    void end_cell();

    std::wstring default_row_cell_style();
    std::wstring default_column_cell_style();

    void start_covered_cell();
    void end_covered_cell();

    void start_cell_content();    
    int	end_cell_content();

    void set_current_cell_style_id(unsigned int xfId);
    int get_current_cell_style_id();

    int current_column();
    int current_row();

	void set_table_row_group(int count, bool collapsed, int level);
    void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
    void non_empty_row();
    bool is_empty_row();
    void end_row();

    void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName);

	unsigned int columns_count();

	void serialize_sort					(std::wostream & _Wostream);
	void serialize_autofilter			(std::wostream & _Wostream);
    void serialize_merge_cells			(std::wostream & _Wostream);
	void serialize_table_format			(std::wostream & _Wostream);
	void serialize_conditionalFormatting(std::wostream & _Wostream);

	xlsx_table_metrics & get_table_metrics();
  
	xlsx_drawing_context & get_drawing_context();

    xlsx_comments_context & get_comments_context();
	
    void table_column_last_width	(double w);
    double table_column_last_width	();

    xlsx_table_state_ptr state();

	void			start_hyperlink();
	std::wstring	end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display);

	void dump_rels_hyperlinks(rels & Rels);
	
	void serialize_hyperlinks(std::wostream & _Wostream);
	
	void start_database_range(std::wstring table_name, std::wstring ref);
		void set_database_orientation	(bool val);
		void set_database_header		(bool val);
		void set_database_filter		(bool val);
		
	void add_database_sort	(int field_number, int order);
	void end_database_range();


private:
    xlsx_conversion_context				*xlsx_conversion_context_;
    xlsx_text_context					&xlsx_text_context_;
	
	std::vector<xlsx_table_state_ptr>	 xlsx_table_states_;
	std::vector<xlsx_data_range_ptr>	 xlsx_data_ranges_;
	
	std::multimap<std::wstring, int>	 xlsx_data_ranges_map_;		

};


}
}
