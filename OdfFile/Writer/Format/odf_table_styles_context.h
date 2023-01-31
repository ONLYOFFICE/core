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

#include <string>
#include <vector>

#include "office_elements_create.h"

namespace cpdoccore {
namespace odf_writer {

class style_table_cell_properties;
class style_text_properties;
class style_paragraph_properties;
class style_table_properties;

//typedef shared_ptr<style_text_properties>::Type			style_text_properties_ptr;
//typedef shared_ptr<style_paragraph_properties>::Type	style_paragraph_properties_ptr;
//typedef shared_ptr<style_table_cell_properties>::Type	style_table_cell_properties_ptr;
//row, column ???

struct _style_properties
{
	_style_properties(){is = false;}
	office_element_ptr	table_props;
	office_element_ptr	text_props;
	office_element_ptr	paragraph_props;
	office_element_ptr	table_cell_props;

	bool is;
};
struct table_format_state
{
	std::wstring style_name;
	
	_style_properties table_;

	_style_properties band1Horz_;
	_style_properties band1Vert_;
	_style_properties band2Horz_;
	_style_properties band2Vert_;
	_style_properties firstCol_;
	_style_properties firstRow_;
	_style_properties lastCol_;
	_style_properties lastRow_;
	_style_properties neCell_;
	_style_properties nwCell_;
	_style_properties seCell_;
	_style_properties swCell_;
	_style_properties wholeTable_; //???

	_CP_OPT(std::wstring)	insizeH;
	_CP_OPT(std::wstring)	insizeV;

};

class odf_table_styles_context
{
public:
	odf_table_styles_context(){current = NULL; context_ = NULL; }
	
	void set_odf_context(odf_conversion_context * Context)
	{
		context_ = Context;
	}

//-----------------------------------------------
// input
//-----------------------------------------------

	void start_style(std::wstring styale_name);
	void end_style();

	void add_band1Horz();
	void add_band1Vert();
	void add_band2Horz();
	void add_band2Vert();
	void add_firstCol();
	void add_firstRow();
	void add_lastCol();
	void add_lastRow();
	void add_neCell();
	void add_nwCell();
	void add_seCell();
	void add_swCell();
	void add_wholeTable(); 

	style_table_cell_properties *get_table_cell_properties();
	style_paragraph_properties	*get_paragraph_properties();
	style_text_properties		*get_text_properties();
	style_table_properties		*get_table_properties();

	void set_table_insideV(std::wstring border);
	void set_table_insideH(std::wstring border);

//-----------------------------------------------
// output
//-----------------------------------------------
	bool start_table(std::wstring style_name);
		void set_current_dimension(int col, int row);
		void set_flags(int val);

		bool is_paragraph_properties();
		bool is_text_properties();

		void get_table_properties		(style_table_properties		*props);
		void get_table_cell_properties	(style_table_cell_properties *props);
		void get_paragraph_properties	(style_paragraph_properties	*props);
		void get_text_properties		(style_text_properties		*props);
		
		void get_table_cell_properties	(int col, int row,	style_table_cell_properties *props);
		void get_text_properties		(int col, int row,	style_text_properties		*props);
		void get_paragraph_properties	(int col, int row,	style_paragraph_properties	*props);

		bool is_styled(int col, int row);
		
		_CP_OPT(std::wstring)  get_table_insideV();
		_CP_OPT(std::wstring)  get_table_insideH();

	void end_table();

private:

	std::vector<table_format_state> table_format_array_;

	_style_properties *current;

	odf_conversion_context *context_;

	struct _use_style
	{
		int table_style_;
		int table_col_count_;
		int table_row_count_;

		bool cols;
		bool rows;
		bool first_row;
		bool last_row;
		bool first_col;
		bool last_col;
	};

	std::vector<_use_style> current_used_;

//////////////////


};
}
}
