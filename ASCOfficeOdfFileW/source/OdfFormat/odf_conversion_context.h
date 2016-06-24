/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include "object_package.h"

#include "odf_style_context.h"
#include "odf_settings_context.h"

#include "odf_page_layout_context.h"

#include "odf_drawing_context.h"
#include "odf_chart_context.h"

#include "mediaitems.h"

class CApplicationFonts;

namespace cpdoccore { 
namespace odf_writer {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class odf_conversion_context : boost::noncopyable
{
	struct _object
	{
		std::wstring						name;

		office_element_ptr					content;
		std::vector<office_element_ptr>		content_styles;
		std::vector<office_element_ptr>		styles;	
		office_element_ptr					settings;	
	
		odf_style_context_ptr				style_context;	
		odf_settings_context_ptr			settings_context;
		_mediaitems							mediaitems;
	};

public:
    odf_conversion_context(package::odf_document * outputDocument);
    virtual ~odf_conversion_context();

    void set_fonts_directory(std::wstring pathFonts);

    virtual void	start_document() = 0 ;
	virtual void	end_document();
	
    package::odf_document   *output_document_;
    CApplicationFonts       *applicationFonts_;


	virtual odf_drawing_context		* drawing_context()		= 0;
	virtual odf_text_context		* text_context()		= 0;

	virtual void					start_text_context()	= 0;
	virtual void					end_text_context()		= 0;

    virtual void start_image(const std::wstring & image_file_name) = 0;
	
	virtual odf_style_context		* styles_context();
	
	odf_settings_context			* settings_context();
	odf_chart_context				* chart_context();
	odf_page_layout_context			* page_layout_context();

	odf_number_styles_context		* numbers_styles_context();
	_mediaitems						* mediaitems();

	void start_chart();
	void end_chart();

	void start_spreadsheet();
	void end_spreadsheet();
	
	void start_text();
	void end_text();

	void create_object();
	void end_object();

	std::wstring			get_next_name_object();
	office_element_ptr &	get_current_object_element();

	office_element_ptr start_tabs();
		void add_tab(_CP_OPT(int) type, _CP_OPT(odf_types::length) length, _CP_OPT(int) leader);
	void end_tabs();

private:
	odf_element_state temporary_;

	std::vector<_object>		objects_;//"0" = root

	odf_chart_context			chart_context_;
	odf_page_layout_context		page_layout_context_;	

	void process_styles		(_object & object, bool isRoot);
	void process_settings	(_object & object, bool isRoot);
	
	int	 current_object_;


	//page_layout_container & pageLayoutContainer()	{ return page_layout_container_; }
	//fonts_container		& fontContainer()		{ return fonts_container_; }
	//list_style_container	& listStyleContainer()	{ return list_style_container_; }

	//notes_configuration &	noteConfiguration()		{ return notes_configuration_; }

	//styles_lite_container &	Templates()			{ return template_container_; }


    //styles_container		major_style_container_;
	//page_layout_container	page_layout_container_;
	//fonts_container		fonts_container_;
	//list_style_container	list_style_container_;
	//notes_configuration	notes_configuration_;

	//styles_lite_container	template_container_;
};

}
}
