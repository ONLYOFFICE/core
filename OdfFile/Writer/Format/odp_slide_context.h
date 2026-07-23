/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <vector>
#include "odp_page_state.h"

#include "odf_table_context.h"
#include "odf_comment_context.h"

namespace cpdoccore {

namespace odf_writer {

class odp_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

//class odp_table_context : public odf_table_context
//{
//public:
//	void clear()
//	{
//		first_row = false;
//		first_col = false;
//		band_row = false;
//		band_col = false;
//		last_row = false;
//		last_col = false;
//	}
//
//	void set_first_row(bool val)	{first_row = val;}
//	void set_first_col(bool val)	{first_col = val;}
//	void set_band_row(bool val)		{band_row = val;}
//	void set_band_col(bool val)		{band_col = val;}
//	void set_last_row(bool val)		{last_row = val;}
//	void set_last_col(bool val)		{last_col = val;}
//
//private:
//	bool first_row = false;
//	bool first_col = false;
//	bool band_row = false;
//	bool band_col = false;
//	bool last_row = false;
//	bool last_col = false;
//};

class odp_slide_context
{
public:
    odp_slide_context(odp_conversion_context & Context);

    void start_page (office_element_ptr & elm);
	void hide_page();
    void end_page ();

	void remove_page();

	void set_styles_context(odf_style_context_ptr  styles_context);
	odf_style_context_ptr get_styles_context();

	odf_table_context*	table_context();

	void start_table				();
		void start_table_columns	();
			void add_table_column	(double width = -1);
		void end_table_columns		();
		void start_table_header_rows();
		void end_table_header_rows	();
		void start_table_row		(bool styled = false);
			void add_default_cell	(int col);
			void start_table_cell	(int col, bool covered, bool styled = true);
			void end_table_cell		();
		void end_table_row			();
	void end_table					();    
	
	odp_page_state & state();
	int				 page_index();

private:

    odp_conversion_context&		context_;
	odf_style_context_ptr		styles_context_;
	
	odf_table_context			table_context_;
	odf_comment_context			comment_context_;
	
	std::vector<odp_page_state>	page_state_list_;
	int							count_slides_;

	friend class odp_conversion_context;

};


}
}
