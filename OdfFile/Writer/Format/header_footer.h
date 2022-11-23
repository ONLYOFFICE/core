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

    style_header_footer_properties_attlist	style_header_footer_properties_attlist_;
    office_element_ptr						style_background_image_;
    
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
