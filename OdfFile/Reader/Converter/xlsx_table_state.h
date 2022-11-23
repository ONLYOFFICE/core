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

#include <vector>
#include <map>

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
	xlsx_data_range() : byRow(true), filter(false), bTablePart(true), withHeader(false), cell_start(0,0), cell_end(0,0) {}

	std::wstring	table_name;
	std::wstring	name;
	
	std::wstring	ref;
	
	std::pair<int, int> cell_start;
	std::pair<int, int> cell_end;

	bool			bTablePart;
	bool			byRow;
	bool			filter;
	bool			withHeader;

	std::vector<std::pair<size_t, bool>> bySort;  //field + order
	
	void serialize_sort			(std::wostream & _Wostream);
	void serialize_autofilter	(std::wostream & _Wostream);
	
	std::vector<std::wstring> header_values;

	void set_header(size_t row, size_t col1, size_t col2)
	{
		row_header = row;
		start_column_header = col1;
		end_column_header = col2;

		for (size_t i = start_column_header; i <= end_column_header; i++)
			header_values.push_back(L"");
	}
	void set_header_value(size_t col, size_t row, const std::wstring& value)
	{
		while (col - start_column_header + 1 >= header_values.size())
			header_values.push_back(L"");

		std::map<std::wstring, int>::iterator pFind = map_unique_header_values.find(value);
		if (pFind == map_unique_header_values.end())
		{
			map_unique_header_values.insert(std::make_pair(value, 1));
			header_values[col - start_column_header] = value;
		}
		else
		{
			pFind->second++;
			header_values[col - start_column_header] = value + std::to_wstring(pFind->second);
		}
	}
	bool in_header(size_t col, size_t row) 
	{
		return (row_header == row && (col >= start_column_header && col <= end_column_header));
	}

private:
	size_t row_header;
	size_t start_column_header;
	size_t end_column_header;

	std::map<std::wstring, int> map_unique_header_values;
};

class xlsx_table_state
{
public:
    xlsx_table_state(xlsx_conversion_context * Context, std::wstring styleName, std::wstring tableName, int tableId);
    
	std::wstring current_style() const { return table_style_; }

	void set_rtl(bool val);
	void set_protection(bool val, const std::wstring &key, const std::wstring &algorithm);
 
	void set_protection_select_protected_cells(bool val);
	void set_protection_select_unprotected_cells(bool val);
	void set_protection_insert_columns(bool val);
	void set_protection_insert_rows(bool val);
	void set_protection_delete_columns(bool val);
	void set_protection_delete_rows(bool val);

	void start_column	(unsigned int repeated, const std::wstring & defaultCellStyleName);
    void start_row		(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);

	void set_column_break_before();
	void set_column_break_after();
    
	void set_row_break_before();
	void set_row_break_after();

	void non_empty_row	();
    bool is_empty_row	() const;
    void end_row		();

	void add_empty_row(int count);

	void set_end_table(){ bEndTable = true; }
	bool get_end_table(){ return bEndTable; }
    
	std::wstring current_row_style			() const;
    std::wstring default_row_cell_style		() const;
    std::wstring default_column_cell_style	() const;

	void start_table_row_group(bool collapsed);
	void end_table_row_group();

	void start_table_column_group(bool collapsed);
	void end_table_column_group();

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
	xlsx_conditionalFormatting_context	& get_conditionalFormatting_context()	{ return xlsx_conditionalFormatting_context_;}

    void	table_column_last_width(double w) { table_column_last_width_ = w; }
    double	table_column_last_width() const { return table_column_last_width_; };

    void			start_hyperlink	();
	std::wstring	end_hyperlink (std::wstring const & ref, std::wstring const & href, std::wstring const & display);

	void set_background (std::wstring rId) { tableBackground_ = rId; }

	void serialize_conditionalFormatting	(std::wostream & _Wostream);
	void serialize_table_format				(std::wostream & _Wostream);
	void serialize_merge_cells				(std::wostream & _Wostream);
    void serialize_hyperlinks				(std::wostream & _Wostream);
	void serialize_controls					(std::wostream & _Wostream);
    void serialize_ole_objects				(std::wostream & _Wostream);
	void serialize_page_properties			(std::wostream & _Wostream);
	void serialize_background				(std::wostream & _Wostream);
	void serialize_protection				(std::wostream & _Wostream);
	void serialize_header_footer			(std::wostream & _Wostream);
	void serialize_breaks					(std::wostream & _Wostream);

	void dump_rels_hyperlinks				(rels & Rels);
	void dump_rels_ole_objects				(rels & Rels);

    std::wstring get_table_name()	const { return tableName_; }
	int			 get_table_id()		const { return tableId_; }
	bool		 get_table_hidden()	const { return bHidden; }

	std::vector<bool>					group_rows_; //collapsed
	std::vector<bool>					group_columns_; //collapsed

	friend class xlsx_conversion_context;
	friend class xlsx_table_context;
private:	
    xlsx_conversion_context *			context_;    

	struct _protection
	{
		bool bEnabled = false;
		std::wstring protect_key;
		std::wstring protect_key_algorithm;
		bool insertColumns = true;
		bool insertRows = true;
		bool deleteColumns = true;
		bool deleteRows = true;
		bool selectLockedCells = false;
		bool selectUnockedCells = false;
	}									protection;
	bool								bRTL;
	bool								bEndTable;
	bool								bHidden;

    std::wstring						tableName_;
	int									tableId_;
    std::wstring						tableBackground_;

	_CP_OPT(double)						header_page;
	_CP_OPT(double)						footer_page;

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
    
    std::vector<unsigned int>			column_breaks_;
    std::vector<unsigned int>			row_breaks_;

	xlsx_drawing_context				xlsx_drawing_context_;
    xlsx_comments_context				xlsx_comments_context_;
	xlsx_conditionalFormatting_context	xlsx_conditionalFormatting_context_; 
	
	xlsx_merge_cells					xlsx_merge_cells_; 
    xlsx_table_metrics					xlsx_table_metrics_;
    xlsx_hyperlinks						xlsx_hyperlinks_;

};


}
}

