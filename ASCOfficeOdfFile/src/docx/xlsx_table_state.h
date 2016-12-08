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

#include <vector>

#include "xlsx_row_spanned.h"
#include "xlsx_merge_cells.h"
#include "xlsx_table_metrics.h"
#include "xlsx_drawing_context.h"
#include "xlsx_comments_context.h"
#include "xlsx_hyperlinks.h"
#include "xlsx_conditionalFormatting.h"

namespace cpdoccore {
namespace oox {

class xlsx_conversion_context;
class xlsx_table_context;

class xlsx_table_state;
typedef _CP_PTR(xlsx_table_state) xlsx_table_state_ptr;

class xlsx_data_range;
typedef _CP_PTR(xlsx_data_range) xlsx_data_range_ptr;


class xlsx_data_range
{
public:
	xlsx_data_range() : byRow(true), filter(false), withHeader(false), cell_start(0,0), cell_end(0,0) {}

	std::wstring	table_name;
	
	std::wstring	ref;
	
	std::pair<int, int> cell_start;
	std::pair<int, int> cell_end;

	bool			byRow;
	bool			filter;
	bool			withHeader;

	std::vector<std::pair<int, bool>> bySort;  //field + order
	
	void serialize_sort			(std::wostream & _Wostream);
	void serialize_autofilter	(std::wostream & _Wostream);
};

class xlsx_table_state
{
public:
    xlsx_table_state(xlsx_conversion_context * Context, std::wstring styleName, std::wstring tableName, int tableId);
    
	std::wstring current_style() const { return table_style_; }
    
	void start_column	(unsigned int repeated, const std::wstring & defaultCellStyleName);
    void start_row		(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
    
	void non_empty_row	();
    bool is_empty_row	() const;
    void end_row		();
    
	std::wstring current_row_style			() const;
    std::wstring default_row_cell_style		() const;
    std::wstring default_column_cell_style	() const;

	void set_table_row_group(int count, bool collapsed, int level);
   
	void start_cell(size_t columnsSpanned, size_t rowsSpanned);
    void end_cell();

    void start_covered_cell();
    void end_covered_cell();

    void set_current_cell_style_id(unsigned int xfId);
    int  get_current_cell_style_id();

    int current_column() const;
    int current_row() const;

    unsigned int current_columns_spaned	() const;
    unsigned int current_rows_spanned	(unsigned int Column) const;
    unsigned int columns_count			() const;
    
    xlsx_table_metrics					& get_table_metrics()					{ return xlsx_table_metrics_; }
    xlsx_drawing_context				& get_drawing_context()					{ return xlsx_drawing_context_; }
    xlsx_comments_context				& get_comments_context()				{ return xlsx_comments_context_; }
	xlsx_conditionalFormatting_context	& get_conditionalFormatting_context()	{return xlsx_conditionalFormatting_context_;}

    void	table_column_last_width(double w) { table_column_last_width_ = w; }
    double	table_column_last_width()	const { return table_column_last_width_; };

    void			start_hyperlink	();
	std::wstring	end_hyperlink	(std::wstring const & ref, std::wstring const & href, std::wstring const & display);

	void serialize_conditionalFormatting	(std::wostream & _Wostream);
	void serialize_table_format				(std::wostream & _Wostream);
	void serialize_merge_cells				(std::wostream & _Wostream);
    void serialize_hyperlinks				(std::wostream & _Wostream);
    void serialize_ole_objects				(std::wostream & _Wostream);
    
	void dump_rels_hyperlinks				(rels & Rels);
	void dump_rels_ole_objects				(rels & Rels);

    std::wstring get_table_name()	const { return tableName_; }
	int			 get_table_id()		const { return tableId_; }
	
	struct _group_row
	{
		bool enabled;
		int count;
		int level;
		bool collapsed;
	}group_row_;

	friend class xlsx_conversion_context;
	friend class xlsx_table_context;

private:	
    xlsx_conversion_context *			context_;    

    std::wstring						tableName_;
	int									tableId_;

    std::wstring						table_style_;
    std::wstring						table_row_style_;
    std::wstring						cell_style_;
    std::vector<std::wstring>			column_default_cell_style_name_;
    std::wstring						row_default_cell_style_name_;
   
	int									current_table_column_;
    int									current_table_row_;
	bool								empty_row_;
	bool								in_cell;

    unsigned int						columns_spanned_num_;
    std::wstring						columns_spanned_style_;
   
	std::vector<xlsx_row_spanned>		rows_spanned_;
    std::vector<unsigned int>			columns_;
    unsigned int						columns_count_;
    double								table_column_last_width_;
    
    xlsx_drawing_context				xlsx_drawing_context_;
    xlsx_comments_context				xlsx_comments_context_;
	xlsx_conditionalFormatting_context	xlsx_conditionalFormatting_context_; 
	
	xlsx_merge_cells					xlsx_merge_cells_; 
    xlsx_table_metrics					xlsx_table_metrics_;
    xlsx_hyperlinks						xlsx_hyperlinks_;

};


}
}

