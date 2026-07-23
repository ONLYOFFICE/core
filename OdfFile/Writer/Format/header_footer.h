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

#include <iosfwd>
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"


namespace cpdoccore { 
namespace odf_writer {


// style:header-footer-properties-attlist
class style_header_footer_properties_attlist
{
public:
	void serialize(CP_ATTR_NODE);

    _CP_OPT(odf_types::length)						svg_height_;
    _CP_OPT(odf_types::length)						fo_min_height_;
    odf_types::common_horizontal_margin_attlist		common_horizontal_margin_attlist_;
    odf_types::common_vertical_margin_attlist		common_vertical_margin_attlist_;
    odf_types::common_margin_attlist				common_margin_attlist_;
    odf_types::common_border_attlist				common_border_attlist_;
    odf_types::common_border_line_width_attlist		common_border_line_width_attlist_;
    odf_types::common_padding_attlist				common_padding_attlist_;
    odf_types::common_background_color_attlist		common_background_color_attlist_;
    odf_types::common_shadow_attlist				common_shadow_attlist_;
    _CP_OPT(odf_types::Bool)						style_dynamic_spacing_;    
};

// style:header-footer-properties
class style_header_footer_properties : public office_element_impl<style_header_footer_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleHeaderFooterProperties;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child)
    {
        _CP_LOG << L"Non add child in " << ns << L":" << name << std::endl;
    }
	virtual void serialize(std::wostream & strm);

    style_header_footer_properties_attlist style_header_footer_properties_attlist_;
    office_element_ptr style_background_image_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(style_header_footer_properties)


// common:style-header-footer-attlist
class common_style_header_footer_attlist
{
public:

    bool style_display_; // default true
    
};

// header:footer-content
class header_footer_content
{
public:
    void create_child_element	( const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
    void add_child_element		( const office_element_ptr & child, odf_conversion_context * Context);

	void serialize(std::wostream & strm);
private:
	office_element_ptr			tracked_changes_;
    office_element_ptr_array	content_;
    
};

}
}
