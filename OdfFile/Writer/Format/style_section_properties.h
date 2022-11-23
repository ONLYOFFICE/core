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
#include <xml/attributes.h>
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/verticalalign.h"

namespace cpdoccore { 
namespace odf_writer {

/// style:columns
class style_columns : public office_element_impl<style_columns>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleColumns;
    

    virtual void create_child_element(  const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    _CP_OPT(unsigned int)		fo_column_count_;
    _CP_OPT(odf_types::length)	fo_column_gap_;

    office_element_ptr			style_column_sep_;
    office_element_ptr_array	style_column_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_columns);

///		style:column
class style_column : public office_element_impl<style_column>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleColumn;
    

    virtual void create_child_element(  const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child){}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(odf_types::percent_rel)	style_rel_width_;
	_CP_OPT(odf_types::length)		fo_start_indent_;
    _CP_OPT(odf_types::length)		fo_end_indent_;
    _CP_OPT(odf_types::length)		fo_space_before_;
    _CP_OPT(odf_types::length)		fo_space_after_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_column);

/// style:column-sep
class style_column_sep : public office_element_impl<style_column_sep>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleColumnSep;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child){}
	
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring)	style_style_;			 // default solid
    _CP_OPT(odf_types::length)			style_width_;
	_CP_OPT(odf_types::percent)		style_height_;			// default 100
    _CP_OPT(odf_types::vertical_align) style_vertical_align_;	// default top
    _CP_OPT(odf_types::color)			style_color_;			// default #000000
};

CP_REGISTER_OFFICE_ELEMENT2(style_column_sep);

///         style:section-properties
class style_section_properties : public office_element_impl<style_section_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleSectionProperties;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    odf_types::common_background_color_attlist		common_background_color_attlist_;
    odf_types::common_horizontal_margin_attlist		common_horizontal_margin_attlist_;
    odf_types::common_writing_mode_attlist			common_writing_mode_attlist_;

	_CP_OPT(odf_types::Bool)						style_editable_;
    _CP_OPT(odf_types::Bool)						style_protect_; // default false
    _CP_OPT(odf_types::Bool)						text_dont_balance_text_columns_;
 
    office_element_ptr								style_columns_;
    office_element_ptr								style_background_image_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_section_properties);
}
}