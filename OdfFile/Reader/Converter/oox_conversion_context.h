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

#include <sstream>
#include <iosfwd>
#include <map>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace cpdoccore { 

namespace oox{

	class docx_conversion_context;
	class pptx_conversion_context;
};

namespace odf_reader
{
	class	style_instance;
	class	style_text_properties;
	class	fonts_container;
	typedef boost::shared_ptr<style_text_properties> style_text_properties_ptr;
	
	class	office_element;
	class	form_element;
	typedef boost::shared_ptr<office_element> office_element_ptr;
};

namespace oox 
{
	enum _form_type
	{
		formNone = 0,
		formButton = 1,
		formLabel = 2,
		formCheckbox = 3,
		formCombobox = 4,
		formListbox = 5,
		formDateTime = 6,
		formSpin = 7,
		formScroll = 8,
		formFrame = 9,
		formImage = 10,
		formText = 11
	};
class forms_context
{
public:

	struct _state
	{
		std::wstring				id;
		std::wstring				name;
		_form_type					type;
		std::wstring				label;
		std::wstring				uuid;
		std::wstring				value;
		odf_reader::form_element*	element = NULL;

		std::wstring				ctrlPropId;
	
		void clear()
		{
			type = formNone;
			id.clear();
			name.clear();
			label.clear();
			value.clear();
			uuid.clear();
			ctrlPropId.clear();
			element = NULL;
		}
	};
	forms_context(){}

	void start_element	(_form_type type);
		void set_id		(const std::wstring& id);
		void set_name	(const std::wstring& name);
		void set_label	(const std::wstring& label);
		void set_uuid	(const std::wstring& uuid);
		void set_value	(const std::wstring& value);
		void set_element(odf_reader::form_element *elm);
	void end_element	();

	_state& get_state_element (std::wstring id);

private:
	_state							current_state_;
	std::map<std::wstring, _state>	mapElements_;
};
class tabs_context : boost::noncopyable
{
public:

	std::vector<odf_reader::office_element_ptr> tabs;
	std::map<int, odf_reader::office_element_ptr> clear_tabs;

	void clear()
	{
		tabs.clear();
		clear_tabs.clear();
	}

	void reset();

	void add(const odf_reader::office_element_ptr & element, double margin_left);
	void docx_convert(oox::docx_conversion_context & Context);
};
class styles_context : boost::noncopyable
{
public:
    void start();
   
	std::wstringstream & text_style();
    std::wstringstream & paragraph_nodes();
    std::wstringstream & paragraph_attr();
    std::wstringstream & table_style();
    std::wstringstream & list_style();

    void docx_serialize_text_style	(std::wostream & strm, std::wstring parenStyleId, std::wstring & strChange);
    void docx_serialize_table_style	(std::wostream & strm, std::wstring & strChange);
  
	void pptx_serialize_table_style	(std::wostream & strm);

	std::wstring & extern_node(){return extern_node_;}
	std::wstring & hlinkClick(){return hlinkClick_;}
	std::wstring & text_style_ext(){return text_style_ext_;}

	const odf_reader::style_instance * get_current_processed_style() const { return current_processed_style_; }
   
	void start_process_style(const odf_reader::style_instance * Instance);
    void end_process_style();

private:
	const odf_reader::style_instance *	current_processed_style_;
	
	std::wstring				extern_node_;
	std::wstring				hlinkClick_;
	std::wstring				text_style_ext_;

    std::wstringstream			list_style_;
    std::wstringstream			text_style_;
    std::wstringstream			paragraph_nodes_;
    std::wstringstream			paragraph_attr_;
    std::wstringstream			table_style_;
};


	
	class math_context : boost::noncopyable
	{
	public:
		math_context(odf_reader::fonts_container & fonts, bool graphic = false);
		
		void				start();
		std::wstring		end();

		std::wostream		& output_stream()		{ return math_stream_; }	
		
		std::wstringstream	& math_style_stream()	{ return math_style_stream_; }
		
		odf_reader::fonts_container &			fonts_container_;
		int										base_font_size_;
		odf_reader::style_text_properties_ptr	text_properties_;

		std::wstring							nsRPr_;
		bool									graphRPR_;

		bool									is_need_e_;
	private:
		std::wstringstream						math_stream_;
		std::wstringstream						math_style_stream_;
	};

}
}

