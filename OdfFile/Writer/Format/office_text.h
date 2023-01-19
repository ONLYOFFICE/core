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

#include <xml/simple_xml_writer.h>

#include "../../DataTypes/bool.h"

namespace cpdoccore { 
namespace odf_writer {

class office_text_attlist
{
public:
    _CP_OPT(odf_types::Bool) text_use_soft_page_breaks_;

	void serialize(CP_ATTR_NODE);
};

class office_text : public office_element_impl<office_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeOfficeText;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	office_text_attlist			office_text_attlist_;

private:
	office_element_ptr			tracked_changes_;
	office_element_ptr			table_content_validations_;
	office_element_ptr			user_fields_;
	office_element_ptr			variables_;
 	office_element_ptr			sequences_;
	office_element_ptr			forms_;

	office_element_ptr_array	content_;
	
	// TODO: office-text-content-prelude:
    // TODO: text-decls
    // TODO: table-decls
	// TODO: text-page-sequence
    // TODO: office-text-content-epilogue:
    // TODO: table-functions
};
CP_REGISTER_OFFICE_ELEMENT2(office_text)

//-------------------------------------------------------------------------------------
class office_change_info : public office_element_impl<office_change_info>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeOfficeChangeInfo;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

private:
    office_element_ptr		dc_date_;
    office_element_ptr		dc_creator_;
	
};
CP_REGISTER_OFFICE_ELEMENT2(office_change_info)


}
}

