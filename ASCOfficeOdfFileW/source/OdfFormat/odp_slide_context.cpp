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
#include "logging.h"

#include "odp_conversion_context.h"
#include "odp_slide_context.h"
#include "odf_text_context.h"
#include "style_graphic_properties.h"
#include "style_paragraph_properties.h"

#include <iostream>


namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

odp_slide_context::odp_slide_context(odp_conversion_context & Context) : styles_context_(NULL), context_(Context), table_context_(&Context), comment_context_(&Context)
{       
	count_slides_	= 0;
	styles_context_ = Context.styles_context();
}
void odp_slide_context::set_styles_context(odf_style_context*  styles_context)
{
	styles_context_ = styles_context;
}

odf_style_context* odp_slide_context::get_styles_context()
{
	return styles_context_;
}

odp_page_state & odp_slide_context::state()
{
    return page_state_list_.back();
}

void odp_slide_context::start_page(office_element_ptr & elm)
{
	page_state_list_.push_back( odp_page_state(&context_, elm) );
	
	std::wstring style_name_new = L"dp" + std::to_wstring(++count_slides_);
	
	office_element_ptr & style = styles_context_->add_or_find(style_name_new, style_family::DrawingPage, true);
	style->create_child_element(L"style", L"drawing-page-properties");

	state().set_page_id(count_slides_);
	state().set_page_style(style);
	state().drawing_context()->set_styles_context(styles_context_);
}

void odp_slide_context::end_page()
{
	state().drawing_context()->finalize(state().page_elm_);
	state().finalize_page();
}

void odp_slide_context::remove_page()
{
	page_state_list_.pop_back();
}

odf_table_context* odp_slide_context::table_context()
{
	return &table_context_;
}

void odp_slide_context::start_table()
{
	state().drawing_context()->start_frame();

	office_element_ptr elm, style_elm, default_cell;
	create_element(L"table", L"table", elm, &context_);

//--------------------------------------------------------------------
	odf_style_state_ptr style_state;

//общие свойства ячеек
	styles_context_->create_style(L"", odf_types::style_family::TableCell, true, false, -1);
					//ради нормального задания дефолтовых свойств на cells
	style_state = styles_context_->last_state(style_family::TableCell);
	if (style_state)
	{
		default_cell = style_state->get_office_element();	
		table_context()->set_default_cell_properties(style_state->get_name());
	}

//стиль создаем всегда	
	styles_context_->create_style(L"", odf_types::style_family::Table, true, false, -1); 
	
	style_state = styles_context_->last_state(style_family::Table);
	if (style_state)	style_elm = style_state->get_office_element();
		
//--------------------------------------------------------------------
	
	table_context()->start_table(elm, true);

	state().drawing_context()->start_element(elm, default_cell);
}
void odp_slide_context::start_table_columns()
{
	office_element_ptr elm;
	create_element(L"table", L"table-columns", elm, &context_);

	state().drawing_context()->start_element(elm);
}
void odp_slide_context::add_table_column(double width)
{
	office_element_ptr elm;
	create_element(L"table", L"table-column", elm, &context_);

	styles_context_->create_style(L"", style_family::TableColumn, true, false, -1);

	//не срабатывает ..
	//std::wstring parent_name = table_context()->get_default_cell_properties();

	//if (parent_name.length() > 0) 
	//{
	//	odf_writer::style_table_cell_properties * props = styles_context()->last_state().get_table_cell_properties();
	//	style * style_ = NULL;
	//	
	//	if (styles_context()->find_odf_style(parent_name,style_family::TableCell,style_))
	//	{
	//		style_table_cell_properties * parent = style_->content_.get_style_table_cell_properties();
	//		props->apply_from(parent);
	//	}
	//}

	table_context()->add_column(elm, true);
		table_context()->set_column_width(width);

	state().drawing_context()->start_element(elm); // для связи элментов
	state().drawing_context()->end_element();
}
void odp_slide_context::end_table_columns()
{
	state().drawing_context()->end_element();
}
void odp_slide_context::start_table_header_rows()
{
	office_element_ptr elm;
	create_element(L"table", L"table-header-rows", elm, &context_);

	state().drawing_context()->start_element(elm);
}
void odp_slide_context::end_table_header_rows()
{
	state().drawing_context()->end_element();
}
void odp_slide_context::start_table_row (bool styled)
{
	office_element_ptr elm, style_elm;
	create_element(L"table", L"table-row", elm, &context_);

	if (styled)
	{
		styles_context_->create_style(L"",odf_types::style_family::TableRow, true, false, -1);
		
		odf_style_state_ptr style_state = styles_context_->last_state(style_family::TableRow);
		if (style_state)
			style_elm = style_state->get_office_element();
	}
	state().drawing_context()->start_element(elm, style_elm);

	table_context()->start_row(elm, styled);

}
void odp_slide_context::start_table_cell(int col, bool covered, bool styled)
{
	for (int i = table_context()->current_column() ; i < col;  i++)
	{
		add_default_cell(col);
	}
//-------------------------------------------------------
	office_element_ptr elm, style_elm;
	if (covered)
		create_element(L"table", L"covered-table-cell", elm, &context_);
	else
		create_element(L"table", L"table-cell", elm, &context_);

	if (styled)
	{
		styles_context_->create_style(L"", odf_types::style_family::TableCell, true, false, -1); 
		
		odf_style_state_ptr style_state = styles_context_->last_state(style_family::TableCell);
		if (style_state)
			style_elm = style_state->get_office_element();

		std::wstring def_style = table_context()->get_default_cell_properties();
		if (false == def_style.empty())
		{
			odf_writer::odf_style_state_ptr style_state_def;
			if (styles_context_->find_odf_style_state(def_style, style_family::TableCell, style_state_def))
			{
				graphic_format_properties *gr = style_state->get_graphic_properties();
				graphic_format_properties *gr_def = style_state_def->get_graphic_properties();

				if (gr && gr_def) gr->apply_from(*gr_def);

				style_paragraph_properties *para = style_state->get_paragraph_properties();
				style_paragraph_properties *para_def = style_state_def->get_paragraph_properties();
				
				if (para && para_def) para->apply_from(para_def);
			}
		}
	}
	
	table_context()->start_cell(elm, style_elm ? true : false);

	state().drawing_context()->start_element(elm, style_elm);
}
void odp_slide_context::end_table_cell()
{
	table_context()->end_cell();
	state().drawing_context()->end_element();
}
void odp_slide_context::end_table_row()
{
	for (int i = table_context()->current_column() ; i < table_context()->count_columns(); i++)
	{
		add_default_cell(i);
	}
//---------------------------------------------
	table_context()->end_row();
	state().drawing_context()->end_element();
}

void odp_slide_context::add_default_cell(int col)
{
	office_element_ptr elm;
	create_element(L"table", L"covered-table-cell", elm, &context_);

	table_context()->start_cell(elm, false);
	state().drawing_context()->start_element(elm);

	table_context()->end_cell();
	state().drawing_context()->end_element();
}
void odp_slide_context::end_table()
{
	table_context()->end_table();
	state().drawing_context()->end_element();
	
	state().drawing_context()->end_frame();	
}


}
}
