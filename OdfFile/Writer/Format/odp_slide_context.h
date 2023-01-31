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
    void end_page ();

	void remove_page();

	void				set_styles_context(odf_style_context*  styles_context);
	odf_style_context*	get_styles_context();
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

private:

    odp_conversion_context&		context_;
	odf_style_context*			styles_context_;
	
	odf_table_context			table_context_;
	odf_comment_context			comment_context_;
	
	std::vector<odp_page_state>	page_state_list_;
	int							count_slides_;

	friend class odp_conversion_context;

};


}
}
