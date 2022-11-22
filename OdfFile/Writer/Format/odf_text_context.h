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

#include "odf_page_layout_state.h"

#include "office_elements_create.h"

#include "../../DataTypes/fobreak.h"

namespace cpdoccore {
namespace odf_writer
{
	enum _typeField
	{
		fieldUnknown = 0,
		fieldHyperlink,
		fieldPage,
		fieldNumPages,
		fieldDateTime,
		fieldPageRef,
		fieldSeq,
		fieldXE,
		fieldTextInput,
		fieldExpression,
		fieldDropDown,
		fieldDate,
		fieldTime,

		fieldBibliography = 0xff + 1,
		fieldIndex,
		fieldIllustration,
		fieldTable,
		fieldToc
	};	
class odf_conversion_context;
class odf_style_context;
class paragraph;
class style_paragraph_properties;
class style_text_properties;

class odf_text_context: boost::noncopyable
{
public:
	odf_text_context	(odf_conversion_context *odf_context, odf_style_context *styles_context);
    ~odf_text_context	();
public:
 	odf_style_context*		get_styles_context();//для embedded
	void					set_styles_context(odf_style_context*  styles_context);//для embedded
		
	void clear_params();
   
	void add_text_style		(office_element_ptr & style_elm, std::wstring style_name);

	void set_single_object	(bool bSingle, style_paragraph_properties *para_props, style_text_properties *text_props);
	
    void add_text_content	(const std::wstring &text);
	void add_text_space		(int count);
	void add_text_date		(const std::wstring &text);
	void add_text_time		(const std::wstring &text);
	void add_text_page_number(const std::wstring &text);
	void add_text_page_count(const std::wstring &text);
	void add_text_file_name	(const std::wstring &text);
 	void add_text_sheet_name(const std::wstring &text);
   
	void set_symbol_font	(const std::wstring & font);
 	void set_symbol_text	(int sym);
   
	void start_paragraph	(bool styled = false);
	void start_paragraph	(office_element_ptr & elm, bool styled = false);
    void end_paragraph		();

	void start_element		(office_element_ptr & elm, office_element_ptr style_elm = office_element_ptr(),std::wstring style_name = L"");
    void end_element		();

	void add_element_in_span_or_par(office_element_ptr & elm);
 	
	bool start_field		(int type, const std::wstring& value, const std::wstring& format);
	void end_field			();

	void start_span			(bool styled = false); 
    void end_span			();

	void start_list_item	();
	void end_list_item		();

	bool start_list			(std::wstring style_name);
	void end_list			();

	bool is_need_break		(){return bool(need_break_);}
	bool set_type_break		(int type, int clear);
	void save_property_break();

	void add_tab			(_CP_OPT(int) ref = boost::none);
	void add_hyperlink		(const std::wstring & link, const std::wstring & display, const std::wstring & location);
	void set_outline_level	(int level);

	bool get_list_item_state()		{return list_state_.levels.size() > 0 ? list_state_.levels.back() : false;}
	
	void set_parent_paragraph_style	(std::wstring & style_name);
	void set_parent_span_style		(std::wstring & style_name);

	bool set_master_page_name		(std::wstring & master_page_name);

	bool get_KeepNextParagraph		()			{return keep_next_paragraph_;}
	void set_KeepNextParagraph		(bool val)	{keep_next_paragraph_ = val;}
	
	style_text_properties		*get_text_properties		()	{return text_properties_;}
	style_paragraph_properties	*get_paragraph_properties	()	{return paragraph_properties_;}
//------------------------------------------------------------------------------------
	int		current_outline_;
	bool	in_field_;

	std::vector<odf_element_state>	current_level_;			//постоянно меняющийся список уровней наследования
	std::vector<odf_element_state>	text_elements_list_;	//параграфы, списки , ... 
	struct _list_state
	{
		bool				started_list = false;
		std::vector<bool>	levels;
		std::wstring		style_name;
		int					prevNumID = -1;
	}list_state_;
	bool					single_paragraph_;
private:
	bool					keep_next_paragraph_;

	_CP_OPT(odf_types::fo_break) need_break_; 

	style_paragraph_properties	*paragraph_properties_;	//хранилка-опознавалка что свойства приписаны другому, не текстовому, объекту
	style_text_properties		*text_properties_;		//хранилка-опознавалка что свойства приписаны другому, не текстовому, объекту

	odf_style_context		*styles_context_;
	odf_conversion_context	*odf_context_;

	std::wstring			parent_span_style_;
	std::wstring			parent_paragraph_style_;
};
typedef shared_ptr<odf_text_context>::Type		odf_text_context_ptr;

}
}
