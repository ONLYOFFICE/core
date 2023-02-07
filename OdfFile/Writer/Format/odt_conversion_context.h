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

#include "odf_conversion_context.h"

#include "odf_comment_context.h"
#include "odf_notes_context.h"
#include "odf_table_context.h"
#include "odf_text_context.h"

namespace cpdoccore 
{ 
namespace odf_writer 
{
class office_text;

struct odt_section_state
{
	office_element_ptr	elm;
	office_element_ptr	style_elm;
	std::wstring		style_name;

	bool				empty;
	bool				continuous;
	int					count_columns;
};


class odt_conversion_context : public odf_conversion_context
{
public:
	odt_conversion_context(package::odf_document * outputDocument);
	~odt_conversion_context();
	
	virtual void start_document();
	virtual void end_document();

	virtual odf_drawing_context		* drawing_context();
	virtual odf_text_context		* text_context();
	virtual odf_controls_context	* controls_context();

	virtual odf_style_context		* styles_context();
		
	odf_comment_context				* comment_context();
	odf_table_context				* table_context();
	
	odf_notes_context				* notes_context();

	virtual void start_drawing_context();
	virtual void end_drawing_context();

	virtual bool start_math();
	virtual void end_math();

	void add_text_content	(const std::wstring & text);

	void start_paragraph (bool styled = false);
	void end_paragraph ();

	void add_paragraph_break(int type);

	void start_hyperlink (const std::wstring& link, const std::wstring& location);
	void end_hyperlink ();
	
	void start_bookmark (int id, const std::wstring& name);
	void end_bookmark (int id);

	void start_sequence ();
	void end_sequence ();

	void start_drop_down();
	void end_drop_down();

	void start_table_of_content ();
	void end_table_of_content ();

	void start_bibliography ();
	void end_bibliography ();
	
	void start_alphabetical_index ();
	void end_alphabetical_index ();
	
	void start_illustration_index ();
	void end_illustration_index ();

	void start_table_index ();
	void end_table_index ();

	void start_index_field();
	void end_index_field();

	void start_field		(bool in_span);
	void end_field			();
	void separate_field		();
	void add_field_instr	(const std::wstring &instr);
	void set_field_instr	();
	std::map<std::wstring, std::wstring> parse_instr_options(const std::wstring& value);
	
	void set_field_date_time(const std::wstring &date_time);
	void set_field_color	(_CP_OPT(color) &color);
	void set_field_name		(const std::wstring &name);
	void set_field_format	(const std::wstring &format);
	void set_field_drop_down();
	void set_field_item		(const std::wstring &value, const std::wstring &display);

	void start_run			(bool styled = false);
	void end_run			();

	void	add_section					(bool continuous);
	void	add_section_columns			(int count, double space_pt, bool separator );
	void	add_section_column			(std::vector<std::pair<double,double>> width_space);
	int		get_current_section_columns	();
	void	flush_section				();

	void set_master_page_name(std::wstring master_name);

	void start_drop_cap			(style_paragraph_properties * paragraph_properties);
		void set_drop_cap_lines	(int lines);
		void set_drop_cap_margin(bool val);
	void end_drop_cap			();
	bool in_drop_cap			() {return drop_cap_state_.enabled;}
	style_text_properties* get_drop_cap_properties();
	int get_drop_cap_lines() {return drop_cap_state_.lines;}

	int start_comment			(int oox_comment_id);
	void end_comment			(int oox_comment_id);
	void start_comment_content	();
	void end_comment_content	();

	void start_note				(int oox_ref_id, int type);
		void start_note_content	();
		void end_note_content	();
	void end_note				();

	bool start_change			(int id, int type, std::wstring &author, std::wstring &userId, std::wstring &date, std::wstring style_name = L"");
	void end_change				(int id, int type);
	//bool is_delete_changes		();

	void start_table				(bool styled = false);
		void start_table_columns	();
			void add_table_column	(double width = -1);
		void end_table_columns		();
		void start_table_row		(bool styled = false, bool header = false);
			void add_default_cell	();
			void start_table_cell	(int col, bool covered, bool styled = false);
			void end_table_cell		();
		void end_table_row			();
	void end_table					();

	void start_list_item	(int level, std::wstring style_name);
	void end_list_item		();

	void set_no_list();

	bool is_empty_section	(){return current_root_elements_.size() > 0 ? false : true; }

	bool start_header		(int type);
	bool start_footer		(int type);
	void end_header_footer	();

	void add_empty_header	(int type);
	void add_empty_footer	(int type);

	void set_background		(_CP_OPT(color) &color, int type);

	bool is_paragraph_in_current_section_;

	bool empty() {return current_root_elements_.empty();}

private:
	void start_table_header_rows();
	void end_table_header_rows	();

	office_text*				root_text_;
	office_element_ptr			root_document_;
	
	odf_text_context*			main_text_context_;

	odf_table_context			table_context_;
	odf_comment_context			comment_context_;
	odf_notes_context			notes_context_;

	bool			is_header_;
	bool			is_footer_;
	bool			is_background_;

	std::wstring	current_master_page_;
	
	odf_controls_context					controls_context_;	

	std::vector<odf_element_state>		current_root_elements_; // for section, if needed
	std::vector<odt_section_state>		sections_;

	std::map<std::wstring, int>			mapSequenceDecls;
	std::map<int, std::wstring>			mapBookmarks;

	void add_to_root();

	struct _field_state
	{
		_typeField		type = fieldUnknown;

		std::wstring	name;
		std::wstring	value;

		std::wstring	format;
		std::wstring	instrText;

		_CP_OPT(color)	color_;

		short			status = 0;//0, 1, 2, 3 - init, prapare, start, finish
		bool			in_span = false;
		bool			result = false; //after separate
		bool			bHyperlinks	= false;
		bool			bHidePageNumbers = false;
		std::wstring	captionSEQ;
		std::wstring	title;
		std::vector<std::wstring> arStyleLevels;
		int				outline_levels = 0;
		std::vector<int> arTemplateTypes;//text, link, tab ....
		int				tabLeader = 0;//dot

		std::vector<std::pair<std::wstring, std::wstring>> items;
	};

	std::vector<_field_state> current_fields;
	
	struct _text_changes_state
	{
		odf_text_context	*main_text_context;
		std::vector<int>	current_types;
	}text_changes_state_;

	bool table_row_header_state_ = false;

	bool is_hyperlink_;

	struct _drop_cap_state
	{
		void clear()
		{
			enabled = false; 
			paragraph_properties = NULL;	
			characters = 0; 
			inline_style = true; 
			characters_size_pt = 0;
			lines = 0;
			text_properties = office_element_ptr();
		}

		bool enabled = false;
		style_paragraph_properties	*paragraph_properties = NULL;
		office_element_ptr			text_properties;

		int		lines = 0;
		int		characters = 0;
		bool	inline_style = false;
		double	characters_size_pt = 0;
	}drop_cap_state_;

};


}
}
