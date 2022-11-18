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
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class shadow_type
{
public:
    enum type
    {
        None,
        Enable
    };

    shadow_type() {}

    shadow_type(type _Type) : type_(_Type)
    {}

    shadow_type(const std::wstring & _Name) : type_(Enable), name_(_Name)
    {}

    type get_type() const
    {
        return type_;
    };

    const std::wstring & get_name() const
    {
        return name_;
    }
    
    static shadow_type parse(const std::wstring & Str);

private:
    type type_;
    std::wstring name_;

};

std::wostream & operator << (std::wostream & _Wostream, const shadow_type & _Val);


//--------------------------------------------------------------------------------------------------------------------------------
class shadow_type1
{
public:
    enum type
    {
        Hidden,
        Visible
    };

    shadow_type1() {}

    shadow_type1(type _Type) : type_(_Type)
    {}

    shadow_type1(const std::wstring & _Name) : type_(Visible), name_(_Name)
    {}

    type get_type() const
    {
        return type_;
    };

    const std::wstring & get_name() const
    {
        return name_;
    }
    
    static shadow_type1 parse(const std::wstring & Str);

private:
    type type_;
    std::wstring name_;

};

std::wostream & operator << (std::wostream & _Wostream, const shadow_type1 & _Val);

} 
APPLY_PARSE_XML_ATTRIBUTES(odf_types::shadow_type);
APPLY_PARSE_XML_ATTRIBUTES(odf_types::shadow_type1);
}
