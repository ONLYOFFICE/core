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
#ifndef _CPDOCCORE_ODF_HORIZONTALREL_H_
#define _CPDOCCORE_ODF_HORIZONTALREL_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class horizontal_rel
{
public:
    enum type
    {
        Page,
        PageContent,
        PageStartMargin,
        PageEndMargin,
        Frame,
        FrameContent,
        FrameStartMargin,
        FrameEndMargin,
        Paragraph,
        ParagraphContent,
        ParagraphStartMargin,
        ParagraphEndMargin,
        Char
    };

    horizontal_rel() {}

    horizontal_rel(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
	std::wstring get_type_str() const
	{
		switch(type_)
		{
		case horizontal_rel::Page:
			return L"page";
		case horizontal_rel::Paragraph:
		case horizontal_rel::ParagraphContent:
			return L"column";
		case horizontal_rel::PageEndMargin:
			return L"rightMargin";
		case horizontal_rel::PageStartMargin:
			return L"leftMargin";
		}
		return L"margin";
	}
   
    static horizontal_rel parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const horizontal_rel & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::horizontal_rel);

}

#endif
