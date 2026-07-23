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

#include "Palette.h"

namespace XLS
{

Palette::Palette()
:	ccv(56)
{
}


Palette::Palette(const unsigned short ccv_init)
:	ccv(ccv_init)
{

}


BaseObjectPtr Palette::clone()
{
	return BaseObjectPtr(new Palette(*this));
}


const std::wstring standart_color[8] = {
										L"00000000",
										L"00FFFFFF",
										L"00FF0000",
										L"0000FF00",
										L"000000FF",
										L"00FFFF00",
										L"00FF00FF",
										L"0000FFFF"};


void Palette::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	unsigned short ccv;
	record >> ccv;
	
	for(int i = 0; i < 8; ++i)
	{	
		global_info->RegisterPaletteColor(i, standart_color[i].substr(2,6));
	}

	for(int i = 0; i < ccv; ++i)
	{
		LongRGBPtr rgb(new LongRGB);
		record >> *rgb;
		rgColor.push_back(rgb);

		global_info->RegisterPaletteColor(i+8, rgb->strRGB);
	}
}

void Palette::writeFields(CFRecord& record)
{
    ccv = rgColor.size();
    record << ccv;
    for(auto i: rgColor)
        if(i != nullptr)
			i->save(record);
}



int Palette::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"indexedColors")
		{		
			for(int i = 0; i < 8; ++i)
			{	
				CP_XML_NODE(L"rgbColor")
				{
					CP_XML_ATTR(L"rgb", standart_color[i]);
				}
			}

			for(size_t i = 0; i < rgColor.size(); ++i)
			{		
				LongRGB * rgb = dynamic_cast<LongRGB *>(rgColor[i].get());
				CP_XML_NODE(L"rgbColor")
				{
					CP_XML_ATTR(L"rgb", rgb->strARGB);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

