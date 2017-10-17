/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "odf_conversion_context.h"
#include "ods_table_context.h"

namespace cpdoccore { 
namespace odf_writer {

class office_spreadsheet;
class odf_text_context;

struct _font_metrix
{
	_font_metrix(){IsCalc = italic = bold = false; font_size = approx_symbol_size =0;}
	bool IsCalc;

	std::wstring font_name;
	double		 font_size;
	bool		 italic;
	bool		 bold;

	double		 approx_symbol_size;//in pt
};

class ods_conversion_context : public odf_conversion_context
{
public:
	ods_conversion_context(package::odf_document * outputDocument);
	
	virtual void start_document();
	virtual void end_document();

	void start_sheet();
        void set_sheet_dimension(const std::wstring & ref);
	void end_sheet();

	void start_columns();
		void add_column(int start_column, int repeated, int level = 0, bool _default = false);
	void end_columns();

	void calculate_font_metrix(std::wstring name, double size, bool italic, bool bold);

	void start_rows();
		void start_row(int _start_row, int repeated, int level = 0, bool _default = false);
		void end_row();

		void start_cell(std::wstring & ref, int xfd_style);
		void end_cell();
	void end_rows();

	void start_cell_text();
    void add_text_content(const std::wstring & text);
	void end_cell_text();

    void add_merge_cells(const std::wstring & ref);
	void add_hyperlink(std::wstring & ref, std::wstring & link, std::wstring & display);

	void start_comment		(int col, int row, std::wstring & author);
	void set_comment_rect	(double l, double t, double w, double h);
	void end_comment		();
///////////////////////////////////////////////////////
	ods_table_state &		current_table() { return table_context_.state();}
/////////////////////////////////////////////////////
	virtual void start_text_context();
	virtual void end_text_context();

	virtual odf_drawing_context		* drawing_context()	{return current_table().drawing_context();}
	virtual odf_text_context		* text_context()	{return current_text_context_;}
	
	void start_drawings();
	void end_drawings();

	double convert_symbol_width(double val);
	
    void add_defined_range		(const std::wstring & name, const std::wstring & cell_range, int sheet_id, bool printable = false);
    void add_defined_expression	(const std::wstring & name, const std::wstring & value, int sheet_id, bool printable = false);
	void add_autofilter			(std::wstring ref);

	void start_table_part	(std::wstring name, std::wstring ref);
	void end_table_part		();
	void set_table_part_autofilter(bool val);

	void start_conditional_formats();
	void end_conditional_formats(){}

	void start_table_view(int view_id);
	void end_table_view();

private:
	_font_metrix				font_metrix_;
	ods_table_context			table_context_;
	
	odf_text_context*			current_text_context_;
	office_spreadsheet*			root_spreadsheet_;


	

};


}
}
