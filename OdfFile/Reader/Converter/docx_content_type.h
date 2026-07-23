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
#include <iosfwd>
#include <vector>

#include <xml/xmlelement.h>

namespace cpdoccore { 
namespace oox {

///////////////////////////////////////////////////////////////////////////////////////    
// default_content_type
class default_content_type : public xml::element_impl<default_content_type>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;

public:
    default_content_type(const std::wstring & Extension, const std::wstring & contentType) : extension_(Extension),
        content_type_(contentType)
    {}
    std::wostream & xml_to_stream(std::wostream & _Wostream) const;

    const std::wstring & extension() const { return extension_; }
    const std::wstring & content_type() const { return content_type_; }
    
private:
    std::wstring extension_;
    std::wstring content_type_;
};

///////////////////////////////////////////////////////////////////////////////////////

class override_content_type : public xml::element_impl<override_content_type>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;

public:
    override_content_type(const std::wstring & PartName, const std::wstring & contentType) : part_name_(PartName),
        content_type_(contentType)
    {}
    std::wostream & xml_to_stream(std::wostream & _Wostream) const;

    const std::wstring & part_name()	const { return part_name_; }
    const std::wstring & content_type() const { return content_type_; }

public:
    std::wstring part_name_;
    std::wstring content_type_;

};

///////////////////////////////////////////////////////////////////////////////////////
// content_type
class content_type : public xml::element_impl<content_type>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;

    std::wostream & xml_to_stream(std::wostream & _Wostream) const;

    void add_default(default_content_type const & _Ct)
    {
        get_default().push_back(_Ct);        
    }

    void add_default(std::wstring const & Extension, std::wstring const & contentType)
    {
        add_default(default_content_type(Extension, contentType));
    }

    void add_override(override_content_type const & _Ct)
    {
        get_override().push_back(_Ct);
    }

    void add_override(std::wstring const & PartName, std::wstring const & contentType)
    {
        add_override(override_content_type(PartName, contentType));    
    }

    std::vector<default_content_type>	& get_default()		{ return default_; };
    std::vector<override_content_type>	& get_override()	{ return override_; };

private:
    std::vector<default_content_type>	default_;
    std::vector<override_content_type>	override_;

};

}
}
