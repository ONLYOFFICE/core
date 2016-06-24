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

#include <string>
#include <iosfwd>
#include <cpdoccore/xml/xmlelement.h>

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
    ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

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
    ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

    const std::wstring & part_name() const { return part_name_; }
    const std::wstring & content_type() const { return content_type_; }

public:
    std::wstring part_name_;
    std::wstring content_type_;

};

///////////////////////////////////////////////////////////////////////////////////////
// content_type_content
class content_type_content : public xml::element_impl<content_type_content>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;

    ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

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
