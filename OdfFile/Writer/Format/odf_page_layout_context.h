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

#include <string>

#include "odf_page_layout_state.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/color.h"

namespace cpdoccore {
namespace odf_writer {

class odf_conversion_context;

class odf_style_context;
typedef shared_ptr<odf_style_context>::Type odf_style_context_ptr;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_page_layout_context;
typedef shared_ptr<odf_page_layout_context>::Type odf_page_layout_context_ptr;

class odf_page_layout_context
{
public:
    odf_page_layout_context(odf_conversion_context * Context);
    ~odf_page_layout_context();

	void set_styles_context	(odf_style_context_ptr Context);

	void create_layer_sets();

	void add_master_page(std::wstring oox_name);
	void create_layout_page();
	
	office_element_ptr add_presentation_layout_page();

	void set_current_master_page_base();

	void process_master_styles			(office_element_ptr root );
	void process_automatic_for_styles	(office_element_ptr root );
	void process_office_styles			(office_element_ptr root );//images, fonts, ... for master pages and templates

    odf_layout_state * last_layout();
    odf_master_state * last_master();

	odf_style_context_ptr get_local_styles_context() //for automatic styles of the styles themselves
		{return local_style_context_;}

	void set_page_margin				(_CP_OPT(double) top, _CP_OPT(double) left, _CP_OPT(double) bottom, _CP_OPT(double) right, _CP_OPT(double) header, _CP_OPT(double) footer);
	void set_page_margin				(_CP_OPT(odf_types::length) top, _CP_OPT(odf_types::length) left, _CP_OPT(odf_types::length) bottom, _CP_OPT(odf_types::length) right);
	void set_page_gutter				(_CP_OPT(odf_types::length) length_);
	
	void set_page_border				(std::wstring top, std::wstring left, std::wstring bottom, std::wstring right);
	void set_page_border_shadow			(bool val);
	
	void set_page_border_padding		(int border, double length_pt);
	void set_page_border_offset			(int type);

	void set_page_orientation			(int type);
	void set_page_size					(_CP_OPT(odf_types::length) width, _CP_OPT(odf_types::length) height);
	void set_page_scale					(double val);
	void set_page_scaleToX				(int val);
	void set_page_scaleToY				(int val);
	void set_page_first_page			(int val);
	void set_page_centered				(bool bHorizontal, bool bVertical);
	void set_page_print_gridLines		(bool val);
	void set_page_print_headings		(bool val);

	void set_title_page_enable			(bool val);
	void set_pages_mirrored				(bool val);
	
	bool add_footer(int type);
		void set_footer_size(_CP_OPT(odf_types::length) length_, _CP_OPT(odf_types::length) length_min);
	
	bool add_header(int type);
		void set_header_size(_CP_OPT(odf_types::length) length_, _CP_OPT(odf_types::length) length_min);

	void set_header_footer_image	(office_element_ptr image);
	void set_background				(_CP_OPT(odf_types::color) & color, int type);


	void set_page_number_format		(_CP_OPT(int) & type, _CP_OPT(int) & start);

	office_element_ptr	root_header_footer_; //for top-level elements in style:footer
	
	double				current_page_width_;
	bool				even_and_left_headers_;
private:


	style_page_layout_properties	*get_properties();
	style_header_footer_properties	*get_header_properties();
	style_header_footer_properties	*get_footer_properties();

    odf_conversion_context	* odf_context_;
    odf_style_context_ptr style_context_;
    odf_style_context_ptr local_style_context_;
  
	std::vector<odf_layout_state> layout_state_list_;
 	std::vector<odf_master_state> master_state_list_;
	
};


}
}
