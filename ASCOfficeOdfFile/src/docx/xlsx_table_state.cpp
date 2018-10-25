﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include <odf/odf_document.h>
#include <xml/simple_xml_writer.h>

#include "logging.h"

#include "xlsx_table_state.h"
#include "xlsx_utils.h"
#include "xlsxconversioncontext.h"

#include "../odf/odfcontext.h"
#include "../odf/style_table_properties.h"
#include "../odf/datatypes/stylefamily.h"

#include "../formulasconvert/formulasconvert.h"


namespace cpdoccore {
namespace oox {

void xlsx_data_range::serialize_autofilter (std::wostream & _Wostream)
{
	if (!filter) return;

	CP_XML_WRITER(_Wostream)
	{			
		CP_XML_NODE(L"autoFilter")
		{
			CP_XML_ATTR(L"ref", ref);
		}
	}
}
void xlsx_data_range::serialize_sort (std::wostream & _Wostream)
{
	if (bySort.empty()) return;

	if (byRow) return;

	CP_XML_WRITER(_Wostream)
	{			
		CP_XML_NODE(L"sortState")
		{
			CP_XML_ATTR(L"ref", ref);

			if (!byRow)
				CP_XML_ATTR(L"columnSort", true);

			for (size_t i = 0 ; i < bySort.size(); i++)
			{
				bool in_range = true;
				std::wstring ref1, ref2;
				size_t col_1, row_1, col_2, row_2;

				int pos = ref.find(L":");
				if (pos >= 0)
				{
					ref1 = ref.substr(0, pos );
					ref2 = ref.substr(pos + 1);
				}
				getCellAddressInv(ref1, col_1, row_1);
				getCellAddressInv(ref2, col_2, row_2);

				if (byRow)
				{
					if (bySort[i].first < col_1 || bySort[i].first > col_2 )	in_range = false;

					ref1 = getCellAddress(bySort[i].first + ( withHeader ? 1 : 0), row_1);
					ref2 = getCellAddress(bySort[i].first + ( withHeader ? 1 : 0), row_2);
				}
				else
				{
					if (bySort[i].first < row_1 || bySort[i].first > row_2 )	in_range = false;

					ref1 = getCellAddress(col_1, bySort[i].first + ( withHeader ? 1 : 0));
					ref2 = getCellAddress(col_2, bySort[i].first + ( withHeader ? 1 : 0));
				}
				if (in_range)
				{
					CP_XML_NODE(L"sortCondition")
					{
						CP_XML_ATTR(L"ref", ref1 + L":" + ref2);	

						if (bySort[i].second)
							CP_XML_ATTR(L"descending", 1);	
					}

				}
			}
		}	
	}
}
//-------------------------------------------------------------------------------------------------------------------------

xlsx_table_state::xlsx_table_state(xlsx_conversion_context * Context, std::wstring styleName, std::wstring tableName, int tableId)
  : context_				(Context),
    table_style_			(styleName),
    tableName_				(tableName),
	tableId_				(tableId),
    current_table_column_	(-1),
    current_table_row_		(-1),
    columns_spanned_num_	(0),
    columns_count_			(0),
    xlsx_drawing_context_	(Context->get_drawing_context_handle()),
    xlsx_comments_context_	(Context->get_comments_context_handle()),
    table_column_last_width_(0.0),
	in_cell(false),
	bEndTable_(false),
	bRTL_(false)

{        
	odf_reader::style_table_properties	* table_prop = NULL;
	odf_reader::style_instance			* tableStyle = context_->root()->odf_context().styleContainer().style_by_name(table_style_, odf_types::style_family::Table, false);
	
	if ((tableStyle) && (tableStyle->content()))
		table_prop = tableStyle->content()->get_style_table_properties();

	if ((table_prop) && (table_prop->content().common_writing_mode_attlist_.style_writing_mode_))
	{
		if (table_prop->content().common_writing_mode_attlist_.style_writing_mode_->get_type() == odf_types::writing_mode::RlTb)
			bRTL_ = true;
	}
}
    
void xlsx_table_state::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    for (unsigned int i = 0; i <repeated; ++i)
        column_default_cell_style_name_.push_back(defaultCellStyleName);

    columns_count_ += repeated;
    columns_.push_back(repeated);
}

void xlsx_table_state::set_rtl(bool val)
{
	bRTL_ = val;
}

unsigned int xlsx_table_state::columns_count() const
{
    return columns_count_;
}

void xlsx_table_state::set_table_row_group(int count, bool collapsed, int level)
{
	group_row_.enabled = true;
	group_row_.count = count;
	group_row_.collapsed = collapsed;
	group_row_.level = level;
}
void xlsx_table_state::add_empty_row(int count)
{
	current_table_row_ += count;
}
void xlsx_table_state::start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
{
    empty_row_ = true;
    // reset column num, column spanned style
    current_table_column_ = -1; 
    current_table_row_++;
    columns_spanned_style_ = L"";
    row_default_cell_style_name_ = defaultCellStyleName;
        
    // set row style name
    table_row_style_ = StyleName;
}

void xlsx_table_state::non_empty_row()
{
    empty_row_= false;
}

bool xlsx_table_state::is_empty_row() const
{
    return empty_row_;
}

std::wstring xlsx_table_state::default_row_cell_style() const
{
    return row_default_cell_style_name_;
}

std::wstring xlsx_table_state::default_column_cell_style() const
{
	if (current_table_column_ + 1 < (int)column_default_cell_style_name_.size())
		return column_default_cell_style_name_.at(current_table_column_ + 1);
	else
	{
		//непонятная хрень!! - неправильно сформирован ods???
		return L"";
	}
}

void xlsx_table_state::end_row()
{
    table_row_style_ = L"";
}

std::wstring xlsx_table_state::current_row_style() const
{
    return table_row_style_;
}

void xlsx_table_state::start_cell(size_t columnsSpanned, size_t rowsSpanned)
{
    current_table_column_++;

    // в случае если объединение имеет место добавляем запись о нем
    if (columnsSpanned != 0 || rowsSpanned != 0)
	{
		xlsx_merge_cells_.add_merge(current_table_column_, current_table_row_, columnsSpanned, rowsSpanned);
	}

    if ( current_columns_spaned() > 0 )
    {
        _CP_LOG << L"[warning] current columns spanned > 0\n";
    }

    columns_spanned_num_ = static_cast<int>(columnsSpanned);

    // обновляем вектор, в котором хранятся информация об объединении строк
    // добавляем в него новый столбец

    
    for (size_t i = 0; i <= columns_spanned_num_; ++i)
    {
        if (current_table_column_+i >= (int)(rows_spanned_.size()))
            rows_spanned_.push_back(xlsx_row_spanned());   
    }

    if ((int)rows_spanned_.size() <= current_table_column_)
    {
        _CP_LOG << L"[warning] set_rows_spanned error\n";
    }
    else
    {
        if (rows_spanned_[current_table_column_].num() > 0)
        {
            _CP_LOG << L"[warning] current rows spanned > 0\n";
        }
        rows_spanned_[current_table_column_].num(static_cast<unsigned int>(rowsSpanned));
        rows_spanned_[current_table_column_].column_spanned(static_cast<unsigned int>(columnsSpanned));
        for (size_t i = 0; i <= columns_spanned_num_; ++i)
        {
            rows_spanned_[current_table_column_ + i].set_style_id(-1);
        }
    }

	in_cell = true;
}

void xlsx_table_state::end_cell()
{
	in_cell = false;
}

void xlsx_table_state::set_current_cell_style_id(unsigned int xfId)
{
    for (size_t i = 0; i <= columns_spanned_num_; ++i)
    {
        rows_spanned_[current_table_column_ + i].set_style_id((int)xfId);
    }
}

int xlsx_table_state::get_current_cell_style_id()
{
    return rows_spanned_[current_table_column_].get_style_id();
}

void xlsx_table_state::start_covered_cell()
{
    current_table_column_++;

    // обновляем вектор, в котором хранятся информация об объединении строк
    // добавляем в него новый столбец

    if (current_table_column_ >= (int)(rows_spanned_.size()))
        rows_spanned_.push_back(xlsx_row_spanned());

    // использовали текущую ячейку, уменьшаем счетчики оставшихся объединенных ячеек
    // для столбцов и строк

    if (columns_spanned_num_ > 0)
        columns_spanned_num_--;

    if (rows_spanned_[current_table_column_].num() > 0)
        rows_spanned_[current_table_column_].decrease();

    if (current_table_column_ > 0)
    {
        const unsigned int pred = rows_spanned_[current_table_column_ - 1].column_spanned();
        if (pred > 0)
        {
            rows_spanned_[current_table_column_].column_spanned(pred - 1);
            rows_spanned_[current_table_column_].num(rows_spanned_[current_table_column_ - 1].num());
        }
    }
}

void xlsx_table_state::end_covered_cell()
{
}

int xlsx_table_state::current_column() const
{
    return current_table_column_;
}

int xlsx_table_state::current_row() const
{
    return current_table_row_;
}

unsigned int xlsx_table_state::current_columns_spaned() const
{
    return columns_spanned_num_;
}

unsigned int xlsx_table_state::current_rows_spanned(unsigned int Column) const
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG << L"[warning] current_rows_spanned error\n";
        return 0;
    }
    else
    {
        return rows_spanned_[Column].num();
    }
}

double charsToSize(unsigned int charsCount, double maxDigitSize) 
{
	return 1.0 * int((maxDigitSize * charsCount + 5.0) / maxDigitSize * 256.0) / 256.0;
}

void xlsx_table_state::serialize_page_properties (std::wostream & strm)
{
	_CP_OPT(std::wstring) masterPageName = context_->root()->odf_context().styleContainer().master_page_name_by_name(table_style_);
	if (!masterPageName) return;

	odf_reader::style_master_page* master_style_ = context_->root()->odf_context().pageLayoutContainer().master_page_by_name(*masterPageName);
	if (!master_style_) return;
	if (!master_style_->attlist_.style_page_layout_name_) return;

	odf_reader::page_layout_instance * page_layout = context_->root()->odf_context().pageLayoutContainer().page_layout_by_name(*master_style_->attlist_.style_page_layout_name_);
	if (!page_layout) return;

	page_layout->xlsx_serialize(strm, *context_);
}
void xlsx_table_state::serialize_background (std::wostream & strm)
{
	if (tableBackground_.empty()) return;

	CP_XML_WRITER(strm)
	{			
		CP_XML_NODE(L"picture")
		{
			CP_XML_ATTR(L"r:id",  tableBackground_);
		}
	}
}
void xlsx_table_state::serialize_table_format (std::wostream & strm)
{
	odf_reader::odf_read_context & odfContext = context_->root()->odf_context();

	CP_XML_WRITER(strm)
	{
		odf_reader::style_table_properties	* table_prop = NULL;
		odf_reader::style_instance			* tableStyle = odfContext.styleContainer().style_by_name(table_style_, odf_types::style_family::Table, false);
		
		if ((tableStyle) && (tableStyle->content()))
			table_prop = tableStyle->content()->get_style_table_properties();

		if (table_prop)
		{
			CP_XML_NODE(L"sheetPr")
			{
				//at filterMode="false">
				if (table_prop->content().tableooo_tab_color_)
				{
					CP_XML_NODE(L"tabColor")
					{
						CP_XML_ATTR(L"rgb", table_prop->content().tableooo_tab_color_->get_hex_value());	
					}
				}
				//<pageSetUpPr fitToPage="true"/>
			}
		}
		int columns = (std::max)(current_table_column_, (int)columns_count_);
		int rows	= (std::max)(current_table_row_,	1);

		if (columns	< 1024 && columns	> 1 &&
			rows	< 1024 && rows		> 1)
		{
			CP_XML_NODE(L"dimension")
			{
				if (current_table_column_ < 0) current_table_column_ = columns_count_;
				std::wstring ref2 = getCellAddress( current_table_column_, current_table_row_);
				CP_XML_ATTR(L"ref", L"A1:" + ref2);
			}
		}
		if (odfContext.Settings().get_views_count() > 0)
		{
			CP_XML_NODE(L"sheetViews")
			{
				CP_XML_NODE(L"sheetView")
				{
					CP_XML_ATTR(L"workbookViewId", 0);

					if (bRTL_)
						CP_XML_ATTR(L"rightToLeft", 1); 

					std::wstring s_col, s_row;
					for (int i = 0; i < odfContext.Settings().get_table_view_count(0, tableName_); i++)
					{
						std::pair<std::wstring, std::wstring> value = odfContext.Settings().get_table_view(0, tableName_, i);

						if (value.first == L"ZoomValue")
						{
							CP_XML_ATTR(L"zoomScale",		value.second);
							CP_XML_ATTR(L"zoomScaleNormal", value.second);
						}
						if (value.first == L"ShowGrid")			CP_XML_ATTR(L"showGridLines",	value.second);
						if (value.first == L"CursorPositionX")	s_col = value.second;
						if (value.first == L"CursorPositionY")	s_row = value.second;
					}

					int col = -1, row = -1;
					try
					{
						if (!s_col.empty())		col =  boost::lexical_cast<int>(s_col);
						if (!s_row.empty())		row =  boost::lexical_cast<int>(s_row);
					}
					catch(...){}

					if (col >= 0 && row >= 0)
					{
						CP_XML_NODE(L"selection")
						{	
							CP_XML_ATTR(L"activeCell",		getCellAddress(col, row));			
							CP_XML_ATTR(L"activeCellId",	0);			
							CP_XML_ATTR(L"pane",			L"topLeft");			
							CP_XML_ATTR(L"sqref",			getCellAddress(col, row));			
						}
						
					}
				}
			}
			//	-showRowColHeaders
		} 

		double default_height = (2 * context_->getMaxDigitSize().second * 72. / 96. * 100.) /100.;//in point size.
		
		odf_reader::style_instance * rowDefStyle = odfContext.styleContainer().style_default_by_type(odf_types::style_family::TableRow);
		if ((rowDefStyle) && (rowDefStyle->content()))
		{
			const odf_reader::style_table_row_properties * prop = rowDefStyle->content()->get_style_table_row_properties();

			if ( (prop) &&  (prop->attlist_.style_row_height_))
			{
				default_height = prop->attlist_.style_row_height_->get_value_unit(odf_types::length::pt);
			}
			std::wstringstream ht_s;
			ht_s.precision(1);
			ht_s << std::fixed << default_height;
			
			CP_XML_NODE(L"sheetFormatPr")
			{
				CP_XML_ATTR(L"defaultRowHeight", ht_s.str());	
			}
		}  
	}

}
void xlsx_table_state::serialize_merge_cells(std::wostream & strm)
{
    return xlsx_merge_cells_.xlsx_serialize(strm);
}
void xlsx_table_state::serialize_ole_objects(std::wostream & strm)
{
    return xlsx_drawing_context_.get_drawings()->serialize_objects(strm);
}
void xlsx_table_state::serialize_controls(std::wostream & strm)
{
    return xlsx_drawing_context_.get_drawings()->serialize_controls(strm);
}
void xlsx_table_state::serialize_hyperlinks(std::wostream & strm)
{
    return xlsx_hyperlinks_.xlsx_serialize(strm);
}
void xlsx_table_state::serialize_conditionalFormatting(std::wostream & strm)
{
    return xlsx_conditionalFormatting_context_.serialize(strm);
}
void xlsx_table_state::dump_rels_hyperlinks(rels & Rels)
{
    return xlsx_hyperlinks_.dump_rels(Rels);
}
void xlsx_table_state::dump_rels_ole_objects(rels & Rels)
{
    return get_drawing_context().get_drawings()->dump_rels_sheet(Rels);
}

void xlsx_table_state::start_hyperlink()
{
}

 std::wstring xlsx_table_state::end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display)
{
    return xlsx_hyperlinks_.add(ref, href, display);
}




}
}
