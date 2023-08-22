/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

class sparkline_type
{
public:
    enum type
    {
        Line = 0,
        Column = 1,
        Stacked = 2
    };

    sparkline_type() {}

    sparkline_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static sparkline_type parse(const std::wstring & Str);

private:
    type type_;
};
std::wostream& operator << (std::wostream& _Wostream, const sparkline_type& _Val);
//-----------------------------------------------------------------------------------------------
class sparkline_empty
{
public:
    enum type
    {
        Span = 0,
        Gap = 1,
        Zero = 2
    };

    sparkline_empty() {}

    sparkline_empty(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };

    static sparkline_empty parse(const std::wstring& Str);

private:
    type type_;
};
std::wostream& operator << (std::wostream& _Wostream, const sparkline_empty& _Val);
//---------------------------------------------------------------------------------------------------
class sparkline_axis_type
{
public:
    enum type
    {
        Individual = 0,
        Group = 1,
        Custom = 2
    };

    sparkline_axis_type() {}

    sparkline_axis_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };

    static sparkline_axis_type parse(const std::wstring& Str);

private:
    type type_;
};
std::wostream& operator << (std::wostream& _Wostream, const sparkline_axis_type& _Val);
}
APPLY_PARSE_XML_ATTRIBUTES(odf_types::sparkline_type);
APPLY_PARSE_XML_ATTRIBUTES(odf_types::sparkline_axis_type);
APPLY_PARSE_XML_ATTRIBUTES(odf_types::sparkline_empty);
}

